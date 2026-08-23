#!/usr/bin/env python3
import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tarfile
import tempfile
import time
import tomllib
from contextlib import ExitStack
from dataclasses import dataclass
from pathlib import Path, PurePosixPath

import submit_yosupo as yosupo


RECEIPT_SCHEMA = 3
RECEIPT_KIND = "yosupo_template_local_verification"
RECEIPT_NAME = "yosupo-local-verification.json"
CASE_KINDS = ("sample", "boundary", "random")
RANDOM_SEED = 0
COMPILE_FLAGS = ("-std=gnu++20", "-O2", "-pipe")
COMPILE_MARKER = "source_cxx20_compile"
NAME_RE = re.compile(r"[A-Za-z0-9_]+")
SHA256_RE = re.compile(r"[0-9a-f]{64}")


class LocalVerifyError(RuntimeError):
    pass


@dataclass(frozen=True)
class CaseSpec:
    kind: str
    source: str
    index: int = 0
    input_path: Path | None = None

    @property
    def name(self) -> str:
        return f"{Path(self.source).stem}_{self.index:02d}"

    @property
    def seed(self) -> int | None:
        if self.kind == "random":
            return self.index
        return None


@dataclass(frozen=True)
class RunResult:
    stdout: bytes
    seconds: float


def default_receipt_path(root: Path) -> Path:
    return root.resolve() / ".local" / RECEIPT_NAME


def default_problems_path(root: Path) -> Path:
    return root.resolve() / ".local" / "library-checker-problems"


def local_arg_path(root: Path, path: Path, label: str) -> Path:
    root = root.resolve()
    if path.is_absolute():
        return path.resolve()
    target = (root / path).resolve()
    local = (root / ".local").resolve()
    try:
        target.relative_to(local)
    except ValueError as error:
        raise LocalVerifyError(
            f"{label} 的相对路径必须位于本目录 .local 下：{path}"
        ) from error
    return target


def manifest_sha256(manifest: dict) -> str:
    text = json.dumps(
        manifest,
        ensure_ascii=False,
        sort_keys=True,
        separators=(",", ":"),
    )
    return yosupo.sha256_text(text)


def official_commit(manifest: dict) -> str:
    repository = manifest.get("official_repository")
    commit = repository.get("commit") if isinstance(repository, dict) else None
    if not isinstance(commit, str) or re.fullmatch(r"[0-9a-f]{40}", commit) is None:
        raise LocalVerifyError("manifest 的 official_repository.commit 无效")
    return commit


def _decode(data: bytes | str | None) -> str:
    if data is None:
        return ""
    if isinstance(data, str):
        return data
    return data.decode("utf-8", errors="replace")


def run_cmd(
    command: list[str],
    label: str,
    timeout: float,
    cwd: Path | None = None,
    stdin_path: Path | None = None,
    stdout_path: Path | None = None,
) -> RunResult:
    started = time.monotonic()
    try:
        with ExitStack() as stack:
            stdin = (
                stack.enter_context(stdin_path.open("rb"))
                if stdin_path is not None
                else None
            )
            stdout = (
                stack.enter_context(stdout_path.open("wb"))
                if stdout_path is not None
                else subprocess.PIPE
            )
            result = subprocess.run(
                command,
                cwd=str(cwd) if cwd is not None else None,
                stdin=stdin,
                stdout=stdout,
                stderr=subprocess.PIPE,
                timeout=timeout,
                check=False,
            )
    except FileNotFoundError as error:
        raise LocalVerifyError(f"{label}: 找不到程序 {command[0]}") from error
    except subprocess.TimeoutExpired as error:
        raise LocalVerifyError(f"{label}: 超过 {timeout:g} 秒") from error
    seconds = time.monotonic() - started
    if result.returncode != 0:
        message = "\n".join(filter(None, [
            _decode(result.stdout).strip(),
            _decode(result.stderr).strip(),
        ]))
        if len(message) > 8000:
            message = message[-8000:]
        suffix = f"\n{message}" if message else ""
        raise LocalVerifyError(
            f"{label}: 退出码 {result.returncode}{suffix}"
        )
    stdout_data = result.stdout if isinstance(result.stdout, bytes) else b""
    return RunResult(stdout_data, seconds)


def _compiler_args(args: tuple[str, ...]) -> tuple[str, ...]:
    if any("NDEBUG" in arg for arg in args):
        raise LocalVerifyError("本地预检禁止通过编译参数定义 NDEBUG")
    return args


def compile_cpp(
    source: Path,
    output: Path,
    compiler: str,
    timeout: float,
    compiler_args: tuple[str, ...] = (),
    include_dirs: tuple[Path, ...] = (),
    online_judge: bool = False,
    linker_args: tuple[str, ...] = (),
    label: str | None = None,
) -> float:
    if not source.is_file():
        raise LocalVerifyError(f"缺少待编译源码：{source}")
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [compiler, *_compiler_args(compiler_args), *COMPILE_FLAGS]
    if online_judge:
        command.append("-DONLINE_JUDGE")
    for directory in include_dirs:
        command.extend(["-I", str(directory)])
    command.extend([
        str(source),
        "-o",
        str(output),
        *yosupo.local_linker_args(),
        *linker_args,
    ])
    result = run_cmd(
        command,
        label or f"编译 {source.name}",
        timeout,
        cwd=source.parent,
    )
    return result.seconds


def read_manifest(root: Path) -> dict:
    path = root / "manifest.json"
    try:
        manifest = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
        raise LocalVerifyError(f"无法读取 manifest：{path}") from error
    if not isinstance(manifest, dict):
        raise LocalVerifyError("manifest 根节点不是对象")
    return manifest


def load_sources(
    root: Path,
    manifest: dict,
    wanted: set[str] | None,
) -> list[yosupo.PreparedSource]:
    try:
        items = yosupo.load_items(
            root,
            manifest,
            wanted,
            solution_only=True,
        )
        prepared = yosupo.prepare_sources(items)
    except yosupo.SubmissionError as error:
        raise LocalVerifyError(str(error)) from error
    if wanted is None:
        count = manifest.get("problem_count")
        if type(count) is not int or count != len(prepared):
            raise LocalVerifyError(
                f"manifest problem_count 与严格加载结果不符：{count!r} != {len(prepared)}"
            )
    if not prepared:
        raise LocalVerifyError("没有选中当前独立源码")
    return prepared


def _read_meta(path: Path) -> dict:
    try:
        meta = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
        raise LocalVerifyError(f"无法读取元数据：{path}") from error
    if not isinstance(meta, dict):
        raise LocalVerifyError(f"元数据根节点不是对象：{path}")
    return meta


def collect_checks(
    root: Path,
    prepared: list[yosupo.PreparedSource],
) -> tuple[dict[str, list[str]], dict[str, Path]]:
    workspace = root.resolve().parent
    by_problem = {}
    unique = {}
    for source in prepared:
        meta = _read_meta(source.item.meta_path)
        validation = meta.get("validation")
        checks = validation.get("checks") if isinstance(validation, dict) else None
        if not isinstance(checks, list) or not all(
            isinstance(check, str) and check for check in checks
        ):
            raise LocalVerifyError(f"{source.item.name}: validation.checks 无效")
        if COMPILE_MARKER not in checks:
            raise LocalVerifyError(
                f"{source.item.name}: validation.checks 缺少 {COMPILE_MARKER}"
            )
        paths = []
        for check in checks:
            if check == COMPILE_MARKER:
                continue
            path = (workspace / check).resolve()
            try:
                relative = path.relative_to(workspace).as_posix()
            except ValueError as error:
                raise LocalVerifyError(
                    f"{source.item.name}: 模板测试越出工作区：{check}"
                ) from error
            if path.suffix != ".cpp" or not path.is_file():
                raise LocalVerifyError(
                    f"{source.item.name}: 模板测试不存在或不是 C++ 源码：{check}"
                )
            paths.append(relative)
            unique[relative] = path
        by_problem[source.item.name] = paths
    return by_problem, unique


def _git_text(root: Path, args: list[str], timeout: float) -> str:
    result = run_cmd(
        ["git", "-C", str(root), *args],
        "检查 Library Checker problems 仓库",
        timeout,
    )
    return result.stdout.decode("utf-8", errors="replace").strip()


def validate_official_root(
    problems_root: Path,
    commit: str,
    timeout: float = 30.0,
) -> Path:
    root = problems_root.resolve()
    if not root.is_dir():
        raise LocalVerifyError(f"Library Checker problems 目录不存在：{root}")
    top = _git_text(root, ["rev-parse", "--show-toplevel"], timeout)
    try:
        top_path = Path(top).resolve()
    except OSError as error:
        raise LocalVerifyError(f"无法解析 Git 工作树根目录：{top}") from error
    if top_path != root:
        raise LocalVerifyError(
            f"--problems-root 必须指向 Git 工作树根目录：{top_path}"
        )
    head = _git_text(root, ["rev-parse", "HEAD"], timeout).lower()
    if head != commit:
        raise LocalVerifyError(
            f"Library Checker problems 提交不匹配：期望 {commit}，实际 {head}"
        )
    return root


def official_problem_rel(entry: dict) -> str:
    name = entry.get("name")
    category = entry.get("category")
    if not isinstance(name, str) or NAME_RE.fullmatch(name) is None:
        raise LocalVerifyError("manifest 包含无效官方题目名")
    if not isinstance(category, str) or NAME_RE.fullmatch(category) is None:
        raise LocalVerifyError(f"{name}: manifest category 无效")
    return f"{category}/{name}"


def stage_problem(
    problems_root: Path,
    commit: str,
    relative: str,
    destination: Path,
    timeout: float,
) -> Path:
    pure = PurePosixPath(relative)
    if pure.is_absolute() or ".." in pure.parts or len(pure.parts) != 2:
        raise LocalVerifyError(f"官方题目路径无效：{relative}")
    destination.mkdir(parents=True, exist_ok=True)
    archive = destination / "snapshot.tar"
    try:
        run_cmd(
            [
                "git",
                "-C",
                str(problems_root),
                "archive",
                "--format=tar",
                commit,
                "--",
                "common",
                relative,
            ],
            f"复制官方题目 {relative}",
            timeout,
            stdout_path=archive,
        )
        with tarfile.open(archive, "r") as tar:
            for member in tar.getmembers():
                path = PurePosixPath(member.name)
                if path.is_absolute() or ".." in path.parts:
                    raise LocalVerifyError(
                        f"官方归档包含越界路径：{member.name}"
                    )
            tar.extractall(destination, filter="data")
    except (tarfile.TarError, OSError) as error:
        raise LocalVerifyError(f"无法展开官方题目 {relative}") from error
    finally:
        archive.unlink(missing_ok=True)
    problem = destination.joinpath(*pure.parts)
    if not (problem / "info.toml").is_file():
        raise LocalVerifyError(f"官方提交中缺少题目：{relative}")
    return problem


def read_info(path: Path) -> dict:
    try:
        info = tomllib.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, tomllib.TOMLDecodeError) as error:
        raise LocalVerifyError(f"无法读取官方 info.toml：{path}") from error
    if not isinstance(info, dict):
        raise LocalVerifyError(f"官方 info.toml 根节点无效：{path}")
    return info


def validate_official_title(entry: dict, info: dict) -> None:
    name = entry.get("name")
    expected = entry.get("title")
    actual = info.get("title")
    if not isinstance(name, str) or not name:
        raise LocalVerifyError("manifest 缺少题目名")
    if not isinstance(expected, str) or not expected:
        raise LocalVerifyError(f"{name}: manifest title 无效")
    if not isinstance(actual, str) or not actual:
        raise LocalVerifyError(f"{name}: 官方 info.toml title 无效")
    if expected != actual:
        raise LocalVerifyError(
            f"{name}: manifest title 与官方题目标题不一致："
            f"{expected!r} != {actual!r}"
        )


def write_params(problem: Path, info: dict) -> None:
    params = info.get("params", {})
    if not isinstance(params, dict):
        raise LocalVerifyError(f"{problem.name}: info.toml params 无效")
    lines = []
    for key, value in params.items():
        if not isinstance(key, str) or re.fullmatch(
            r"[A-Za-z_][A-Za-z0-9_]*",
            key,
        ) is None:
            raise LocalVerifyError(f"{problem.name}: 参数名无效：{key!r}")
        if type(value) is int:
            encoded = f"(long long){value}"
        elif type(value) is float:
            encoded = repr(value)
        elif isinstance(value, str):
            encoded = json.dumps(value, ensure_ascii=False)
        else:
            raise LocalVerifyError(
                f"{problem.name}: 参数 {key} 的类型不受支持"
            )
        lines.append(f"#define {key} {encoded}\n")
    (problem / "params.h").write_text("".join(lines), encoding="utf-8")


def _tests(config: list[dict]) -> list[tuple[str, int]]:
    result = []
    for entry in config:
        if not isinstance(entry, dict):
            raise LocalVerifyError("info.toml tests 包含非对象条目")
        name = entry.get("name")
        number = entry.get("number")
        if not isinstance(name, str):
            raise LocalVerifyError("info.toml tests 包含无效 name")
        path = PurePosixPath(name)
        if (
            path.is_absolute()
            or ".." in path.parts
            or len(path.parts) != 1
            or path.suffix not in {".cpp", ".in"}
        ):
            raise LocalVerifyError(f"info.toml 测试源路径无效：{name}")
        if type(number) is not int or number <= 0:
            raise LocalVerifyError(f"info.toml 测试数量无效：{name}")
        result.append((name, number))
    return result


def _random_rank(name: str) -> tuple[int, str]:
    stem = Path(name).stem.lower()
    if stem == "random":
        return (0, stem)
    if "max" in stem:
        return (4, stem)
    if stem.startswith("random_") or stem.endswith("_random"):
        return (1, stem)
    if "random" in stem and "max" not in stem:
        return (2, stem)
    return (3, stem)


def _boundary_rank(name: str) -> tuple[int, str] | None:
    stem = Path(name).stem.lower()
    low = (
        "zero",
        "identity",
        "constant",
        "deg0",
        "n_1",
        "n_01",
        "nm_1",
        "nm_01",
        "eq_0",
        "r01",
        "y0",
        "nar_0",
        "first_zero",
        "lower_deg",
        "monomial",
        "hand",
    )
    penalty = 5 if "max" in stem else 0
    if any(token in stem for token in low):
        return (penalty, stem)
    if "small" in stem or "minimum" in stem or re.search(r"(^|_)min(_|$)", stem):
        return (1 + penalty, stem)
    if "random" in stem and "abnormal" not in stem:
        return None
    structural = (
        "abnormal",
        "hack",
        "killer",
        "overflow",
        "issue",
        "trailing",
        "unbalanced",
        "irreducible",
        "same_degree",
        "square_free",
        "all_linear",
        "all_same",
        "all_distinct",
    )
    if any(token in stem for token in structural):
        return (2, stem)
    if "near" in stem:
        return (3, stem)
    if "max" in stem:
        return (4, stem)
    return None


def select_cases(
    config: list[dict],
    boundary_input: Path | None = None,
    boundary_source: str | None = None,
) -> list[CaseSpec]:
    tests = _tests(config)
    sample = [
        name for name, _ in tests
        if Path(name).suffix == ".in"
        and Path(name).stem.lower() in {"example", "sample"}
    ]
    if not sample:
        raise LocalVerifyError("info.toml 没有固定 example/sample 输入")

    randoms = [
        name for name, _ in tests
        if Path(name).suffix == ".cpp" and "random" in Path(name).stem.lower()
    ]
    if randoms:
        random_name = min(randoms, key=_random_rank)
    else:
        generators = [name for name, _ in tests if Path(name).suffix == ".cpp"]
        if not generators:
            fixed = [
                name for name, _ in tests
                if Path(name).suffix == ".in" and name not in sample
            ]
            if not fixed:
                raise LocalVerifyError("info.toml 没有 C++ generator 或非样例固定输入")
            random_name = sorted(fixed)[len(fixed) // 2]
        else:
            # 部分官方题把随机生成器命名为 max.cpp、medium.cpp 等；优先最大规模，
            # 再按原有稳定排序选取，不能把命名习惯当作缺少随机输入。
            random_name = min(
                generators,
                key=lambda name: ("max" not in Path(name).stem.lower(), _random_rank(name)),
            )

    boundaries = []
    for order, (name, _) in enumerate(tests):
        if name in {sample[0], random_name}:
            continue
        rank = _boundary_rank(name)
        if rank is not None:
            boundaries.append((rank[0], order, rank[1], name))
    if not boundaries:
        # 官方快照中也有 hand/path/ans_big 这类未统一命名的固定用例；
        # 在没有语义名称时，优先选择固定输入，再选择非随机生成器。
        fallback = []
        for order, (name, _) in enumerate(tests):
            if name in {sample[0], random_name}:
                continue
            stem = Path(name).stem.lower()
            if "random" in stem:
                continue
            suffix_rank = 0 if Path(name).suffix == ".in" else 1
            fallback.append((suffix_rank, order, stem, name))
        boundaries = [(rank, order, stem, name) for rank, order, stem, name in fallback]
    if not boundaries:
        # 极少数题只有 example、普通随机和最大规模随机。保留不同的普通随机作为
        # random 后，最大规模生成器仍是可复现的边界压力输入。
        generator_fallback = []
        for order, (name, _) in enumerate(tests):
            if name in {sample[0], random_name} or Path(name).suffix != ".cpp":
                continue
            stem = Path(name).stem.lower()
            generator_fallback.append(("max" not in stem, order, stem, name))
        boundaries = [
            (rank, order, stem, name)
            for rank, order, stem, name in generator_fallback
        ]
    if boundary_input is not None and boundary_source is not None:
        boundary = CaseSpec("boundary", boundary_source, 0, boundary_input)
    elif not boundaries:
        raise LocalVerifyError("info.toml 没有可明确识别的边界测试")
    else:
        boundary = CaseSpec("boundary", min(boundaries)[3], 0)
    return [
        CaseSpec("sample", sample[0], 0),
        boundary,
        CaseSpec("random", random_name, RANDOM_SEED),
    ]


def custom_boundary_input(root: Path, source: yosupo.PreparedSource) -> tuple[Path, str] | None:
    """Return an explicit, hash-checked local boundary fixture when needed."""
    meta = _read_meta(source.item.meta_path)
    validation = meta.get("validation")
    value = validation.get("boundary_input") if isinstance(validation, dict) else None
    if value is None:
        return None
    if not isinstance(value, str) or not value:
        raise LocalVerifyError(f"{source.item.name}: validation.boundary_input 无效")
    workspace = root.resolve().parent
    path = Path(value)
    if path.is_absolute() or path.suffix != ".in":
        raise LocalVerifyError(f"{source.item.name}: boundary_input 必须是工作区内 .in 文件")
    target = (workspace / path).resolve()
    try:
        relative = target.relative_to(workspace).as_posix()
    except ValueError as error:
        raise LocalVerifyError(f"{source.item.name}: boundary_input 路径越出工作区") from error
    if not target.is_file():
        raise LocalVerifyError(f"{source.item.name}: 缺少 boundary_input：{relative}")
    return target, relative


def _inside(base: Path, relative: str, label: str) -> Path:
    path = Path(relative)
    if path.is_absolute():
        raise LocalVerifyError(f"{label} 路径不能是绝对路径：{relative}")
    target = (base / path).resolve()
    try:
        target.relative_to(base.resolve())
    except ValueError as error:
        raise LocalVerifyError(f"{label} 路径越界：{relative}") from error
    if not target.is_file():
        raise LocalVerifyError(f"缺少 {label}：{target}")
    return target


def _exe(path: Path) -> Path:
    return path.with_suffix(".exe") if os.name == "nt" else path


def _case_input(
    spec: CaseSpec,
    problem: Path,
    generator: Path | None,
    target: Path,
    timeout: float,
) -> float:
    if spec.input_path is not None:
        started = time.monotonic()
        shutil.copyfile(spec.input_path, target)
        return time.monotonic() - started
    if Path(spec.source).suffix == ".in":
        source = problem / "gen" / f"{Path(spec.source).stem}_{spec.index:02d}.in"
        if not source.is_file():
            raise LocalVerifyError(f"缺少官方固定输入：{source}")
        started = time.monotonic()
        shutil.copyfile(source, target)
        return time.monotonic() - started
    if generator is None:
        raise LocalVerifyError(f"缺少官方 generator：{spec.source}")
    return run_cmd(
        [str(generator), str(spec.index)],
        f"生成 {problem.name}/{spec.name}",
        timeout,
        cwd=problem,
        stdout_path=target,
    ).seconds


def run_official_cases(
    root: Path,
    source: yosupo.PreparedSource,
    solution: Path,
    entry: dict,
    problems_root: Path,
    commit: str,
    work: Path,
    compiler: str,
    compiler_args: tuple[str, ...],
    compile_timeout: float,
    run_timeout: float,
    solution_command: list[str] | None = None,
) -> list[dict]:
    relative = official_problem_rel(entry)
    problem = stage_problem(
        problems_root,
        commit,
        relative,
        work,
        compile_timeout,
    )
    info = read_info(problem / "info.toml")
    validate_official_title(entry, info)
    config = info.get("tests")
    if not isinstance(config, list):
        raise LocalVerifyError(f"{source.item.name}: info.toml 缺少 tests")
    custom_boundary = custom_boundary_input(root, source)
    cases = select_cases(
        config,
        None if custom_boundary is None else custom_boundary[0],
        None if custom_boundary is None else custom_boundary[1],
    )
    write_params(problem, info)

    common = work / "common"
    correct_src = _inside(problem, "sol/correct.cpp", "官方 correct.cpp")
    if relative in {
        "enumerative_combinatorics/stirling_number_of_the_first_kind_fixed_k",
        "enumerative_combinatorics/stirling_number_of_the_second_kind_fixed_k",
    }:
        # 冻结快照早于 C++20 concepts；临时副本中的 integral 与
        # std::integral 在 GCC 14 下产生歧义，限定到全局函数即可保持原算法语义。
        text = correct_src.read_text(encoding="utf-8")
        correct_src.write_text(
            text.replace("=integral(", "=::integral(").replace(
                "res = integral(", "res = ::integral("
            ),
            encoding="utf-8",
            newline="\n",
        )
    checker_name = info.get("checker", "checker.cpp")
    if not isinstance(checker_name, str):
        raise LocalVerifyError(f"{source.item.name}: checker 配置无效")
    checker_src = _inside(problem, checker_name, "官方 checker")
    if correct_src.suffix != ".cpp" or checker_src.suffix != ".cpp":
        raise LocalVerifyError(f"{source.item.name}: 官方 correct/checker 必须是 C++ 源码")

    bin_dir = work / "bin"
    correct = _exe(bin_dir / "correct")
    checker = _exe(bin_dir / "checker")
    compile_cpp(
        correct_src,
        correct,
        compiler,
        compile_timeout,
        compiler_args,
        (common,),
        label=f"{source.item.name}: 编译官方 correct.cpp",
    )
    compile_cpp(
        checker_src,
        checker,
        compiler,
        compile_timeout,
        compiler_args,
        (common,),
        label=f"{source.item.name}: 编译官方 checker",
    )

    generators = {}
    for spec in cases:
        if Path(spec.source).suffix != ".cpp" or spec.source in generators:
            continue
        generator_src = _inside(problem / "gen", spec.source, "官方 generator")
        generator = _exe(bin_dir / f"gen-{Path(spec.source).stem}")
        compile_cpp(
            generator_src,
            generator,
            compiler,
            compile_timeout,
            compiler_args,
            (common,),
            label=f"{source.item.name}: 编译 generator {spec.source}",
        )
        generators[spec.source] = generator

    records = []
    for spec in cases:
        case_dir = work / "cases" / spec.kind
        case_dir.mkdir(parents=True, exist_ok=True)
        input_path = case_dir / "input.txt"
        expected_path = case_dir / "expected.txt"
        actual_path = case_dir / "actual.txt"
        started = time.monotonic()
        generation_seconds = _case_input(
            spec,
            problem,
            generators.get(spec.source),
            input_path,
            run_timeout,
        )
        correct_seconds = run_cmd(
            [str(correct)],
            f"{source.item.name}/{spec.name}: 运行官方 correct.cpp",
            run_timeout,
            cwd=problem,
            stdin_path=input_path,
            stdout_path=expected_path,
        ).seconds
        source_seconds = run_cmd(
            solution_command or [str(solution)],
            f"{source.item.name}/{spec.name}: 运行当前独立源码",
            run_timeout,
            cwd=problem,
            stdin_path=input_path,
            stdout_path=actual_path,
        ).seconds
        # Windows CRT may turn an official solver's LF into CRLF, while a few
        # official checkers require literal LF. Both are temporary checker inputs.
        for path in (expected_path, actual_path):
            data = path.read_bytes()
            if b"\r\n" in data:
                path.write_bytes(data.replace(b"\r\n", b"\n"))
        checker_seconds = run_cmd(
            [str(checker), str(input_path), str(actual_path), str(expected_path)],
            f"{source.item.name}/{spec.name}: 官方 checker",
            run_timeout,
            cwd=problem,
        ).seconds
        record = {
            "kind": spec.kind,
            "name": spec.name,
            "source": spec.source,
            "index": spec.index,
            "seed": spec.seed,
            "status": "passed",
            "input_sha256": yosupo.sha256_file(input_path),
            "expected_sha256": yosupo.sha256_file(expected_path),
            "actual_sha256": yosupo.sha256_file(actual_path),
            "seconds": round(time.monotonic() - started, 6),
            "generation_seconds": round(generation_seconds, 6),
            "correct_seconds": round(correct_seconds, 6),
            "source_seconds": round(source_seconds, 6),
            "checker_seconds": round(checker_seconds, 6),
        }
        records.append(record)
        print(f"PASSED {source.item.name}: {spec.kind} {spec.name}")
    return records


def compile_sources(
    prepared: list[yosupo.PreparedSource],
    build: Path,
    compiler: str,
    compiler_args: tuple[str, ...],
    timeout: float,
) -> tuple[dict[str, Path], dict[str, float]]:
    binaries = {}
    seconds = {}
    for index, source in enumerate(prepared):
        source_path = build / "source" / f"{index:02d}-{source.item.name}.cpp"
        source_path.parent.mkdir(parents=True, exist_ok=True)
        source_path.write_text(source.source, encoding="utf-8", newline="\n")
        output = _exe(build / "source-bin" / f"{index:02d}-{source.item.name}")
        elapsed = compile_cpp(
            source_path,
            output,
            compiler,
            timeout,
            compiler_args,
            online_judge=True,
            label=f"{source.item.name}: 编译当前独立源码",
        )
        binaries[source.item.name] = output
        seconds[source.item.name] = elapsed
        print(f"COMPILED {source.item.name}")
    return binaries, seconds


def run_template_checks(
    checks: dict[str, Path],
    build: Path,
    compiler: str,
    compiler_args: tuple[str, ...],
    compile_timeout: float,
    run_timeout: float,
) -> dict[str, dict]:
    records = {}
    for index, (relative, source) in enumerate(sorted(checks.items())):
        output = _exe(build / "test-bin" / f"{index:02d}-{source.stem}")
        compile_seconds = compile_cpp(
            source,
            output,
            compiler,
            compile_timeout,
            compiler_args,
            label=f"编译模板测试 {relative}",
        )
        run_seconds = run_cmd(
            [str(output)],
            f"运行模板测试 {relative}",
            run_timeout,
            cwd=source.parent,
        ).seconds
        records[relative] = {
            "path": relative,
            "sha256": yosupo.sha256_file(source),
            "status": "passed",
            "compile_seconds": round(compile_seconds, 6),
            "run_seconds": round(run_seconds, 6),
        }
        print(f"TESTED {relative}")
    return records


def _entry_map(manifest: dict) -> dict[str, dict]:
    entries = manifest.get("problems")
    if not isinstance(entries, list):
        raise LocalVerifyError("manifest 缺少 problems 数组")
    result = {}
    for entry in entries:
        if not isinstance(entry, dict) or not isinstance(entry.get("name"), str):
            raise LocalVerifyError("manifest 包含无效题目条目")
        name = entry["name"]
        if name in result:
            raise LocalVerifyError(f"manifest 包含重复题目：{name}")
        result[name] = entry
    return result


def _ensure_current(
    root: Path,
    manifest_hash: str,
    prepared: list[yosupo.PreparedSource],
    checks: dict[str, Path],
    check_records: dict[str, dict],
) -> None:
    current_manifest = read_manifest(root)
    if manifest_sha256(current_manifest) != manifest_hash:
        raise LocalVerifyError("manifest 在本地预检期间发生变化")
    for source in prepared:
        try:
            yosupo.check_current(source)
        except yosupo.SubmissionError as error:
            raise LocalVerifyError(str(error)) from error
    for relative, path in checks.items():
        if yosupo.sha256_file(path) != check_records[relative]["sha256"]:
            raise LocalVerifyError(f"模板测试在本地预检期间发生变化：{relative}")


def merge_receipt(path: Path, receipt: dict, names: set[str]) -> None:
    # path 是目标收据；receipt 是本轮结果；names 是当前 manifest 中的题目名。
    # 返回值为空；同一 manifest、commit 和编译配置下保留其他题的旧记录。
    try:
        old = json.loads(path.read_text(encoding="utf-8"))
    except FileNotFoundError:
        return
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
        raise LocalVerifyError(f"无法读取已有本地预检收据：{path}") from error
    if not isinstance(old, dict):
        raise LocalVerifyError(f"已有本地预检收据根节点不是对象：{path}")
    same = (
        old.get("schema_version") == receipt.get("schema_version")
        and old.get("kind") == receipt.get("kind")
        and old.get("status") == "passed"
        and old.get("manifest_sha256") == receipt.get("manifest_sha256")
        and old.get("official_repository") == receipt.get("official_repository")
        and old.get("compiler") == receipt.get("compiler")
    )
    if not same:
        return
    records = old.get("problems")
    if not isinstance(records, dict):
        raise LocalVerifyError(f"已有本地预检收据缺少 problems：{path}")
    current = receipt["problems"]
    receipt["problems"] = {
        name: record
        for name, record in records.items()
        if name in names and name not in current
    } | current


def verify(
    root: Path,
    manifest: dict,
    prepared: list[yosupo.PreparedSource],
    problems_root: Path,
    compiler: str = "g++",
    compiler_args: tuple[str, ...] = (),
    compile_timeout: float = 180.0,
    run_timeout: float = 60.0,
    receipt_path: Path | None = None,
) -> Path:
    root = root.resolve()
    commit = official_commit(manifest)
    problems_root = local_arg_path(root, problems_root, "官方题库")
    problems_root = validate_official_root(
        problems_root,
        commit,
        min(compile_timeout, 60.0),
    )
    manifest_hash = manifest_sha256(manifest)
    by_problem, checks = collect_checks(root, prepared)
    entries = _entry_map(manifest)
    compiler_args = _compiler_args(tuple(compiler_args))

    with tempfile.TemporaryDirectory(prefix="yosupo-local-") as directory:
        build = Path(directory)
        binaries, compile_times = compile_sources(
            prepared,
            build,
            compiler,
            compiler_args,
            compile_timeout,
        )
        check_records = run_template_checks(
            checks,
            build,
            compiler,
            compiler_args,
            compile_timeout,
            run_timeout,
        )
        problem_records = {}
        for source in prepared:
            entry = entries.get(source.item.name)
            if entry is None:
                raise LocalVerifyError(f"manifest 缺少题目：{source.item.name}")
            problem_work = build / "official" / source.item.name
            cases = run_official_cases(
                root,
                source,
                binaries[source.item.name],
                entry,
                problems_root,
                commit,
                problem_work,
                compiler,
                compiler_args,
                compile_timeout,
                run_timeout,
            )
            problem_records[source.item.name] = {
                "status": "passed",
                "verified_at": yosupo.utc_now(),
                "official_commit": commit,
                "source_sha256": source.source_sha256,
                "source_compile": {
                    "status": "passed",
                    "seconds": round(compile_times[source.item.name], 6),
                },
                "source_checks": [
                    check_records[path] for path in by_problem[source.item.name]
                ],
                "cases": cases,
            }

        validate_official_root(
            problems_root,
            commit,
            min(compile_timeout, 60.0),
        )
        _ensure_current(root, manifest_hash, prepared, checks, check_records)

    receipt = {
        "schema_version": RECEIPT_SCHEMA,
        "kind": RECEIPT_KIND,
        "status": "passed",
        "verified_at": yosupo.utc_now(),
        "manifest_sha256": manifest_hash,
        "official_repository": {"commit": commit},
        "compiler": {
            "command": compiler,
            "args": list(compiler_args),
            "linker_args": list(yosupo.local_linker_args()),
            "standard": "gnu++20",
            "optimization": "-O2",
            "ndebug": False,
        },
        "problems": problem_records,
    }
    target = (
        local_arg_path(root, receipt_path, "本地预检收据")
        if receipt_path is not None
        else default_receipt_path(root)
    )
    target.parent.mkdir(parents=True, exist_ok=True)
    merge_receipt(target, receipt, set(entries))
    yosupo.atomic_write_json(target, receipt)
    return target


def require_receipt(
    root: Path,
    prepared: list[yosupo.PreparedSource],
    manifest: dict,
    receipt_path: Path | None = None,
) -> dict:
    root = root.resolve()
    path = (
        local_arg_path(root, receipt_path, "本地预检收据")
        if receipt_path is not None
        else default_receipt_path(root)
    )
    try:
        receipt = json.loads(path.read_text(encoding="utf-8"))
    except FileNotFoundError as error:
        raise LocalVerifyError(f"缺少本地预检收据：{path}") from error
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
        raise LocalVerifyError(f"无法读取本地预检收据：{path}") from error
    if not isinstance(receipt, dict):
        raise LocalVerifyError("本地预检收据根节点不是对象")
    if (
        receipt.get("schema_version") != RECEIPT_SCHEMA
        or receipt.get("kind") != RECEIPT_KIND
        or receipt.get("status") != "passed"
    ):
        raise LocalVerifyError("本地预检收据版本或状态无效")
    commit = official_commit(manifest)
    repository = receipt.get("official_repository")
    if not isinstance(repository, dict) or repository.get("commit") != commit:
        raise LocalVerifyError("本地预检收据的官方 commit 已过期")
    if receipt.get("manifest_sha256") != manifest_sha256(manifest):
        raise LocalVerifyError("本地预检收据的 manifest 哈希已过期")
    if not isinstance(receipt.get("verified_at"), str) or not receipt["verified_at"]:
        raise LocalVerifyError("本地预检收据缺少验证时间")
    compiler = receipt.get("compiler")
    if (
        not isinstance(compiler, dict)
        or not isinstance(compiler.get("command"), str)
        or not compiler["command"]
        or not isinstance(compiler.get("args"), list)
        or not all(isinstance(arg, str) for arg in compiler["args"])
        or compiler.get("linker_args") != list(yosupo.local_linker_args())
        or compiler.get("standard") != "gnu++20"
        or compiler.get("optimization") != "-O2"
        or compiler.get("ndebug") is not False
        or any("NDEBUG" in arg for arg in compiler["args"])
    ):
        raise LocalVerifyError("本地预检收据的编译配置无效")
    records = receipt.get("problems")
    if not isinstance(records, dict):
        raise LocalVerifyError("本地预检收据缺少 problems")

    by_problem, checks = collect_checks(root, prepared)
    for source in prepared:
        try:
            yosupo.check_current(source)
        except yosupo.SubmissionError as error:
            raise LocalVerifyError(str(error)) from error
        record = records.get(source.item.name)
        if not isinstance(record, dict) or record.get("status") != "passed":
            raise LocalVerifyError(f"{source.item.name}: 没有通过的本地预检记录")
        if record.get("official_commit") != commit:
            raise LocalVerifyError(f"{source.item.name}: 官方 commit 已过期")
        if record.get("source_sha256") != source.source_sha256:
            raise LocalVerifyError(f"{source.item.name}: 源码哈希已过期")
        if not isinstance(record.get("verified_at"), str) or not record["verified_at"]:
            raise LocalVerifyError(f"{source.item.name}: 缺少验证时间")
        compiled = record.get("source_compile")
        if not isinstance(compiled, dict) or compiled.get("status") != "passed":
            raise LocalVerifyError(f"{source.item.name}: 源码编译记录无效")

        got_checks = record.get("source_checks")
        if not isinstance(got_checks, list):
            raise LocalVerifyError(f"{source.item.name}: 模板测试记录无效")
        got_by_path = {
            check.get("path"): check
            for check in got_checks
            if isinstance(check, dict) and isinstance(check.get("path"), str)
        }
        if (
            len(got_by_path) != len(got_checks)
            or set(got_by_path) != set(by_problem[source.item.name])
        ):
            raise LocalVerifyError(f"{source.item.name}: 模板测试集合已过期")
        for relative in by_problem[source.item.name]:
            check = got_by_path[relative]
            if (
                check.get("status") != "passed"
                or check.get("sha256") != yosupo.sha256_file(checks[relative])
            ):
                raise LocalVerifyError(f"{source.item.name}: 模板测试已过期：{relative}")

        cases = record.get("cases")
        if not isinstance(cases, list):
            raise LocalVerifyError(f"{source.item.name}: 官方用例记录无效")
        by_kind = {
            case.get("kind"): case
            for case in cases
            if isinstance(case, dict) and isinstance(case.get("kind"), str)
        }
        if set(by_kind) != set(CASE_KINDS) or len(cases) != len(CASE_KINDS):
            raise LocalVerifyError(f"{source.item.name}: 官方三类用例不完整")
        for kind in CASE_KINDS:
            case = by_kind[kind]
            if (
                case.get("status") != "passed"
                or not isinstance(case.get("name"), str)
                or not case["name"]
                or not isinstance(case.get("source"), str)
                or not case["source"]
                or type(case.get("index")) is not int
                or case["index"] != 0
                or case["name"]
                != f"{Path(case['source']).stem}_{case['index']:02d}"
                or any(
                    not isinstance(case.get(field), str)
                    or SHA256_RE.fullmatch(case[field]) is None
                    for field in (
                        "input_sha256",
                        "expected_sha256",
                        "actual_sha256",
                    )
                )
            ):
                raise LocalVerifyError(
                    f"{source.item.name}: {kind} 用例记录无效"
                )
            seed = case.get("seed")
            if (kind == "random" and seed != RANDOM_SEED) or (
                kind != "random" and seed is not None
            ):
                raise LocalVerifyError(
                    f"{source.item.name}: {kind} 固定种子不匹配"
                )
        custom_boundary = custom_boundary_input(root, source)
        if custom_boundary is not None:
            fixture, relative = custom_boundary
            boundary = by_kind["boundary"]
            if (
                boundary.get("source") != relative
                or boundary.get("input_sha256") != yosupo.sha256_file(fixture)
            ):
                raise LocalVerifyError(f"{source.item.name}: 自建边界输入已过期")
    return receipt


def main() -> None:
    parser = argparse.ArgumentParser(
        description="离线编译并验证当前 Library Checker 独立源码",
    )
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--only", help="逗号分隔的题目名")
    mode.add_argument("--all", action="store_true", help="验证全部题目（默认）")
    parser.add_argument(
        "--problems-root",
        type=Path,
        help=(
            "固定 commit 的题库目录，默认本目录 .local/library-checker-problems；"
            "相对路径必须位于本目录 .local 下"
        ),
    )
    parser.add_argument("--compiler", default="g++", help="gnu++20 编译器")
    parser.add_argument(
        "--compiler-arg",
        action="append",
        default=[],
        help="附加编译参数；以 - 开头时使用 --compiler-arg=VALUE",
    )
    parser.add_argument(
        "--compile-timeout",
        type=float,
        default=180.0,
        help="单次编译超时秒数，默认 180",
    )
    parser.add_argument(
        "--run-timeout",
        type=float,
        default=60.0,
        help="单次程序运行超时秒数，默认 60",
    )
    parser.add_argument(
        "--receipt",
        type=Path,
        help=(
            f"收据路径，默认本目录 .local/{RECEIPT_NAME}；"
            "相对路径必须位于本目录 .local 下"
        ),
    )
    args = parser.parse_args()
    if args.compile_timeout <= 0 or args.run_timeout <= 0:
        parser.error("超时必须为正数")

    root = Path(__file__).resolve().parent.parent
    problems_root = args.problems_root or default_problems_path(root)
    try:
        manifest = read_manifest(root)
        wanted = yosupo.parse_wanted(args.only)
        prepared = load_sources(root, manifest, wanted)
        print(f"共验证 {len(prepared)} 题")
        receipt = verify(
            root,
            manifest,
            prepared,
            problems_root,
            compiler=args.compiler,
            compiler_args=tuple(args.compiler_arg),
            compile_timeout=args.compile_timeout,
            run_timeout=args.run_timeout,
            receipt_path=args.receipt,
        )
    except (LocalVerifyError, yosupo.SubmissionError) as error:
        sys.exit(str(error))
    print(f"LOCAL VERIFIED: {receipt}")


if __name__ == "__main__":
    main()
