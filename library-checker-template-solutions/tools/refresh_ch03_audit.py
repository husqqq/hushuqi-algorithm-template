#!/usr/bin/env python3
"""Render the Chapter 03 Library Checker coverage matrix from current evidence."""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
WORKSPACE = ROOT.parent
TOOLS = Path(__file__).resolve().parent
GENERATOR = TOOLS / "generate_ch03_lc.py"
RECEIPT = ROOT / ".local" / "yosupo-local-verification.json"
PERFORMANCE_DIR = ROOT / ".local" / "ch03-performance"
AUDIT = ROOT / "docs" / "03-数据结构审计.md"


# 第 03 章共有 110 个三级条目。SPECS 中的 topic 是单题候选的主入口；
# 下表记录正文明确复用、但不单独作为生成入口的条目，避免把它们错误写成 N/A。
SHARED_ENTRY_COVERAGE = {
    "3.1.002": (
        "`data_structure/unionfind`",
        "按大小合并是 `3.1.001 DSU` 的既有合并策略，正文明确该条直接依赖 `DSU`。",
    ),
    "3.2.003": (
        "`data_structure/point_add_range_sum`",
        "前缀和二分是 `3.2.001 BIT<T>::kth` 的同一公开接口，不另建结构。",
    ),
    "3.2.007": (
        "`data_structure/point_add_range_sum`",
        "取模树状数组只是 `BIT<T>` 的值类型配置，不另有底层实现。",
    ),
    "3.3.002": (
        "`data_structure/range_affine_point_get`, `data_structure/range_affine_range_sum`",
        "懒标记骨架由 `3.3.003 LazySeg` 作为当前候选的公共实现入口。",
    ),
    "3.3.013": (
        "`data_structure/area_of_union_of_rectangles`",
        "矩形周长并与面积并共用 `3.3.012 RectUnion` 的扫描线与覆盖计数实现。",
    ),
    "3.3.014": (
        "`data_structure/static_rectangle_add_rectangle_sum`",
        "静态矩形加矩形和是 `3.2.009 RectAddSum` 的同一离线接口域。",
    ),
    "3.4.006": (
        "`data_structure/range_kth_smallest`, `data_structure/static_range_frequency`, `data_structure/static_range_sum_with_upper_bound`",
        "第 k 小、排名和前驱后继均由 `3.4.005 Wavelet` 的公开查询接口提供。",
    ),
    "3.4.007": (
        "`data_structure/static_range_frequency`",
        "该条是同一静态区间频次语义的离线替代方案，当前主候选使用 `Wavelet`。",
    ),
    "3.7.002": (
        "`data_structure/line_add_get_min`",
        "动态开点李超树与 `3.7.001 LiChao` 属于相同直线最小值问题域；当前题面可离线收集坐标，故主候选取后者。",
    ),
    "3.9.007": (
        "`data_structure/ordered_set`",
        "order-statistics 更新策略是 `3.9.001 OSet` 的排名能力来源。",
    ),
    "3.9.008": (
        "`data_structure/ordered_set`",
        "`find_by_order` 与 `order_of_key` 就是该题第 k 小和排名查询所用接口。",
    ),
}


# 这些条目没有在冻结的第 03 章最终范围（47 个 data_structure 题，other 分配为 0）
# 建立独立映射。这里的 N/A 是章节范围结论，不声称其它主责章节不存在同名或可复用的题。
NOT_APPLICABLE_GROUPS = {
    "冻结题集没有要求最近操作回滚或元素删除的并查集语义。": (
        "3.1.004", "3.1.007",
    ),
    "冻结题集没有要求该树状数组变体独有的赋值、区间修改或单向极值接口。": (
        "3.2.002", "3.2.004", "3.2.005", "3.2.006",
    ),
    "冻结题集没有要求该线段树变体独有的迭代、可持久化、合并、分治或二维动态语义。": (
        "3.3.005", "3.3.006", "3.3.008", "3.3.009", "3.3.010", "3.3.015", "3.3.016",
    ),
    "冻结题集没有以该 RMQ、堆式查询或笛卡尔树接口作为第 03 章主责题的语义核心；`tree/cartesian_tree` 由第 06 章负责。": (
        "3.4.002", "3.4.003", "3.4.004",
    ),
    "冻结题集没有要求该平衡树、可删堆或 Top-K 结构的独有公开接口；不能以可替代实现替换已记录的主映射。": (
        "3.5.001", "3.5.002", "3.5.003", "3.5.004", "3.5.005", "3.5.006", "3.5.008", "3.5.009", "3.5.010",
    ),
    "冻结题集没有要求该 Trie、可持久化 Trie、bitset 或 MEX/标记维护的独有语义。": (
        "3.6.001", "3.6.002", "3.6.004", "3.6.005", "3.6.007", "3.6.008", "3.6.010", "3.6.011", "3.6.012", "3.6.013",
    ),
    "冻结题集没有要求该持久化李超、KD-Tree、分块 KD 或析合树接口；它们不能仅凭可替代性计入现有题目。": (
        "3.7.003", "3.7.004", "3.7.005", "3.7.006", "3.7.008",
    ),
    "冻结题集没有以异或线性基及其区间/合并变体作为主责语义。": (
        "3.8.001", "3.8.002", "3.8.003", "3.8.004",
    ),
    "冻结题集没有要求该 PBDS 堆、策略、元数据或哈希接口的独有语义。": (
        "3.9.002", "3.9.003", "3.9.004", "3.9.005", "3.9.006", "3.9.009", "3.9.010", "3.9.011", "3.9.012", "3.9.013",
    ),
    "冻结题集没有以一维或图上根号分块的更新/查询模型作为主责语义。": (
        "3.10.001", "3.10.002", "3.10.003", "3.10.004",
    ),
}

ENTRY_RE = re.compile(r"^### (3\.\d+\.\d{3}) (.+)$", re.MULTILINE)


def sha256_file(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def canonical_sha256(value: object) -> str:
    """返回流程规定的规范化 JSON SHA256。"""
    data = json.dumps(
        value,
        ensure_ascii=False,
        sort_keys=True,
        separators=(",", ":"),
    ).encode("utf-8")
    return hashlib.sha256(data).hexdigest()


def read_json(path: Path) -> dict:
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
        raise RuntimeError(f"cannot read JSON: {path}") from error
    if not isinstance(data, dict):
        raise RuntimeError(f"JSON root is not an object: {path}")
    return data


def load_module(path: Path, name: str):
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot load module: {path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def compact(value: str, limit: int = 96) -> str:
    value = " ".join(value.split())
    return value if len(value) <= limit else value[: limit - 3] + "..."


def code(value: str) -> str:
    return f"`{value}`"


def local_state(
    receipt: dict | None,
    receipt_current: bool,
    name: str,
    source_sha256: str,
) -> str:
    if (
        not receipt_current
        or not isinstance(receipt, dict)
        or receipt.get("status") != "passed"
    ):
        return "`local_pending`"
    records = receipt.get("problems")
    record = records.get(name) if isinstance(records, dict) else None
    if (
        isinstance(record, dict)
        and record.get("status") == "passed"
        and record.get("source_sha256") == source_sha256
    ):
        return "`local_verified`"
    return "`local_pending`"


def performance_state(name: str, source_sha256: str) -> str:
    path = PERFORMANCE_DIR / f"{name}.json"
    if not path.is_file():
        return "`performance_pending`"
    try:
        data = read_json(path)
    except RuntimeError:
        return "`performance_invalid`"
    candidate = data.get("candidate")
    if not isinstance(candidate, dict) or candidate.get("sha256") != source_sha256:
        return "`performance_stale`"
    status = data.get("status")
    ratio = data.get("candidate_over_reference_median")
    if status == "passed" and isinstance(ratio, (int, float)):
        return f"`performance_passed` ({ratio:.3f}x)"
    return "`performance_failed`"


def online_state(submitter, prepared, meta: dict) -> str:
    # prepared 是当前独立源码；只在摘要和匹配的原始提交记录同时成立时显示线上 AC。
    validation = meta.get("validation")
    if not isinstance(validation, dict):
        return "`online_pending`"
    try:
        certified = submitter.already_certified(prepared)
    except submitter.SubmissionError as error:
        raise RuntimeError(f"{prepared.item.name}: cannot certify online state: {error}") from error
    if certified:
        return f"`online_ac` #{validation['current_source_submission_id']}"
    return "`online_pending`"


def template_entry_rows(generator) -> tuple[list[str], list[str]]:
    """返回共享覆盖与真正 N/A 条目的 Markdown 行，并校验两类清单覆盖关系。"""
    templates = generator.template_root(WORKSPACE)
    chapter = generator.read_text(generator.chapter_path(templates))
    headings = dict(ENTRY_RE.findall(chapter))
    if len(headings) != 110:
        raise RuntimeError(f"chapter 03 entry count changed: {len(headings)}")

    primary = {spec.topic for spec in generator.SPECS if spec.topic is not None}
    expected_shared = set(SHARED_ENTRY_COVERAGE)
    not_applicable = {
        entry
        for entries in NOT_APPLICABLE_GROUPS.values()
        for entry in entries
    }
    missing = set(headings) - primary - expected_shared - not_applicable
    duplicate = (primary & expected_shared) | (primary & not_applicable) | (expected_shared & not_applicable)
    if missing or duplicate:
        detail = []
        if missing:
            detail.append("unclassified=" + ", ".join(sorted(missing)))
        if duplicate:
            detail.append("overlapping=" + ", ".join(sorted(duplicate)))
        raise RuntimeError("chapter 03 template-only audit coverage invalid: " + "; ".join(detail))

    def label(entry: str) -> str:
        title = headings[entry].replace("`", "").replace("|", r"\|")
        return f"`{entry}` {title}"

    shared_rows = []
    for entry in sorted(expected_shared):
        targets, reason = SHARED_ENTRY_COVERAGE[entry]
        shared_rows.append(
            f"| {label(entry)} | {targets} | {reason} |"
        )

    evidence = (
        f"冻结 `{generator.OFFICIAL_COMMIT}:data_structure` 的 47 个直属题完整枚举；"
        "`11-其他分类审计.md` 已确认第 03 章分配题数为 0；逐题主映射见上表。"
    )
    not_applicable_rows = []
    for reason, entries in NOT_APPLICABLE_GROUPS.items():
        for entry in entries:
            not_applicable_rows.append(
                f"| {label(entry)} | {reason} | {evidence} |"
            )
    return shared_rows, not_applicable_rows


def render() -> str:
    generator = load_module(GENERATOR, "ch03_audit_generator")
    submitter = load_module(TOOLS / "submit_yosupo.py", "ch03_audit_submitter")
    verifier = load_module(TOOLS / "verify_local.py", "ch03_audit_verifier")
    manifest = read_json(ROOT / "manifest.json")
    entries = {
        entry["name"]: entry
        for entry in manifest.get("problems", [])
        if isinstance(entry, dict) and entry.get("category") == "data_structure"
    }
    specs = {spec.name: spec for spec in generator.SPECS}
    official_names = generator.official_names(WORKSPACE)
    if sorted(entries) != official_names or sorted(specs) != official_names:
        raise RuntimeError("manifest, Specs, and frozen official data_structure names differ")

    receipt = read_json(RECEIPT) if RECEIPT.is_file() else None
    prepared_by_name = {}
    for name in official_names:
        spec = specs[name]
        generated, _ = generator.generate_twice(WORKSPACE, spec)
        target = ROOT / generator.source_path(spec)
        if not target.is_file() or target.read_bytes() != generated:
            raise RuntimeError(f"{name}: current deterministic generation differs from main.cpp")
        prepared = submitter.prepare_sources(
            submitter.load_items(ROOT, manifest, {name}, solution_only=True)
        )
        if len(prepared) != 1:
            raise RuntimeError(f"{name}: cannot prepare current source")
        prepared_by_name[name] = prepared[0]
    try:
        # 第 03 章的状态只由冻结的 47 道 data_structure 题决定；默认收据
        # 仍绑定整个 manifest，但不要求同时覆盖其他章节的候选。
        verifier.require_receipt(ROOT, list(prepared_by_name.values()), manifest)
        receipt_current = True
    except verifier.LocalVerifyError:
        receipt_current = False

    rows = []
    local_verified = 0
    performance_passed = 0
    online_ac = 0
    for name in official_names:
        spec = specs[name]
        entry = entries[name]
        source_path = ROOT / entry["source"]
        source_sha256 = sha256_file(source_path)
        if source_sha256 != entry.get("source_sha256"):
            raise RuntimeError(f"{name}: manifest source SHA256 is stale")
        meta = read_json(ROOT / entry["meta"])
        prepared = prepared_by_name[name]
        if prepared.source_sha256 != source_sha256:
            raise RuntimeError(f"{name}: source preparation SHA256 mismatch")

        topic = code(spec.topic) if spec.topic else code("1.4.002 STL deque")
        api = "<br>".join(code(compact(symbol)) for symbol in spec.symbols)
        if not api:
            api = code("deque<long long>")
        headers = ", ".join(code(header) for header in spec.headers) or code("STL")
        tests = ", ".join(code(Path(test).name) for test in spec.tests)
        reference = meta.get("reference")
        official = meta.get("official_reference")
        reference_path = reference.get("source") if isinstance(reference, dict) else "null"
        official_blob = official.get("git_blob") if isinstance(official, dict) else "null"
        evidence = (
            f"{code('tools/generate_ch03_lc.py --check')}<br>"
            f"{code(str(reference_path))}<br>official correct.cpp blob {code(str(official_blob)[:12])}"
        )
        local = local_state(receipt, receipt_current, name, source_sha256)
        performance = performance_state(name, source_sha256)
        online = online_state(submitter, prepared, meta)
        local_verified += local.startswith("`local_verified`")
        performance_passed += performance.startswith("`performance_passed`")
        online_ac += online.startswith("`online_ac`")
        rows.append(
            "| "
            + " | ".join([
                code(f"data_structure/{name}"),
                f"{topic}<br>{api}",
                f"{headers}<br>{tests}",
                f"{code(entry['source'])}<br>{code(source_sha256)}",
                evidence,
                local,
                performance,
                online,
            ])
            + " |"
        )

    shared_rows, not_applicable_rows = template_entry_rows(generator)
    receipt_sha256 = sha256_file(RECEIPT) if RECEIPT.is_file() else "null"
    return "\n".join([
        "# Chapter 03 Data Structure Audit",
        "",
        "本矩阵以冻结官方题库的 `data_structure/` 直属题集为反向基线。每行的候选均为当前用户模板确定性生成的唯一 `main.cpp`；参考目录和官方 `correct.cpp` 只用于源码对照与本地判定，绝不作为提交候选。",
        "",
        "## Coverage Status",
        "",
        f"固定官方 commit `{generator.OFFICIAL_COMMIT}` 共 `{len(official_names)}` 题：已建立真实模板映射、独立源码和本轮本地收据 `{local_verified}/{len(official_names)}`；性能门 `{performance_passed}/{len(official_names)}`；当前可认证线上 AC `{online_ac}/{len(official_names)}`。`performance_pending` 和 `online_pending` 都表示该题尚未越过对应门禁，不能被报告为完成。",
        "",
        "| LC problem | Template mapping / API | Header / test | Current source | Source evidence | Local state | Performance | Online state |",
        "|---|---|---|---|---|---|---|---|",
        *rows,
        "",
        "## Shared Template Coverage",
        "",
        "下表 11 个条目并非独立生成入口，但正文明确复用或提供了同一官方题所需的公开能力。因此它们不属于模板专属 `not_applicable`，也不新增或替代上方的官方题行。",
        "",
        "| Template entry | Shared official coverage | Review conclusion |",
        "|---|---|---|",
        *shared_rows,
        "",
        "## Template-only `not_applicable`",
        "",
        "本轮最终范围是固定 `data_structure/` 的 47 题，且 `11-其他分类审计.md` 向第 03 章分配 0 题。以下 58 个条目没有在该范围建立独立的、满足题面语义和复杂度的官方映射；它们不会替代或减少上方 47 题基线。",
        "",
        "| Template entry | Reason no dedicated official data_structure problem is used here | Review evidence |",
        "|---|---|---|",
        *not_applicable_rows,
        "",
        "## Frozen Inputs",
        "",
        f"- Official repository: `{generator.OFFICIAL_REPOSITORY_URL}`",
        f"- Official commit: `{generator.OFFICIAL_COMMIT}`",
        f"- Official license: `{generator.OFFICIAL_LICENSE}`, blob `{generator.OFFICIAL_LICENSE_BLOB}`",
        f"- Normalized manifest SHA256: `{canonical_sha256(manifest)}`",
        f"- Local receipt SHA256: `{receipt_sha256}`",
        f"- Deterministic generation: `python library-checker-template-solutions/tools/generate_ch03_lc.py --check`",
        f"- Local verification: `python library-checker-template-solutions/tools/verify_local.py --only <all 47 data_structure names>`",
        "",
        "## Review Notes",
        "",
        "- 全部 47 道题已按题面 I/O、约束、公开接口、区间/复合方向和复杂度与当前模板调用链对照；本轮未发现需要修改算法模板的确定性错误。",
        f"- 当前默认本地收据覆盖 {local_verified}/47 题；每条有效记录均绑定候选编译、关联模板测试和样例/边界/可复现随机三类官方 checker 用例。",
        "- 重点性能风险保留在持久化节点池、排序线段树和高内存 Wavelet 路径；只有同一源码的五轮基准通过后才会写为 `performance_passed` 并进入单题线上提交。",
        "- 历史在线 AC 只有在服务端回显源码 SHA256 与当前 `main.cpp` 相同、最终 verdict 为 AC 且 `is_latest=true` 时才写为 `online_ac`。",
        "",
    ])


def main() -> None:
    parser = argparse.ArgumentParser(description="Refresh the Chapter 03 audit matrix")
    parser.add_argument("--check", action="store_true", help="fail if the rendered audit differs")
    args = parser.parse_args()
    text = render()
    if args.check:
        if not AUDIT.is_file() or AUDIT.read_text(encoding="utf-8") != text:
            raise SystemExit("docs/03-数据结构审计.md is stale")
        print("AUDIT MATCHED")
        return
    AUDIT.write_text(text, encoding="utf-8", newline="\n")
    print(f"UPDATED {AUDIT}")


if __name__ == "__main__":
    main()
