#!/usr/bin/env python3
"""生成第 03 章单题提交前的可重算批次身份。"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import math
import re
import statistics
import subprocess
import sys
from pathlib import Path, PurePosixPath

import submit_yosupo as yosupo
import verify_local


class BatchError(RuntimeError):
    pass


OFFICIAL_REPOSITORY_URL = "https://github.com/yosupo06/library-checker-problems"
OFFICIAL_LICENSE = "Apache-2.0"
OFFICIAL_LICENSE_BLOB = "91554e3bd99da6761c82645d6c740ced590d17bb"


def canonical_bytes(value: object) -> bytes:
    # value 是 JSON 可序列化对象；返回流程规定的规范化 UTF-8 字节串。
    return json.dumps(
        value,
        ensure_ascii=False,
        sort_keys=True,
        separators=(",", ":"),
    ).encode("utf-8")


def canonical_sha256(value: object) -> str:
    # value 是 JSON 可序列化对象；返回其规范化 SHA256。
    return hashlib.sha256(canonical_bytes(value)).hexdigest()


def sha256_file(path: Path) -> str:
    # path 是常规文件；返回其原始字节 SHA256。
    return hashlib.sha256(path.read_bytes()).hexdigest()


def read_json(path: Path) -> dict:
    # path 是 JSON 文件；返回根对象，非法时抛出 BatchError。
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
        raise BatchError(f"无法读取 JSON：{path}") from error
    if not isinstance(value, dict):
        raise BatchError(f"JSON 根节点不是对象：{path}")
    return value


def load_module(path: Path, name: str):
    # path 是 Python 工具路径，name 是本次加载的模块名；返回当前文件对应的模块。
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise BatchError(f"无法加载工具：{path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def load_generator(root: Path):
    # root 是题解目录；加载当前第 03 章生成器以在批次前重新生成源码。
    return load_module(Path(__file__).resolve().parent / "generate_ch03_lc.py", "ch03_batch_generator")


def local_path(root: Path, path: Path, label: str) -> Path:
    # root 是题解目录；返回 .local 内的显式批次或性能状态文件路径。
    target = path.resolve() if path.is_absolute() else (root / path).resolve()
    local = (root / ".local").resolve()
    try:
        target.relative_to(local)
    except ValueError as error:
        raise BatchError(f"{label} 必须位于 .local 内：{path}") from error
    return target


def workspace_path(root: Path, value: object, label: str) -> Path:
    # root 是题解目录，value 是仓库相对 POSIX 路径；拒绝绝对路径和越界路径。
    if not isinstance(value, str) or not value:
        raise BatchError(f"{label} 必须是非空仓库相对 POSIX 路径")
    if "\\" in value:
        raise BatchError(f"{label} 必须使用 POSIX 路径")
    pure = PurePosixPath(value)
    if value != pure.as_posix() or pure.is_absolute() or ".." in pure.parts:
        raise BatchError(f"{label} 不是规范仓库相对路径：{value}")
    workspace = root.parent.resolve()
    target = workspace.joinpath(*pure.parts).resolve()
    try:
        target.relative_to(workspace)
    except ValueError as error:
        raise BatchError(f"{label} 越出工作区：{value}") from error
    return target


def git_bytes(problems_root: Path, arguments: list[str], label: str) -> bytes:
    try:
        result = subprocess.run(
            ["git", "-C", str(problems_root), *arguments],
            capture_output=True,
            check=False,
            timeout=30,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise BatchError(f"无法读取固定官方题库的 {label}") from error
    if result.returncode:
        detail = result.stderr.decode("utf-8", errors="replace").strip()
        raise BatchError(f"无法读取固定官方题库的 {label}: {detail}")
    return result.stdout


def official_names(problems_root: Path, commit: str) -> list[str]:
    # problems_root 是已冻结官方工作树；返回 data_structure 下按名字排序的直属题目名。
    result = subprocess.run(
        [
            "git",
            "-C",
            str(problems_root),
            "ls-tree",
            "-d",
            "--name-only",
            f"{commit}:data_structure",
        ],
        capture_output=True,
        check=False,
        timeout=30,
    )
    if result.returncode:
        raise BatchError("无法枚举固定 commit 的 data_structure 官方题目")
    names = [name for name in result.stdout.decode("utf-8").splitlines() if name]
    if len(names) != len(set(names)):
        raise BatchError("官方 data_structure 题目名重复")
    return sorted(names)


def validate_coverage_matrix(audit_path: Path, names: list[str]) -> None:
    try:
        text = audit_path.read_text(encoding="utf-8")
    except (OSError, UnicodeDecodeError) as error:
        raise BatchError(f"无法读取第 03 章覆盖矩阵：{audit_path}") from error
    sections = re.findall(
        r"(?ms)^## Coverage Status\s*$\n(.*?)(?=^## |\Z)",
        text,
    )
    if len(sections) != 1:
        raise BatchError("docs/03-数据结构审计.md 必须恰有一个 Coverage Status 段")
    rows = re.findall(
        r"(?m)^\|\s*`data_structure/([A-Za-z0-9_]+)`\s*\|",
        sections[0],
    )
    official = set(names)
    listed = set(rows)
    missing = sorted(official - listed)
    extra = sorted(listed - official)
    duplicate = sorted(name for name in listed if rows.count(name) != 1)
    if len(rows) != len(names) or missing or extra or duplicate:
        detail = []
        if missing:
            detail.append("缺少=" + ", ".join(missing))
        if extra:
            detail.append("非官方=" + ", ".join(extra))
        if duplicate:
            detail.append("重复=" + ", ".join(duplicate))
        raise BatchError("data_structure 覆盖矩阵不是固定 commit 的完整唯一题集：" + "; ".join(detail))


def require_current_audit(root: Path, audit_path: Path, names: list[str]) -> None:
    # root 是题解目录，audit_path 是当前矩阵；重新渲染后要求文件字节完全一致。
    try:
        audit = load_module(Path(__file__).resolve().parent / "refresh_ch03_audit.py", "ch03_batch_audit")
        expected = audit.render()
        actual = audit_path.read_text(encoding="utf-8")
    except (OSError, RuntimeError, ValueError) as error:
        raise BatchError(f"无法重算第 03 章覆盖矩阵：{error}") from error
    if actual != expected:
        raise BatchError("docs/03-数据结构审计.md 已过期，须先刷新并复核")
    validate_coverage_matrix(audit_path, names)


def is_number(value: object) -> bool:
    # value 是 JSON 标量；返回它是否为有限实数且不是布尔值。
    return type(value) in {int, float} and math.isfinite(value)


def require_sha256(value: object, label: str) -> str:
    # value 是待校验哈希，label 标识所属字段；返回合法的小写 SHA256。
    if not isinstance(value, str) or re.fullmatch(r"[0-9a-f]{64}", value) is None:
        raise BatchError(f"{label} 不是小写 SHA256")
    return value


def validate_run(run: object, order: str, label: str) -> dict:
    # run 是一次已由 checker 判定的计时记录，order 是候选或参考身份；返回经校验的记录。
    if not isinstance(run, dict):
        raise BatchError(f"{label}: 计时记录不是对象")
    if run.get("order") != order:
        raise BatchError(f"{label}: 计时记录顺序不一致")
    seconds = run.get("seconds")
    if not is_number(seconds) or seconds < 0:
        raise BatchError(f"{label}: 计时无效")
    peak = run.get("peak_working_set_bytes")
    if peak is not None and (type(peak) is not int or peak <= 0):
        raise BatchError(f"{label}: 峰值内存无效")
    require_sha256(run.get("output_sha256"), f"{label}: 输出哈希")
    return run


def validate_side(
    side: object,
    order: str,
    path: str,
    source_sha256: str,
    label: str,
) -> None:
    # side 是候选或参考的性能对象；验证五轮原始记录、媒值、峰值与当前源码身份。
    if not isinstance(side, dict):
        raise BatchError(f"{label}: 性能对象无效")
    if side.get("path") != path or side.get("sha256") != source_sha256:
        raise BatchError(f"{label}: 源码身份不一致")
    runs = side.get("runs")
    if not isinstance(runs, list) or len(runs) < 5:
        raise BatchError(f"{label}: 有效轮次不足 5")
    checked = [validate_run(run, order, label) for run in runs]
    seconds = [run["seconds"] for run in checked]
    median = statistics.median(seconds)
    if not is_number(side.get("median_seconds")) or not math.isclose(
        side["median_seconds"], median, rel_tol=0.0, abs_tol=1e-12
    ):
        raise BatchError(f"{label}: 媒值与原始轮次不一致")
    expected_peak = max((run["peak_working_set_bytes"] or 0 for run in checked), default=0) or None
    if side.get("peak_working_set_bytes") != expected_peak:
        raise BatchError(f"{label}: 峰值内存与原始轮次不一致")


def validate_performance(
    performance: dict,
    name: str,
    problem_id: str,
    commit: str,
    candidate_path: str,
    candidate_sha256: str,
    reference_path: str,
    reference_sha256: str,
) -> None:
    # performance 是单题五轮基准对象；检查来源、原始计时、媒值、阈值和两侧输出一致性。
    if performance.get("schema_version") != 1 or performance.get("kind") != "template_performance_review":
        raise BatchError(f"{name}: 缺少规范性能门对象")
    if performance.get("problem_id") != problem_id:
        raise BatchError(f"{name}: 性能门题目不一致")
    if performance.get("official_repository_commit") != commit:
        raise BatchError(f"{name}: 性能门官方 commit 不一致")
    input_info = performance.get("input")
    if (
        not isinstance(input_info, dict)
        or not isinstance(input_info.get("generator"), str)
        or not input_info["generator"].startswith("gen/")
        or not isinstance(input_info.get("seed"), int)
        or type(input_info.get("bytes")) is not int
        or input_info["bytes"] <= 0
    ):
        raise BatchError(f"{name}: 性能输入记录无效")
    require_sha256(input_info.get("generator_sha256"), f"{name}: 性能生成器哈希")
    require_sha256(input_info.get("sha256"), f"{name}: 性能输入哈希")
    environment = performance.get("environment")
    expected_flags = [*verify_local.COMPILE_FLAGS, *yosupo.local_linker_args()]
    if (
        not isinstance(environment, dict)
        or environment.get("platform") != sys.platform
        or not isinstance(environment.get("compiler"), str)
        or not environment["compiler"]
        or environment.get("compile_flags") != expected_flags
        or environment.get("measurement")
        != "wall_clock_seconds; Windows PeakWorkingSetSize sampled every 2ms"
    ):
        raise BatchError(f"{name}: 性能环境记录无效")
    candidate = performance.get("candidate")
    reference = performance.get("reference")
    validate_side(candidate, "candidate", candidate_path, candidate_sha256, f"{name}: 候选")
    validate_side(reference, "reference", reference_path, reference_sha256, f"{name}: 参考")
    threshold = performance.get("threshold")
    ratio = performance.get("candidate_over_reference_median")
    if not is_number(threshold) or threshold <= 0 or threshold > 1.25:
        raise BatchError(f"{name}: 性能阈值无效或放宽")
    candidate_median = candidate["median_seconds"]
    reference_median = reference["median_seconds"]
    if reference_median <= 0 or not is_number(ratio) or not math.isclose(
        ratio, candidate_median / reference_median, rel_tol=0.0, abs_tol=1e-12
    ):
        raise BatchError(f"{name}: 性能倍率与媒值不一致")
    if performance.get("status") != "passed" or ratio > threshold:
        raise BatchError(f"{name}: 性能门未通过")


def validate_official_reference(
    meta: dict,
    name: str,
    problems_root: Path,
    commit: str,
) -> dict:
    reference = meta.get("official_reference")
    if not isinstance(reference, dict):
        raise BatchError(f"{name}: 候选元数据缺少官方 correct.cpp 来源")
    path = f"data_structure/{name}/sol/correct.cpp"
    expected = {
        "repository": OFFICIAL_REPOSITORY_URL,
        "commit": commit,
        "license": OFFICIAL_LICENSE,
        "license_blob": OFFICIAL_LICENSE_BLOB,
        "path": path,
    }
    for field, value in expected.items():
        if reference.get(field) != value:
            raise BatchError(f"{name}: 官方来源 {field} 不一致")
    license_blob = git_bytes(
        problems_root,
        ["rev-parse", f"{commit}:LICENSE"],
        "LICENSE blob",
    ).decode("ascii").strip()
    if license_blob != OFFICIAL_LICENSE_BLOB:
        raise BatchError(f"{name}: 固定官方许可证 blob 不一致")
    blob = git_bytes(
        problems_root,
        ["rev-parse", f"{commit}:{path}"],
        f"{name} correct.cpp blob",
    ).decode("ascii").strip()
    source = git_bytes(
        problems_root,
        ["show", f"{commit}:{path}"],
        f"{name} correct.cpp",
    )
    content_sha256 = hashlib.sha256(source).hexdigest()
    if reference.get("git_blob") != blob:
        raise BatchError(f"{name}: 官方 correct.cpp Git blob 不一致")
    if reference.get("content_sha256") != content_sha256:
        raise BatchError(f"{name}: 官方 correct.cpp 内容哈希不一致")
    return reference


def current_online_ac(root: Path, manifest: dict, names: list[str]) -> list[str]:
    # manifest 是当前清单，names 是官方 data_structure 题名；返回当前可认证 AC 的官方唯一标识。
    entries = {
        entry.get("name"): entry
        for entry in manifest.get("problems", [])
        if isinstance(entry, dict) and entry.get("category") == "data_structure"
    }
    result = []
    for name in names:
        entry = entries.get(name)
        if entry is None:
            continue
        try:
            item = yosupo.load_items(root, manifest, {name}, solution_only=True)
            prepared = yosupo.prepare_sources(item)
        except yosupo.SubmissionError as error:
            raise BatchError(f"{name}: 无法检查当前在线状态：{error}") from error
        if len(prepared) == 1 and yosupo.already_certified(prepared[0]):
            result.append(f"data_structure/{name}")
    return sorted(result)


def validate_generation(
    root: Path,
    meta: dict,
    problem_id: str,
    source_sha256: str,
) -> tuple[dict, list[dict]]:
    # meta 是当前逐题元数据；验证确定性生成证据并返回生成器对象和模板输入列表。
    provenance = meta.get("provenance")
    evidence = provenance.get("evidence") if isinstance(provenance, dict) else None
    if not isinstance(evidence, dict) or evidence.get("method") != "deterministic_generation":
        raise BatchError(f"{problem_id}: 缺少确定性生成来源证据")
    generator = evidence.get("generator")
    if not isinstance(generator, dict):
        raise BatchError(f"{problem_id}: 缺少生成器证据")
    generation = generator.get("generation_evidence")
    if not isinstance(generation, dict):
        raise BatchError(f"{problem_id}: 缺少生成比对对象")
    if generator.get("generation_evidence_sha256") != canonical_sha256(generation):
        raise BatchError(f"{problem_id}: 生成比对对象哈希不一致")
    checks = {
        "problem_id": (generation.get("problem_id"), problem_id),
        "source_path": (
            generation.get("source_path"),
            f"library-checker-template-solutions/{problem_id}/main.cpp",
        ),
        "source_sha256": (generation.get("source_sha256"), source_sha256),
        "generated_sha256": (generation.get("generated_sha256"), source_sha256),
        "byte_equal": (generation.get("byte_equal"), True),
        "generator_path": (generation.get("generator_path"), generator.get("path")),
        "generator_sha256": (generation.get("generator_sha256"), generator.get("sha256")),
        "working_directory": (generation.get("working_directory"), generator.get("working_directory")),
        "runner": (generation.get("runner"), generator.get("runner")),
        "argv": (generation.get("argv"), generator.get("argv")),
    }
    for field, (actual, expected) in checks.items():
        if actual != expected:
            raise BatchError(f"{problem_id}: 生成证据 {field} 不一致")
    expected_generator_path = "library-checker-template-solutions/tools/generate_ch03_lc.py"
    if generator.get("path") != expected_generator_path:
        raise BatchError(f"{problem_id}: 生成器路径不是当前第 03 章生成器")
    if generator.get("working_directory") != ".":
        raise BatchError(f"{problem_id}: 生成器工作目录无效")
    if not isinstance(generator.get("runner"), str) or not generator["runner"]:
        raise BatchError(f"{problem_id}: 生成器运行时无效")
    if not isinstance(generator.get("argv"), list) or not all(
        isinstance(arg, str) for arg in generator["argv"]
    ):
        raise BatchError(f"{problem_id}: 生成器参数无效")
    require_sha256(generator.get("sha256"), f"{problem_id}: 生成器哈希")
    generator_path = workspace_path(root, generator["path"], f"{problem_id}: 生成器路径")
    if not generator_path.is_file() or sha256_file(generator_path) != generator["sha256"]:
        raise BatchError(f"{problem_id}: 当前生成器已变化，须重新生成候选")
    inputs = generation.get("inputs")
    if not isinstance(inputs, list) or inputs != sorted(inputs, key=lambda x: x.get("path", "")):
        raise BatchError(f"{problem_id}: 生成输入未按路径排序")
    input_paths = []
    for item in inputs:
        if not isinstance(item, dict) or not isinstance(item.get("path"), str):
            raise BatchError(f"{problem_id}: 生成输入无效")
        require_sha256(item.get("sha256"), f"{problem_id}: 模板输入哈希")
        path = workspace_path(root, item["path"], f"{problem_id}: 模板输入路径")
        if not path.is_file() or item.get("sha256") != sha256_file(path):
            raise BatchError(f"{problem_id}: 模板输入已变化：{item['path']}")
        input_paths.append(item["path"])
    if len(input_paths) != len(set(input_paths)):
        raise BatchError(f"{problem_id}: 生成输入路径重复")

    # 元数据中的生成记录只能证明登记时的结果。在线发送前再调用当前生成器，
    # 直接比较新字节和唯一候选，防止生成器行为在不改变声明输入时漂移。
    current_generator = load_generator(root)
    specs = {spec.name: spec for spec in current_generator.SPECS}
    name = problem_id.removeprefix("data_structure/")
    spec = specs.get(name)
    if spec is None:
        raise BatchError(f"{problem_id}: 当前生成器没有对应题目")
    try:
        generated, headers = current_generator.generate_twice(root.parent, spec)
    except (OSError, ValueError) as error:
        raise BatchError(f"{problem_id}: 当前重新生成失败：{error}") from error
    target = root / current_generator.source_path(spec)
    if not target.is_file() or target.read_bytes() != generated:
        raise BatchError(f"{problem_id}: 当前重新生成与 main.cpp 不一致")
    if hashlib.sha256(generated).hexdigest() != source_sha256:
        raise BatchError(f"{problem_id}: 当前重新生成源码哈希不一致")
    expected_inputs = current_generator.describe(root.parent, spec, generated, headers)["template_inputs"]
    if inputs != expected_inputs:
        raise BatchError(f"{problem_id}: 生成输入与当前生成器不一致")
    return generator, expected_inputs


def make_batch(root: Path, name: str, performance_path: Path) -> dict:
    # root 是题解目录，name 是唯一题名，performance_path 是当前性能 JSON；返回 schema 2 批次对象。
    performance_path = local_path(root, performance_path, "性能结果路径")
    performance = read_json(performance_path)
    manifest_path = root / "manifest.json"
    manifest = read_json(manifest_path)
    prepared = verify_local.load_sources(root, manifest, {name})
    if len(prepared) != 1:
        raise BatchError(f"{name}: 无法唯一定位当前候选")
    source = prepared[0]
    entry = next(entry for entry in manifest["problems"] if entry["name"] == name)
    if entry.get("category") != "data_structure":
        raise BatchError(f"{name}: 不是第 03 章 data_structure 候选")
    problem_id = f"data_structure/{name}"
    try:
        # 单题批次只需验证本题的当前记录；收据本身仍与完整 manifest 绑定。
        verify_local.require_receipt(root, [source], manifest)
    except verify_local.LocalVerifyError as error:
        raise BatchError(f"{name}: 当前本地收据无效：{error}") from error
    meta = read_json(source.item.meta_path)
    generator, template_inputs = validate_generation(
        root,
        meta,
        problem_id,
        source.source_sha256,
    )
    reference = meta.get("reference")
    if not isinstance(reference, dict) or not isinstance(reference.get("source"), str):
        raise BatchError(f"{name}: 缺少受控参考来源")
    reference_path = root.parent / reference["source"]
    reference_meta_path = reference_path.parent / "meta.json"
    if not reference_path.is_file() or not reference_meta_path.is_file():
        raise BatchError(f"{name}: 参考源码或元数据不存在")
    if reference.get("source_sha256") != sha256_file(reference_path):
        raise BatchError(f"{name}: 参考源码哈希已变化")

    commit = verify_local.official_commit(manifest)
    validate_performance(
        performance,
        name,
        problem_id,
        commit,
        entry["source"],
        source.source_sha256,
        reference["source"],
        reference["source_sha256"],
    )
    problems_root = verify_local.validate_official_root(
        verify_local.default_problems_path(root), commit
    )
    names = official_names(problems_root, commit)
    audit_path = root / "docs" / "03-数据结构审计.md"
    require_current_audit(root, audit_path, names)
    if name not in names:
        raise BatchError(f"{name}: 不在固定官方 data_structure 题集")
    official_reference = validate_official_reference(
        meta,
        name,
        problems_root,
        commit,
    )
    receipt_path = verify_local.default_receipt_path(root)
    tools = []
    for relative in (
        "library-checker-template-solutions/tools/benchmark_ch03.py",
        "library-checker-template-solutions/tools/ch03_batch_report.py",
        "library-checker-template-solutions/tools/ch03_result_report.py",
        "library-checker-template-solutions/tools/refresh_ch03_audit.py",
        "library-checker-template-solutions/tools/submit_yosupo.py",
        "library-checker-template-solutions/tools/verify_local.py",
    ):
        path = root.parent / relative
        if not path.is_file():
            raise BatchError(f"缺少工具：{relative}")
        tools.append({"path": relative, "sha256": sha256_file(path)})
    tools.sort(key=lambda item: item["path"])
    return {
        "schema_version": 2,
        "official_repository_commit": commit,
        "chapter": "03",
        "official_categories": ["data_structure"],
        "coverage_audit": {
            "path": "library-checker-template-solutions/docs/03-数据结构审计.md",
            "sha256": sha256_file(audit_path),
            "official_problem_count": len(names),
            "online_ac_problem_ids": current_online_ac(root, manifest, names),
        },
        "manifest_sha256": verify_local.manifest_sha256(manifest),
        "receipt": {
            "path": ".local/yosupo-local-verification.json",
            "sha256": sha256_file(receipt_path),
        },
        "performance": {
            "path": performance_path.relative_to(root.resolve()).as_posix(),
            "sha256": sha256_file(performance_path),
            "canonical_sha256": canonical_sha256(performance),
        },
        "authorization": {
            "mode": "direct_authorized",
            "user_confirmation_required": False,
            "scope": {
                "kind": "problem",
                "chapter": "03",
                "problem_id": problem_id,
            },
        },
        "tools": tools,
        "problems": [
            {
                "name": name,
                "official_category": "data_structure",
                "problem_id": problem_id,
                "source_path": f"library-checker-template-solutions/{entry['source']}",
                "source_sha256": source.source_sha256,
                "meta_path": f"library-checker-template-solutions/{entry['meta']}",
                "meta_pre_submit_sha256": sha256_file(source.item.meta_path),
                "template_inputs": template_inputs,
                "references": [
                    {
                        "path": reference["source"],
                        "sha256": reference["source_sha256"],
                        "meta_path": reference_meta_path.relative_to(root.parent).as_posix(),
                        "meta_sha256": sha256_file(reference_meta_path),
                        "official_reference": official_reference,
                    }
                ],
                "provenance": {
                    "method": "deterministic_generation",
                    "generator": {
                        "path": generator["path"],
                        "sha256": generator["sha256"],
                        "working_directory": generator["working_directory"],
                        "runner": generator["runner"],
                        "argv": generator["argv"],
                        "generation_evidence_sha256": generator["generation_evidence_sha256"],
                    },
                    "mapping_evidence_sha256": None,
                    "reviews": [],
                },
            }
        ],
    }


def revalidate_batch(root: Path, name: str, batch_path: Path) -> dict:
    # root 是题解目录，name 是待提交题名，batch_path 是已保存批次；重新计算所有门禁。
    batch_path = local_path(root, batch_path, "批次路径")
    stored = read_json(batch_path)
    if stored.get("schema_version") != 2 or stored.get("chapter") != "03":
        raise BatchError("批次不是第 03 章 schema 2 对象")
    problems = stored.get("problems")
    if not isinstance(problems, list) or len(problems) != 1 or not isinstance(problems[0], dict):
        raise BatchError("提交前批次必须恰有一道题")
    problem = problems[0]
    if (
        problem.get("name") != name
        or problem.get("problem_id") != f"data_structure/{name}"
    ):
        raise BatchError("批次题目与待提交题目不一致")
    scope = stored.get("authorization", {}).get("scope")
    if not isinstance(scope, dict) or scope.get("problem_id") != problem["problem_id"]:
        raise BatchError("批次授权范围与题目不一致")
    performance_identity = stored.get("performance")
    if not isinstance(performance_identity, dict):
        raise BatchError("批次缺少性能文件身份")
    performance_name = performance_identity.get("path")
    if not isinstance(performance_name, str) or not performance_name or "\\" in performance_name:
        raise BatchError("性能文件路径必须是规范 .local POSIX 路径")
    pure = PurePosixPath(performance_name)
    if performance_name != pure.as_posix() or pure.is_absolute() or ".." in pure.parts:
        raise BatchError("性能文件路径不是规范 .local 相对路径")
    performance_path = local_path(root, Path(*pure.parts), "性能文件路径")
    if performance_name != performance_path.relative_to(root.resolve()).as_posix():
        raise BatchError("性能文件路径不是规范 .local 相对路径")
    require_sha256(performance_identity.get("sha256"), "性能文件哈希")
    require_sha256(performance_identity.get("canonical_sha256"), "性能文件规范哈希")
    if not performance_path.is_file() or sha256_file(performance_path) != performance_identity["sha256"]:
        raise BatchError("性能文件已变化，批次失效")
    performance = read_json(performance_path)
    if canonical_sha256(performance) != performance_identity["canonical_sha256"]:
        raise BatchError("性能文件规范内容已变化，批次失效")
    current = make_batch(root, name, performance_path)
    if canonical_bytes(stored) != canonical_bytes(current):
        raise BatchError("当前源码、收据、性能或覆盖矩阵已变化，批次失效")
    return current


def main() -> None:
    parser = argparse.ArgumentParser(description="生成第 03 章单题批次 JSON")
    parser.add_argument("--only", required=True, help="唯一 data_structure 题目名")
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument(
        "--performance-file",
        type=Path,
        help="读取 .local 内 benchmark_ch03.py 直接写出的 JSON",
    )
    mode.add_argument(
        "--revalidate-batch",
        type=Path,
        help="重新计算 .local 内既有单题批次的全部技术门禁",
    )
    parser.add_argument(
        "--output",
        type=Path,
        help="将批次 JSON 直接以 UTF-8/LF 写入 .local 内的状态文件",
    )
    args = parser.parse_args()
    try:
        root = Path(__file__).resolve().parent.parent
        batch = (
            make_batch(root, args.only, args.performance_file)
            if args.performance_file is not None
            else revalidate_batch(root, args.only, args.revalidate_batch)
        )
    except (BatchError, OSError, ValueError, verify_local.LocalVerifyError, yosupo.SubmissionError) as error:
        sys.exit(str(error))
    if args.output is not None:
        try:
            target = local_path(root, args.output, "批次输出路径")
            target.parent.mkdir(parents=True, exist_ok=True)
            yosupo.atomic_write_json(target, batch)
            print(f"BATCH WRITTEN {target}")
        except (BatchError, OSError, yosupo.SubmissionError) as error:
            sys.exit(str(error))
    else:
        print(json.dumps(batch, ensure_ascii=False, indent=2))
    print(f"BATCH_SHA256 {canonical_sha256(batch)}", file=sys.stderr)


if __name__ == "__main__":
    main()
