#!/usr/bin/env python3
import argparse
import base64
import hashlib
import json
import os
import re
import subprocess
import sys
import tempfile
import threading
import time
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path
from urllib.error import HTTPError, URLError
from urllib.request import Request, urlopen

SITE = "https://judge.yosupo.jp"
API = "https://v3.api.judge.yosupo.jp"
LANG = "cpp20"
SOURCE_LIMIT = 1 << 20
WINDOWS_STACK_BYTES = 1 << 30
LOCAL_INCLUDE_RE = re.compile(
    r'^[ \t]*#[ \t]*include\b[^"\r\n]*"([^"\r\n]+)"'
)
PROGRESS_RE = re.compile(r"^\d+/\d+$")
PENDING_STATUSES = {"WJ", "-", "Fetching", "Compiling"}
FINAL_STATUSES = {
    "AC",
    "WA",
    "RE",
    "TLE",
    "MLE",
    "OLE",
    "PE",
    "Fail",
    "CE",
    "IE",
    "ICE",
    "ITLE",
    "Unknown",
}
SUBMIT_LOCK = threading.Lock()


class SubmissionError(RuntimeError):
    pass


def local_linker_args() -> tuple[str, ...]:
    # MinGW 默认栈不足以稳定运行深递归的本地候选、测试和官方工具。
    # 该参数是 Windows 链接器专用配置，不能传给 Linux/在线评测环境。
    if os.name == "nt":
        return (f"-Wl,--stack,{WINDOWS_STACK_BYTES}",)
    return ()


@dataclass(frozen=True)
class SourceItem:
    name: str
    title: str
    path: Path
    meta_path: Path
    source_sha256: str
    category: str = ""
    language: str = LANG


@dataclass(frozen=True)
class PreparedSource:
    item: SourceItem
    source: str
    source_sha256: str


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_text(text: str) -> str:
    return sha256_bytes(text.encode("utf-8"))


def sha256_file(path: Path) -> str:
    return sha256_bytes(path.read_bytes())


def utc_now() -> str:
    return datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")


def read_submission_source(path: Path) -> tuple[str, str]:
    try:
        data = path.read_bytes()
    except OSError as error:
        raise SubmissionError(f"无法读取源码：{path}") from error
    if data.startswith(b"\xef\xbb\xbf"):
        raise SubmissionError(f"{path}: 独立源码不能包含 UTF-8 BOM")
    if b"\r" in data:
        raise SubmissionError(f"{path}: 独立源码必须使用 LF 换行，不能包含 CRLF")
    try:
        source = data.decode("utf-8")
    except UnicodeDecodeError as error:
        raise SubmissionError(f"文件不是有效 UTF-8：{path}") from error
    logical_source = source.replace("\\\n", "")
    for line in logical_source.splitlines():
        match = LOCAL_INCLUDE_RE.match(line)
        if match is not None:
            raise SubmissionError(
                f'{path}: 独立源码不能包含本地 include "{match.group(1)}"'
            )
    return source, sha256_bytes(data)


def solution_scope(
    entry: dict,
    meta: dict,
    actual_sha256: str,
) -> None:
    provenance = meta.get("provenance")
    if meta.get("schema_version") != 3 or not isinstance(provenance, dict):
        raise SubmissionError(f"{entry.get('name')}: 当前模板题解元数据不是 schema 3")
    checks = {
        "name": (meta.get("name"), entry.get("name")),
        "title": (meta.get("title"), entry.get("title")),
        "category": (meta.get("category"), entry.get("category")),
        "provenance.kind": (provenance.get("kind"), "template_solution"),
        "provenance.source": (provenance.get("source"), entry.get("source")),
        "provenance.source_sha256": (
            provenance.get("source_sha256"),
            actual_sha256,
        ),
    }
    for field, (actual, expected) in checks.items():
        if actual != expected:
            raise SubmissionError(
                f"{entry.get('name')}: {field} 不匹配："
                f"期望 {expected!r}，实际 {actual!r}"
            )
    if "template_headers" in provenance:
        raise SubmissionError(
            f"{entry.get('name')}: 独立源码元数据不能包含 template_headers"
        )


def load_items(
    root: Path,
    manifest: dict,
    wanted: set[str] | None,
    solution_only: bool = False,
) -> list[SourceItem]:
    root = root.resolve()
    entries = manifest.get("problems")
    if not isinstance(entries, list):
        raise SubmissionError("manifest 缺少 problems 数组")
    if manifest.get("schema_version") != 3:
        raise SubmissionError("manifest 不是 schema 3")
    problem_count = manifest.get("problem_count")
    if type(problem_count) is not int or problem_count != len(entries):
        raise SubmissionError("manifest problem_count 与 problems 数量不符")
    counts = manifest.get("counts")
    if counts != {"template_solution": len(entries)}:
        raise SubmissionError("manifest counts 与独立模板题解数量不符")
    items = []
    rejected = []
    rejected_names = set()
    seen_names = set()
    for entry in entries:
        if not isinstance(entry, dict):
            raise SubmissionError("manifest 的 problems 包含非对象条目")
        name = entry.get("name")
        if not isinstance(name, str) or re.fullmatch(r"[A-Za-z0-9_]+", name) is None:
            raise SubmissionError("manifest 包含无效题目名")
        if name in seen_names:
            raise SubmissionError(f"manifest 包含重复题目：{name}")
        seen_names.add(name)

    for entry in entries:
        name = entry["name"]
        if wanted is not None and name not in wanted:
            continue
        if entry.get("state") != "template_solution":
            raise SubmissionError(f"{name}: manifest state 不是 template_solution")
        try:
            path = (root / entry["source"]).resolve()
            meta_path = (root / entry["meta"]).resolve()
            path.relative_to(root)
            meta_path.relative_to(root)
        except (KeyError, ValueError) as error:
            raise SubmissionError(f"题目 {name!r} 的清单路径无效") from error
        if not path.is_file():
            raise SubmissionError(f"缺少代码：{path}")
        if not meta_path.is_file():
            raise SubmissionError(f"缺少元数据：{meta_path}")
        try:
            meta = json.loads(meta_path.read_text(encoding="utf-8"))
        except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
            raise SubmissionError(f"无法读取元数据：{meta_path}") from error
        if not isinstance(meta, dict):
            raise SubmissionError(f"元数据根节点不是对象：{meta_path}")
        provenance = meta.get("provenance")
        kind = provenance.get("kind") if isinstance(provenance, dict) else None
        if solution_only and kind != "template_solution":
            rejected.append(f"{name} ({kind!r})")
            rejected_names.add(name)
            continue
        source_sha256 = entry.get("source_sha256")
        if not isinstance(source_sha256, str) or re.fullmatch(
            r"[0-9a-f]{64}",
            source_sha256,
        ) is None:
            raise SubmissionError(f"题目 {name!r} 的 source_sha256 无效")
        actual_sha256 = sha256_file(path)
        if actual_sha256 != source_sha256:
            raise SubmissionError(
                f"{name}: manifest 源码哈希已过期："
                f"记录 {source_sha256}，实际 {actual_sha256}"
            )
        if solution_only:
            solution_scope(
                entry,
                meta,
                actual_sha256,
            )
        items.append(SourceItem(
            name=name,
            title=entry.get("title", ""),
            path=path,
            meta_path=meta_path,
            source_sha256=source_sha256,
            category=entry.get("category", ""),
        ))
    if wanted is not None:
        found = {item.name for item in items}
        missing = wanted - found - rejected_names
        if missing:
            raise SubmissionError("未知题目：" + ", ".join(sorted(missing)))
    if rejected:
        raise SubmissionError(
            "拒绝提交非当前模板题解：" + ", ".join(rejected)
        )
    return items


def prepare_sources(items: list[SourceItem]) -> list[PreparedSource]:
    prepared = []
    for item in items:
        source, actual_sha256 = read_submission_source(item.path)
        if actual_sha256 != item.source_sha256:
            raise SubmissionError(f"{item.name}: 源码在读取清单后发生变化")
        size = len(source.encode("utf-8"))
        if size > SOURCE_LIMIT:
            raise SubmissionError(
                f"{item.name}: 源码为 {size} 字节，超过 Library Checker 的 1 MiB 限制"
            )
        prepared.append(PreparedSource(item, source, actual_sha256))
    return prepared


def compile_sources(
    prepared: list[PreparedSource],
    compiler: str = "g++",
    timeout: float = 180.0,
) -> None:
    with tempfile.TemporaryDirectory(prefix="yosupo-compile-") as directory:
        build_dir = Path(directory)
        for index, source in enumerate(prepared):
            source_path = build_dir / f"{index}-{source.item.name}.cpp"
            output_path = build_dir / f"{index}-{source.item.name}.exe"
            with source_path.open("w", encoding="utf-8", newline="\n") as file:
                file.write(source.source)
            command = [
                compiler,
                "-std=gnu++20",
                "-O2",
                "-pipe",
                "-DONLINE_JUDGE",
                *local_linker_args(),
                str(source_path),
                "-o",
                str(output_path),
            ]
            try:
                result = subprocess.run(
                    command,
                    capture_output=True,
                    text=True,
                    encoding="utf-8",
                    errors="replace",
                    timeout=timeout,
                    check=False,
                )
            except FileNotFoundError as error:
                raise SubmissionError(f"找不到编译器：{compiler}") from error
            except subprocess.TimeoutExpired as error:
                raise SubmissionError(
                    f"{source.item.name}: 本地完整编译超过 {timeout:g} 秒"
                ) from error
            if result.returncode != 0:
                message = (result.stderr or result.stdout).strip()
                if len(message) > 8000:
                    message = message[-8000:]
                raise SubmissionError(
                    f"{source.item.name}: gnu++20 完整编译失败\n{message}"
                )
            print(f"COMPILED {source.item.name}")


def read_json(url: str, token: str | None = None, timeout: float = 30.0) -> dict:
    headers = {"Authorization": f"Bearer {token}"} if token else {}
    with urlopen(Request(url, headers=headers), timeout=timeout) as response:
        try:
            data = json.load(response)
        except (UnicodeDecodeError, json.JSONDecodeError) as error:
            raise SubmissionError(f"接口返回的不是有效 JSON：{url}") from error
    if not isinstance(data, dict):
        raise SubmissionError(f"接口返回格式错误：{url}")
    return data


def submit(
    api: str,
    token: str,
    problem: str,
    source: str,
    language: str = LANG,
) -> int:
    data = json.dumps({
        "problem": problem,
        "source": source,
        "lang": language,
        "tle_knockout": True,
    }).encode()
    request = Request(
        f"{api}/submit",
        data=data,
        headers={
            "Authorization": f"Bearer {token}",
            "Content-Type": "application/json",
        },
        method="POST",
    )
    try:
        with urlopen(request, timeout=60) as response:
            result = json.load(response)
    except HTTPError as error:
        detail = error.read().decode(errors="replace")
        raise SubmissionError(f"HTTP {error.code}: {detail}") from error
    except (URLError, TimeoutError) as error:
        raise SubmissionError(str(error)) from error
    except (UnicodeDecodeError, json.JSONDecodeError) as error:
        raise SubmissionError("提交接口返回的不是有效 JSON") from error
    submission_id = result.get("id") if isinstance(result, dict) else None
    if (
        not isinstance(submission_id, int)
        or isinstance(submission_id, bool)
        or submission_id < 0
    ):
        raise SubmissionError("提交接口没有返回合法的 submission id")
    return submission_id


def profile_token(page: object) -> str | None:
    # page 已打开 judge.yosupo.jp；从该源的 IndexedDB 读取 Firebase 访问令牌。
    session = profile_session(page)
    token = session.get("access_token") if session is not None else None
    return token if isinstance(token, str) else None


def profile_session(page: object) -> dict[str, str] | None:
    # page 已打开 judge.yosupo.jp；返回内存刷新所需的 Firebase 会话字段。
    session = page.evaluate("""
        async () => {
          const db = await new Promise((ok, bad) => {
            const r = indexedDB.open('firebaseLocalStorageDb');
            r.onsuccess = () => ok(r.result);
            r.onerror = () => bad(r.error);
          });
          const rows = await new Promise((ok, bad) => {
            const r = db.transaction('firebaseLocalStorage')
              .objectStore('firebaseLocalStorage').getAll();
            r.onsuccess = () => ok(r.result);
            r.onerror = () => bad(r.error);
          });
          const value = rows.map(x => x.value).find(x =>
            x?.stsTokenManager?.accessToken && x?.stsTokenManager?.refreshToken && x?.apiKey
          );
          if (!value) return null;
          return {
            access_token: value.stsTokenManager.accessToken,
            refresh_token: value.stsTokenManager.refreshToken,
            api_key: value.apiKey,
          };
        }
    """)
    if not isinstance(session, dict):
        return None
    required = ("access_token", "refresh_token", "api_key")
    return session if all(isinstance(session.get(key), str) and session[key] for key in required) else None


def refresh_token(session: dict[str, str]) -> str | None:
    # session 是 profile_session 的结果；用 Firebase refresh token 取得新的短期访问令牌。
    data = f"grant_type=refresh_token&refresh_token={session['refresh_token']}".encode()
    request = Request(
        f"https://securetoken.googleapis.com/v1/token?key={session['api_key']}",
        data=data,
        headers={"Content-Type": "application/x-www-form-urlencoded"},
        method="POST",
    )
    try:
        with urlopen(request, timeout=30) as response:
            result = json.load(response)
    except (HTTPError, URLError, TimeoutError, UnicodeDecodeError, json.JSONDecodeError):
        return None
    token = result.get("id_token") if isinstance(result, dict) else None
    return token if isinstance(token, str) and token_is_fresh(token) else None


def token_is_fresh(token: str, margin: int = 60) -> bool:
    # token 是 Firebase JWT；仅在离过期仍有 margin 秒时允许静默复用。
    try:
        payload = token.split(".")[1]
        payload += "=" * (-len(payload) % 4)
        data = json.loads(base64.urlsafe_b64decode(payload).decode("utf-8"))
        expires_at = data.get("exp")
        return isinstance(expires_at, (int, float)) and time.time() + margin < expires_at
    except (IndexError, UnicodeDecodeError, ValueError, json.JSONDecodeError):
        return False


def browser_auth(profile: Path) -> str:
    try:
        from playwright.sync_api import sync_playwright
    except ImportError as error:
        raise SubmissionError(
            "缺少 Playwright：python -m pip install playwright"
        ) from error

    with sync_playwright() as playwright:
        context = None
        try:
            # 正常每题只无头读取持久登录态，既不弹窗也不要求用户重复登录。
            context = playwright.chromium.launch_persistent_context(
                str(profile), headless=True,
            )
            page = context.pages[0] if context.pages else context.new_page()
            page.goto(SITE, wait_until="domcontentloaded")
            session = profile_session(page)
            token = session.get("access_token") if session is not None else None
            if token is None or not token_is_fresh(token):
                token = refresh_token(session) if session is not None else None
        except Exception as error:
            raise SubmissionError(f"读取持久登录会话失败：{error}") from error
        finally:
            if context is not None:
                context.close()
        if token is not None and token_is_fresh(token):
            return token

        context = None
        try:
            # 只有本地没有有效令牌时才显示登录页并等待一次用户操作。
            context = playwright.chromium.launch_persistent_context(
                str(profile), headless=False,
            )
            page = context.pages[0] if context.pages else context.new_page()
            page.goto(f"{SITE}/login", wait_until="domcontentloaded")
            try:
                input("未找到有效登录令牌；请在浏览器中完成登录，然后回到这里按回车：")
            except EOFError:
                pass
            session = profile_session(page)
            token = session.get("access_token") if session is not None else None
            if token is None or not token_is_fresh(token):
                token = refresh_token(session) if session is not None else None
        except Exception as error:
            raise SubmissionError(f"登录或读取登录令牌失败：{error}") from error
        finally:
            if context is not None:
                context.close()
    if token is None or not token_is_fresh(token):
        raise SubmissionError("没有取得有效登录令牌，请完成网页登录后重试。")
    return token


def check_language(api: str, language: str) -> None:
    langs = read_json(f"{api}/langs").get("langs")
    if not isinstance(langs, list):
        raise SubmissionError("/langs 接口缺少 langs 数组")
    if not any(
        isinstance(item, dict) and item.get("id") == language for item in langs
    ):
        raise SubmissionError(f"Library Checker 当前没有 {language} 语言")


def check_cpp20(api: str) -> None:
    check_language(api, LANG)


def validate_submission(
    data: dict,
    prepared: PreparedSource,
    submission_id: int,
) -> tuple[dict, str]:
    overview = data.get("overview")
    server_source = data.get("source")
    if not isinstance(overview, dict) or not isinstance(server_source, str):
        raise SubmissionError("提交详情缺少 overview 或 source")
    checks = {
        "submission id": (overview.get("id"), submission_id),
        "题目名": (overview.get("problem_name"), prepared.item.name),
        "题目标题": (overview.get("problem_title"), prepared.item.title),
        "语言": (overview.get("lang"), prepared.item.language),
    }
    for field, (actual, expected) in checks.items():
        if actual != expected:
            raise SubmissionError(
                f"提交详情的{field}不匹配：期望 {expected!r}，实际 {actual!r}"
            )
    server_sha256 = sha256_text(server_source)
    if server_sha256 != prepared.source_sha256:
        raise SubmissionError(
            f"服务端源码 SHA256 不匹配：期望 {prepared.source_sha256}，"
            f"实际 {server_sha256}"
        )
    return overview, server_sha256


def is_final_status(status: str) -> bool:
    if status in PENDING_STATUSES or PROGRESS_RE.fullmatch(status):
        return False
    if status in FINAL_STATUSES:
        return True
    raise SubmissionError(f"无法识别 Library Checker 状态：{status!r}")


def wait_submission(
    api: str,
    prepared: PreparedSource,
    submission_id: int,
    timeout: float = 1800.0,
    interval: float = 3.0,
) -> dict:
    deadline = time.monotonic() + timeout
    last_status = None
    while True:
        try:
            data = read_json(f"{api}/submissions/{submission_id}")
        except (HTTPError, URLError, TimeoutError, OSError) as error:
            if time.monotonic() >= deadline:
                raise SubmissionError(
                    f"等待 submission {submission_id} 时网络超时：{error}"
                ) from error
            print(f"RETRY {prepared.item.name}: {error}")
            time.sleep(interval)
            continue

        overview, _ = validate_submission(data, prepared, submission_id)
        status = overview.get("status")
        if not isinstance(status, str):
            raise SubmissionError("提交详情缺少字符串 status")
        if status != last_status:
            print(f"JUDGING {prepared.item.name}: {status}")
            last_status = status
        if is_final_status(status):
            return data
        if time.monotonic() >= deadline:
            raise SubmissionError(
                f"等待 submission {submission_id} 超过 {timeout:g} 秒，"
                f"最后状态为 {status}"
            )
        time.sleep(interval)


def atomic_write_json(path: Path, data: dict) -> None:
    temporary = None
    try:
        with tempfile.NamedTemporaryFile(
            "w",
            encoding="utf-8",
            newline="\n",
            dir=path.parent,
            prefix=f".{path.name}.",
            suffix=".tmp",
            delete=False,
        ) as file:
            temporary = Path(file.name)
            json.dump(data, file, ensure_ascii=False, indent=2)
            file.write("\n")
            file.flush()
            os.fsync(file.fileno())
        os.replace(temporary, path)
        temporary = None
    finally:
        if temporary is not None:
            temporary.unlink(missing_ok=True)


def current_fingerprint(prepared: PreparedSource) -> tuple[str | None, str | None]:
    try:
        _, source_sha256 = read_submission_source(prepared.item.path)
        return source_sha256, None
    except SubmissionError as error:
        return None, str(error)


def check_current(prepared: PreparedSource) -> None:
    source_sha256, error = current_fingerprint(prepared)
    if error is not None:
        raise SubmissionError(
            f"{prepared.item.name}: 提交前无法重新读取当前源码：{error}"
        )
    if source_sha256 != prepared.source_sha256:
        raise SubmissionError(
            f"{prepared.item.name}: 源码在提交前发生变化，拒绝发送旧源码"
        )


def ch03_batch_path(root: Path, path: Path | None) -> Path:
    # root 是题解目录，path 是第 03 章批次路径；只接受该目录 .local 内的文件。
    if path is None:
        raise SubmissionError("第 03 章在线提交必须提供 --batch 当前单题批次")
    root = root.resolve()
    target = path.resolve() if path.is_absolute() else (root / path).resolve()
    local = (root / ".local").resolve()
    try:
        target.relative_to(local)
    except ValueError as error:
        raise SubmissionError("第 03 章批次必须位于 library-checker-template-solutions/.local/") from error
    if not target.is_file():
        raise SubmissionError(f"第 03 章批次不存在：{target}")
    return target


def validate_ch03_batch(root: Path, prepared: PreparedSource, path: Path | None) -> None:
    # root 是题解目录，prepared 是待发送源码；重新计算其单题批次的全部技术门禁。
    if prepared.item.category != "data_structure":
        if path is not None:
            raise SubmissionError("--batch 只用于第 03 章 data_structure 提交")
        return
    batch = ch03_batch_path(root, path)
    command = [
        sys.executable,
        str(root / "tools" / "ch03_batch_report.py"),
        "--only",
        prepared.item.name,
        "--revalidate-batch",
        str(batch),
    ]
    try:
        result = subprocess.run(
            command,
            cwd=str(root.parent),
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
            timeout=180,
            check=False,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise SubmissionError(f"第 03 章批次重新核验失败：{error}") from error
    if result.returncode:
        detail = (result.stderr or result.stdout).strip()
        if len(detail) > 4000:
            detail = detail[-4000:]
        suffix = f"\n{detail}" if detail else ""
        raise SubmissionError(f"第 03 章批次已失效，拒绝提交{suffix}")


def attempt_is_ac(
    attempt: dict,
    source_sha256: str | None,
) -> bool:
    submission_id = attempt.get("submission_id")
    return (
        attempt.get("source_sha256") == source_sha256
        and attempt.get("server_source_sha256") == source_sha256
        and attempt.get("lang") == LANG
        and attempt.get("verdict") == "AC"
        and attempt.get("is_latest") is True
        and type(submission_id) is int
        and submission_id >= 0
    )


def refresh_validation(
    meta: dict,
    source_sha256: str | None,
    fingerprint_error: str | None,
) -> dict:
    validation = meta.get("validation")
    if not isinstance(validation, dict):
        validation = {}
    validation["checked_at"] = utc_now()
    validation["current_source_sha256"] = source_sha256
    validation.pop("current_adapter_sha256", None)
    validation.pop("current_expanded_sha256", None)

    attempts = meta.get("submission_attempts")
    matching = []
    if isinstance(attempts, list):
        matching = sorted(
            (
                attempt
                for attempt in attempts
                if isinstance(attempt, dict)
                and attempt.get("source_sha256") == source_sha256
            ),
            key=lambda attempt: (
                attempt.get("recorded_at")
                if isinstance(attempt.get("recorded_at"), str)
                else "",
                attempt.get("submission_id")
                if type(attempt.get("submission_id")) is int
                else -1,
            ),
        )

    if fingerprint_error is not None:
        validation.update({
            "status": "local_only",
            "current_source_online_verdict": None,
            "current_source_submission_id": None,
            "current_source_time": None,
            "current_source_memory": None,
            "current_source_submission_time": None,
            "current_source_is_latest": None,
            "server_source_sha256": None,
            "note": f"当前源码无法重新读取，未认证在线结果：{fingerprint_error}",
        })
        return validation
    if not matching:
        validation.update({
            "status": "local_only",
            "current_source_online_verdict": None,
            "current_source_submission_id": None,
            "current_source_time": None,
            "current_source_memory": None,
            "current_source_submission_time": None,
            "current_source_is_latest": None,
            "server_source_sha256": None,
            "note": "当前源码哈希没有对应的在线提交结果。",
        })
        return validation

    attempt = matching[-1]
    verdict = attempt.get("verdict")
    submission_id = attempt.get("submission_id")
    has_submission_id = type(submission_id) is int and submission_id >= 0
    server_sha256 = attempt.get("server_source_sha256")
    if verdict is None:
        # A POST without a confirmed id may already have reached the judge and
        # must never be treated as an ordinary retryable error.  Once an id is
        # known, polling failures retain submitted so the same id can be
        # resumed instead of issuing a second POST.
        if has_submission_id:
            status = "submitted"
        else:
            status = "submission_unknown"
    elif (
        server_sha256 != source_sha256
        or attempt.get("lang") != LANG
        or type(attempt.get("submission_id")) is not int
        or attempt.get("submission_id") < 0
    ):
        status = "local_only"
    elif attempt_is_ac(attempt, source_sha256):
        status = "online_ac"
    elif verdict == "AC":
        status = "online_stale"
    else:
        status = "online_failed"

    validation.update({
        "status": status,
        "current_source_online_verdict": verdict,
        "current_source_submission_id": attempt.get("submission_id"),
        "current_source_time": attempt.get("time"),
        "current_source_memory": attempt.get("memory"),
        "current_source_submission_time": attempt.get("submission_time"),
        "current_source_is_latest": attempt.get("is_latest"),
        "server_source_sha256": server_sha256,
        "note": (
            "当前独立源码已通过 Library Checker 在线评测。"
            if status == "online_ac"
            else "当前源码尚未取得可认证的最新测试 AC。"
        ),
    })
    return validation


def reconcile_validation(prepared: PreparedSource) -> str:
    """根据当前源码和既有提交记录恢复可重算的认证摘要。"""
    with SUBMIT_LOCK:
        try:
            meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
        except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
            raise SubmissionError(f"无法读取元数据：{prepared.item.meta_path}") from error
        if not isinstance(meta, dict):
            raise SubmissionError(f"元数据根节点不是对象：{prepared.item.meta_path}")

        # refresh_validation 会原地更新旧摘要，先序列化保存以避免无意义写文件。
        before_value = meta.get("validation")
        before = json.dumps(
            before_value,
            ensure_ascii=False,
            sort_keys=True,
            separators=(",", ":"),
        )
        source_sha256, fingerprint_error = current_fingerprint(prepared)
        validation = refresh_validation(meta, source_sha256, fingerprint_error)
        after = json.dumps(
            validation,
            ensure_ascii=False,
            sort_keys=True,
            separators=(",", ":"),
        )
        # 认证摘要已经完整时不因一次只读预检刷新 checked_at，避免批次前哈希抖动。
        before_semantic = json.loads(before) if before != "null" else {}
        after_semantic = json.loads(after)
        if isinstance(before_semantic, dict):
            before_semantic.pop("checked_at", None)
        after_semantic.pop("checked_at", None)
        status = validation.get("status")
        if not isinstance(status, str):
            raise SubmissionError(f"{prepared.item.name}: validation.status 无效")
        # 单题批次绑定提交前整个 meta.json。尚未认证为 AC 的候选仍可能
        # 继续发送，预检绝不能为了更新说明文字而改写它的批次身份。
        if status == "online_ac" and before_semantic != after_semantic:
            meta["validation"] = validation
            atomic_write_json(prepared.item.meta_path, meta)
        return status


def record_attempt(prepared: PreparedSource, attempt: dict) -> str:
    with SUBMIT_LOCK:
        try:
            meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
        except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
            raise SubmissionError(f"无法读取元数据：{prepared.item.meta_path}") from error
        if not isinstance(meta, dict):
            raise SubmissionError(f"元数据根节点不是对象：{prepared.item.meta_path}")
        attempts = meta.setdefault("submission_attempts", [])
        if not isinstance(attempts, list):
            raise SubmissionError(f"submission_attempts 不是数组：{prepared.item.meta_path}")

        submission_id = attempt.get("submission_id")
        for index, old_attempt in enumerate(attempts):
            same_submission = (
                submission_id is not None
                and isinstance(old_attempt, dict)
                and old_attempt.get("submission_id") == submission_id
            )
            same_post_error = (
                submission_id is None
                and isinstance(old_attempt, dict)
                and old_attempt.get("submission_id") is None
                and old_attempt.get("recorded_at") == attempt.get("recorded_at")
            )
            if same_submission or same_post_error:
                attempts[index] = attempt
                break
        else:
            attempts.append(attempt)

        source_sha256, fingerprint_error = current_fingerprint(prepared)
        meta["validation"] = refresh_validation(
            meta,
            source_sha256,
            fingerprint_error,
        )
        atomic_write_json(prepared.item.meta_path, meta)
        return meta["validation"]["status"]


def latest_submission_id(prepared: PreparedSource) -> int | None:
    """Return the current source's confirmed pending id, if one exists."""
    try:
        meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
        raise SubmissionError(f"无法读取元数据：{prepared.item.meta_path}") from error
    if not isinstance(meta, dict):
        raise SubmissionError(f"元数据根节点不是对象：{prepared.item.meta_path}")
    validation = meta.get("validation")
    if not isinstance(validation, dict):
        return None
    submission_id = validation.get("current_source_submission_id")
    if type(submission_id) is int and submission_id >= 0:
        return submission_id
    return None


def block_automatic_resubmission(
    prepared: PreparedSource,
    status: str,
    allow_retry_unknown: bool = False,
) -> None:
    """Reject a new POST while this exact source has an unresolved attempt."""
    if status == "submitted":
        submission_id = latest_submission_id(prepared)
        if submission_id is None:
            raise SubmissionError(
                f"{prepared.item.name}: 当前源码处于 submitted 但缺少提交编号，"
                "拒绝自动重交"
            )
        raise SubmissionError(
            f"{prepared.item.name}: 当前源码已有待查询的 submission {submission_id}，"
            "必须继续查询该编号，拒绝自动重交"
        )
    if status == "submission_unknown":
        if allow_retry_unknown:
            return
        raise SubmissionError(
            f"{prepared.item.name}: 当前源码的 POST 结果未知，"
            "必须先定位唯一服务端提交或取得用户明确重交许可，拒绝自动重交"
        )
    if status in {"online_failed", "online_stale", "submission_error"}:
        raise SubmissionError(
            f"{prepared.item.name}: 当前源码已有在线状态 {status}，"
            "必须先按受控流程处理或改变源码并重新验证，拒绝自动重交"
        )


def judge_one(
    prepared: PreparedSource,
    token: str,
    timeout: float,
    interval: float,
) -> bool:
    check_current(prepared)
    try:
        if prepared.item.language == LANG:
            submission_id = submit(API, token, prepared.item.name, prepared.source)
        else:
            submission_id = submit(
                API,
                token,
                prepared.item.name,
                prepared.source,
                prepared.item.language,
            )
    except Exception as error:
        submitted_at = utc_now()
        attempt = {
            "submission_id": None,
            "source_sha256": prepared.source_sha256,
            "server_source_sha256": None,
            "lang": prepared.item.language,
            "verdict": None,
            "last_status": "SUBMISSION_UNKNOWN",
            "time": None,
            "memory": None,
            "submission_time": None,
            "is_latest": None,
            "compile_standard": (
                "gnu++20" if prepared.item.language == "cpp20" else None
            ),
            "submitted_at": submitted_at,
            "recorded_at": submitted_at,
            "submission_unknown": True,
            "error": str(error),
        }
        record_attempt(prepared, attempt)
        raise
    submitted_at = utc_now()
    print(f"SUBMITTED {prepared.item.name}: submission {submission_id}")
    attempt = {
        "submission_id": submission_id,
        "source_sha256": prepared.source_sha256,
        "server_source_sha256": None,
        "lang": prepared.item.language,
        "verdict": None,
        "last_status": "SUBMITTED",
        "time": None,
        "memory": None,
        "submission_time": None,
        "is_latest": None,
        "compile_standard": (
            "gnu++20" if prepared.item.language == "cpp20" else None
        ),
        "submitted_at": submitted_at,
        "recorded_at": submitted_at,
    }
    record_attempt(prepared, attempt)

    try:
        data = wait_submission(API, prepared, submission_id, timeout, interval)
    except Exception as error:
        # The server already gave us an id.  Preserve submitted and the id;
        # a later run can poll this exact submission without re-posting.
        attempt["last_status"] = "SUBMITTED"
        attempt["error"] = str(error)
        attempt["recorded_at"] = utc_now()
        record_attempt(prepared, attempt)
        raise

    overview, server_sha256 = validate_submission(data, prepared, submission_id)
    verdict = overview["status"]
    attempt.update({
        "server_source_sha256": server_sha256,
        "verdict": verdict,
        "last_status": verdict,
        "time": overview.get("time"),
        "memory": overview.get("memory"),
        "submission_time": overview.get("submission_time"),
        "is_latest": overview.get("is_latest"),
        "recorded_at": utc_now(),
    })
    validation_status = record_attempt(prepared, attempt)
    print(
        f"VERDICT {prepared.item.name}: {verdict}, "
        f"time {overview.get('time')} s, memory {overview.get('memory')} B, "
        f"validation {validation_status}"
    )
    return validation_status == "online_ac"


def poll_pending_one(
    prepared: PreparedSource,
    timeout: float,
    interval: float,
) -> bool:
    # prepared 是已有确认提交编号的当前源码；只查询该编号，绝不再次 POST。
    submission_id = latest_submission_id(prepared)
    if submission_id is None:
        raise SubmissionError(f"{prepared.item.name}: submitted 状态缺少提交编号")
    try:
        meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
        raise SubmissionError(f"无法读取元数据：{prepared.item.meta_path}") from error
    attempts = meta.get("submission_attempts") if isinstance(meta, dict) else None
    old = next(
        (
            item
            for item in reversed(attempts if isinstance(attempts, list) else [])
            if isinstance(item, dict) and item.get("submission_id") == submission_id
        ),
        None,
    )
    if old is None:
        raise SubmissionError(f"{prepared.item.name}: 找不到 submission {submission_id} 的尝试记录")
    data = wait_submission(API, prepared, submission_id, timeout, interval)
    overview, server_sha256 = validate_submission(data, prepared, submission_id)
    attempt = dict(old)
    verdict = overview["status"]
    attempt.update({
        "server_source_sha256": server_sha256,
        "verdict": verdict,
        "last_status": verdict,
        "time": overview.get("time"),
        "memory": overview.get("memory"),
        "submission_time": overview.get("submission_time"),
        "is_latest": overview.get("is_latest"),
        "recorded_at": utc_now(),
    })
    validation_status = record_attempt(prepared, attempt)
    print(
        f"VERDICT {prepared.item.name}: {verdict}, "
        f"time {overview.get('time')} s, memory {overview.get('memory')} B, "
        f"validation {validation_status}"
    )
    return validation_status == "online_ac"


def parse_wanted(value: str | None) -> set[str] | None:
    if value is None:
        return None
    wanted = {name.strip() for name in value.split(",") if name.strip()}
    if not wanted:
        raise SubmissionError("--only 没有包含有效题目名")
    return wanted


def already_certified(prepared: PreparedSource) -> bool:
    check_current(prepared)
    try:
        meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
        raise SubmissionError(f"无法读取元数据：{prepared.item.meta_path}") from error
    if not isinstance(meta, dict):
        raise SubmissionError(f"元数据根节点不是对象：{prepared.item.meta_path}")
    validation = meta.get("validation")
    attempts = meta.get("submission_attempts")
    summary_matches = (
        isinstance(validation, dict)
        and validation.get("status") == "online_ac"
        and validation.get("current_source_sha256") == prepared.source_sha256
        and validation.get("server_source_sha256") == prepared.source_sha256
        and validation.get("current_source_online_verdict") == "AC"
        and validation.get("current_source_is_latest") is True
        and type(validation.get("current_source_submission_id")) is int
        and "current_source_time" in validation
        and "current_source_memory" in validation
        and "current_source_submission_time" in validation
    )
    if not summary_matches or not isinstance(attempts, list):
        return False
    submission_id = validation["current_source_submission_id"]
    return any(
        isinstance(attempt, dict)
        and attempt.get("submission_id") == submission_id
        and attempt.get("time") == validation.get("current_source_time")
        and attempt.get("memory") == validation.get("current_source_memory")
        and attempt.get("submission_time")
        == validation.get("current_source_submission_time")
        and attempt_is_ac(
            attempt,
            prepared.source_sha256,
        )
        for attempt in attempts
    )


def check_receipt(
    root: Path,
    prepared: list[PreparedSource],
    manifest: dict,
    path: Path | None = None,
) -> None:
    # root 是题解目录；prepared 是待提交独立源码；manifest 是当前清单；path 是可选收据路径。
    # 返回值为空；收据缺失、过期或不完整时抛出 SubmissionError。
    try:
        from verify_local import LocalVerifyError, require_receipt

        require_receipt(root, prepared, manifest, path)
    except ImportError as error:
        raise SubmissionError("无法加载本地预检收据校验器 verify_local.py") from error
    except LocalVerifyError as error:
        raise SubmissionError(str(error)) from error


def main() -> None:
    parser = argparse.ArgumentParser(description="提交 Library Checker 题解")
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--submit", action="store_true", help="实际发送并等待评测")
    mode.add_argument("--dry-run", action="store_true", help="只列出并检查独立源码")
    parser.add_argument(
        "--allow-retry-unknown",
        action="store_true",
        help="仅在用户已明确允许时，对唯一 submission_unknown 题目重交一次",
    )
    parser.add_argument("--only", help="逗号分隔的题目名")
    parser.add_argument(
        "--batch",
        type=Path,
        help="第 03 章单题批次 JSON；data_structure 在线提交时必填且必须位于 .local/",
    )
    parser.add_argument(
        "--all",
        action="store_true",
        help="选择清单中的全部当前模板题解",
    )
    parser.add_argument(
        "--jobs",
        type=int,
        default=1,
        help="提交并发数；受控流程固定为 1",
    )
    parser.add_argument("--compiler", default="g++", help="本地 gnu++20 编译器")
    parser.add_argument(
        "--receipt",
        type=Path,
        help=(
            "本地预检收据路径，默认本目录 .local/yosupo-local-verification.json；"
            "相对路径必须位于本目录 .local 下"
        ),
    )
    parser.add_argument(
        "--compile-timeout",
        type=float,
        default=180,
        help="每题本地编译超时秒数，默认 180",
    )
    parser.add_argument(
        "--judge-timeout",
        type=float,
        default=1800,
        help="每题在线评测超时秒数，默认 1800",
    )
    parser.add_argument(
        "--poll-interval",
        type=float,
        default=3,
        help="轮询间隔秒数，默认 3",
    )
    args = parser.parse_args()
    if args.allow_retry_unknown and not args.submit:
        parser.error("--allow-retry-unknown 只能与 --submit 同时使用")
    if args.jobs != 1:
        parser.error("受控提交流程要求 --jobs 1")
    if args.compile_timeout <= 0 or args.judge_timeout <= 0:
        parser.error("超时必须为正数")
    if args.poll_interval < 0:
        parser.error("--poll-interval 不能为负数")
    if args.only and args.all:
        parser.error("--only 和 --all 不能同时使用")
    if args.submit and not args.only:
        parser.error("提交时必须用 --only 指定唯一题目")
    if args.submit and args.all:
        parser.error("受控提交流程禁止使用 --all 提交")
    root = Path(__file__).resolve().parent.parent
    try:
        manifest = json.loads((root / "manifest.json").read_text(encoding="utf-8"))
        wanted = parse_wanted(args.only)
        if args.submit and (wanted is None or len(wanted) != 1):
            raise SubmissionError("受控提交流程一次只能提交一道题")
        items = load_items(
            root,
            manifest,
            wanted,
            solution_only=True,
        )
    except (OSError, UnicodeDecodeError, json.JSONDecodeError, SubmissionError) as error:
        sys.exit(str(error))

    print(f"共 {len(items)} 题")
    if args.dry_run:
        for item in items:
            print(f"{item.name}: {item.path.relative_to(root).as_posix()}")
        try:
            prepared = prepare_sources(items)
            for source in prepared:
                size = len(source.source.encode("utf-8"))
                print(
                    f"{source.item.name}: 独立源码有效，{size} 字节，"
                    f"SHA256 {source.source_sha256}"
                )
        except (SubmissionError, ValueError) as error:
            sys.exit(str(error))
        return

    try:
        prepared = prepare_sources(items)
        if args.allow_retry_unknown and len(prepared) != 1:
            raise SubmissionError("--allow-retry-unknown 只能用于唯一一道题")
        resumed = []
        for source in prepared:
            status = reconcile_validation(source)
            if status == "submitted" and latest_submission_id(source) is not None:
                resumed.append(source)
                continue
            block_automatic_resubmission(
                source,
                status,
                args.allow_retry_unknown,
            )
        if resumed:
            if len(resumed) != 1 or len(prepared) != 1:
                raise SubmissionError("受控提交流程一次只能恢复查询一道题")
            source = resumed[0]
            print(f"继续查询已有 submission {latest_submission_id(source)}，不发送新提交")
            certified = poll_pending_one(source, args.judge_timeout, args.poll_interval)
            if not certified:
                raise SubmissionError(f"{source.item.name}: 没有取得当前源码可认证的 AC")
            return
        pending = []
        for source in prepared:
            if already_certified(source):
                print(f"SKIPPED {source.item.name}: 当前源码已有可认证 AC")
            else:
                pending.append(source)
        prepared = pending
        if not prepared:
            print("全部当前源码已有可认证 AC")
            return
        for source in prepared:
            validate_ch03_batch(root, source, args.batch)
        check_receipt(root, prepared, manifest, args.receipt)
        print(f"本地预检收据有效：{len(prepared)} 题")
        compile_sources(prepared, args.compiler, args.compile_timeout)
        check_cpp20(API)
        profile = root / ".local" / "yosupo-profile"
        profile.mkdir(parents=True, exist_ok=True)
        token = browser_auth(profile)
        for source in prepared:
            validate_ch03_batch(root, source, args.batch)
        check_receipt(root, prepared, manifest, args.receipt)
    except (SubmissionError, HTTPError, URLError, TimeoutError) as error:
        sys.exit(str(error))

    if len(prepared) != 1:
        sys.exit("受控提交流程一次只能保留一道待提交源码")
    source = prepared[0]
    try:
        validate_ch03_batch(root, source, args.batch)
        if source.item.category == "data_structure" and args.receipt is not None:
            raise SubmissionError("第 03 章在线提交必须使用默认 .local/yosupo-local-verification.json 收据")
        check_receipt(root, [source], manifest, args.receipt)
    except SubmissionError as error:
        sys.exit(str(error))
    print(f"使用语言 {LANG}，单题串行提交")
    try:
        certified = judge_one(
            source,
            token,
            args.judge_timeout,
            args.poll_interval,
        )
    except Exception as error:
        sys.exit(f"FAILED {source.item.name}: {error}")
    if not certified:
        sys.exit(f"{source.item.name}: 没有取得当前源码可认证的 AC")


if __name__ == "__main__":
    main()
