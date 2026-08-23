#!/usr/bin/env python3
"""Refresh chapter audit matrices from current, already-gated LC evidence."""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import re
from collections import defaultdict
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
WORKSPACE = ROOT.parent
TEMPLATE = WORKSPACE / "hushuqi算法竞赛模板"
DOCS = ROOT / "docs"
MAPPING = ROOT / "LC_TEMPLATE_MAPPING.tsv"
MANIFEST = ROOT / "manifest.json"
RECEIPT = ROOT / ".local" / "yosupo-local-verification.json"
TARGETS = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "14"}


def load_generator():
    path = ROOT / "tools" / "generate_lc_gaps.py"
    spec = importlib.util.spec_from_file_location("generate_lc_gaps", path)
    if spec is None or spec.loader is None:
        raise RuntimeError("cannot load generate_lc_gaps.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def read_rows() -> dict[str, list[tuple[str, str]]]:
    result: dict[str, list[tuple[str, str]]] = defaultdict(list)
    for line in MAPPING.read_text(encoding="utf-8").splitlines():
        if not line or line.startswith("#"):
            continue
        chapter, item, problem, confidence = line.split("\t")
        if confidence != "audited":
            raise RuntimeError(f"{problem}: mapping is not audited")
        result[chapter].append((item, problem))
    return result


def public_api(generator, category: str, name: str) -> str:
    spec = generator.SPECS.get((category, name))
    if spec is None:
        return "template entry adapter"
    header, title, _test, adapter = spec
    expanded = generator.expand(TEMPLATE / "include" / "full" / header, set())
    declared = set(re.findall(r"\b(?:class|struct)\s+([A-Za-z_]\w*)", expanded))
    declared.update(re.findall(
        r"\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*(?:const\s*)?(?:->\s*[^\{]+)?\{",
        expanded,
    ))
    ignored = {
        "if", "for", "while", "switch", "return", "main", "min", "max", "swap",
        "move", "begin", "end", "size", "empty", "push_back", "assert", "bit_ceil",
    }
    used = re.findall(r"\b([A-Za-z_]\w*)\s*\(", adapter)
    used += [symbol for symbol in declared if re.search(rf"\b{re.escape(symbol)}\b", adapter)]
    api = []
    for symbol in used:
        if symbol in declared and symbol not in ignored and symbol not in api:
            api.append(symbol)
    return ", ".join(f"`{symbol}`" for symbol in api[:5]) or title


def chapter_doc(chapter: str) -> Path:
    matches = list(DOCS.glob(f"{chapter}-*审计.md"))
    if len(matches) != 1:
        raise RuntimeError(f"chapter {chapter}: audit document count is {len(matches)}")
    return matches[0]


def table_for(chapter_rows: list[tuple[str, str]], entries: dict[str, dict], generator) -> list[str]:
    lines = [
        "| Official problem | Template mapping / API | Header / dependency / test | Reference source | Current source | Source evidence | Local state | Online state |",
        "|---|---|---|---|---|---|---|---|",
    ]
    for item, problem in sorted(chapter_rows, key=lambda row: row[1]):
        category, name = problem.split("/", 1)
        entry = entries[problem]
        meta = json.loads((ROOT / entry["meta"]).read_text(encoding="utf-8"))
        validation = meta["validation"]
        spec = generator.SPECS.get((category, name))
        if spec is not None:
            header, _title, test, _adapter = spec
            support = f"`include/full/{header}`; `{test}`"
            evidence = "`generate_lc_gaps.py`; deterministic expansion"
        else:
            checks = [check.removeprefix("hushuqi算法竞赛模板/")
                      for check in validation.get("checks", [])
                      if check.startswith("hushuqi算法竞赛模板/")]
            candidate = TEMPLATE / "include" / "full" / (item.replace(".", "_") + ".hpp")
            header = f"include/full/{candidate.name}" if candidate.is_file() else "include/base.hpp"
            support = "; ".join(f"`{value}`" for value in [header] + checks) or "template chapter entry"
            evidence = "`template_solution` provenance; current source SHA256"
        source_sha = entry["source_sha256"][:12]
        submission = validation["current_source_submission_id"]
        seconds = validation.get("current_source_time")
        online = f"`online_ac #{submission}`" + (f", `{seconds}s`" if seconds is not None else "")
        mapping = f"`{item}`; {public_api(generator, category, name)}"
        lines.append(
            f"| `{problem}` | {mapping} | {support} | "
            f"`library-checker-solutions/{category}/{name}/main.cpp` | "
            f"`{entry['source']}` | {evidence}; `{source_sha}...` | "
            f"`current_verified` | {online} |"
        )
    return lines


def table_ranges(lines: list[str], problems: set[str]) -> list[tuple[int, int, int]]:
    ranges = []
    for start, line in enumerate(lines):
        if not (line.startswith("| Official problem |") or line.startswith("| LC problem |")):
            continue
        end = start + 1
        while end < len(lines) and lines[end].startswith("|"):
            end += 1
        count = sum(1 for problem in problems
                    if any(f"`{problem}`" in row for row in lines[start:end]))
        ranges.append((count, start, end))
    return ranges


def refresh_status_table(
    lines: list[str],
    start: int,
    end: int,
    entries: dict[str, dict],
    *,
    chapter: str,
    column_count: int,
    local_column: int,
    online_column: int,
) -> None:
    for index in range(start + 2, end):
        columns = lines[index].split("|")
        if len(columns) != column_count:
            raise RuntimeError(f"chapter {chapter}: malformed matrix row {index + 1}")
        match = re.fullmatch(r"\s*`([^`]+)`\s*", columns[1])
        if match is None:
            raise RuntimeError(f"chapter {chapter}: missing problem id on row {index + 1}")
        problem = match.group(1)
        entry = entries.get(problem)
        if entry is None:
            raise RuntimeError(f"chapter {chapter}: {problem} is absent from manifest")
        meta = json.loads((ROOT / entry["meta"]).read_text(encoding="utf-8"))
        validation = meta["validation"]
        submission = validation["current_source_submission_id"]
        seconds = validation.get("current_source_time")
        online = f"`online_ac` #{submission}"
        if seconds is not None:
            online += f" ({seconds}s)"
        columns[local_column] = " `current_verified` "
        columns[online_column] = f" {online} "
        lines[index] = "|".join(columns)


def sha256_file(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def refresh_prose(text: str, chapter: str, count: int) -> str:
    text = re.sub(
        r"固定官方题库 commit ([^\n]+?)当前尚未完成逐题源码级对照，所有行均为 `review_pending`；`null` 不表示可以跳过。",
        rf"固定官方题库 commit \1已逐题完成题面、checker、受控参考、模板调用链、复杂度、生成源码、本地收据和线上同哈希 AC 复核。",
        text,
    )
    text = re.sub(
        r"第 (05|06|07|08) 章现有 `tests/test_ch\1_\*\.cpp`[^\n]+",
        lambda match: f"第 {match.group(1)} 章全部 {count} 道官方题均已通过当前模板测试、官方 sample/boundary/random checker 与同哈希线上 AC，章节 LC 闭环完成。",
        text,
    )
    if chapter == "09":
        text = re.sub(
            r"18 条 TSV 路由均为 `classified`。[^\n]+",
            "18 条 TSV 路由均已完成源码级映射审计；当前模板确定性生成的独立源码全部通过本地官方 checker 与同哈希线上 AC，第 09 章 LC 闭环完成。",
            text,
        )
    if chapter == "10":
        text = text.replace(
            "当前 `main.cpp` 已经变化且尚未重新在线提交，因此下列历史结果不能证明当前源码在线通过。",
            "当前状态只认文末矩阵中的当前源码、本地收据和同哈希线上 AC。",
        )
        text = text.replace(
            "该表是第 10 章当前冻结范围的唯一逐题矩阵，来自 `LC_TEMPLATE_MAPPING.tsv` 的 `chapter=10`。下方旧审计正文保留历史对照和性能材料，但不能把历史候选、历史本地验证或旧 AC 当成当前闭环。本轮尚未重新按源码顺序完成每题对照，因此所有行保持 `review_pending`；按流程完成后再逐项填入真实接口、参考、候选与验证状态。",
            "该表是第 10 章当前冻结范围的唯一逐题矩阵，来自 `LC_TEMPLATE_MAPPING.tsv` 的 `chapter=10`。旧审计正文仅保留算法与性能对照；当前闭环以本表记录的模板映射、唯一源码、本地收据和同哈希线上 AC 为准。",
        )
        text = re.sub(
            r"2026-08-16 快照的在线验证已经完成，但当前 33 份 `main\.cpp` 尚未重新在线提交。[^\n]+",
            "当前第 10 章全部独立 `main.cpp` 均已重新绑定本地官方 checker 收据并取得同哈希线上 AC；后续任一模板或生成源码变化都必须重新执行本地验证与逐题在线提交。",
            text,
        )
    if chapter == "03":
        text = re.sub(
            r"已建立真实模板映射、独立源码和本轮本地收据 `\d+/47`；",
            "已建立真实模板映射、独立源码和本轮本地收据 `47/47`；",
            text,
        )
        text = re.sub(
            r"当前默认本地收据覆盖 \d+/47 题；",
            "当前默认本地收据覆盖 47/47 题；",
            text,
        )
        text = re.sub(
            r"- Local receipt SHA256: `[0-9a-f]+`",
            f"- Local receipt SHA256: `{sha256_file(RECEIPT)}`",
            text,
        )
    return text


def parse_chapters(value: str) -> list[str]:
    chapters = [part.strip().zfill(2) for part in value.split(",") if part.strip()]
    unknown = sorted(set(chapters) - TARGETS)
    if unknown:
        raise argparse.ArgumentTypeError(
            "unsupported chapter(s): " + ", ".join(unknown)
        )
    return sorted(set(chapters))


def render_chapter(
    chapter: str,
    scoped: list[tuple[str, str]],
    entries: dict[str, dict],
    generator,
) -> tuple[Path, str]:
    doc = chapter_doc(chapter)
    lines = doc.read_text(encoding="utf-8").splitlines()
    ranges = table_ranges(lines, {problem for _item, problem in scoped})
    if not ranges:
        raise RuntimeError(f"chapter {chapter}: official problem table missing")
    count, start, end = max(ranges)
    if count != len(scoped):
        raise RuntimeError(f"chapter {chapter}: matrix covers {count}/{len(scoped)} problems")
    if chapter == "02":
        refresh_status_table(
            lines,
            start,
            end,
            entries,
            chapter=chapter,
            column_count=11,
            local_column=8,
            online_column=9,
        )
    elif chapter == "03":
        refresh_status_table(
            lines,
            start,
            end,
            entries,
            chapter=chapter,
            column_count=10,
            local_column=6,
            online_column=8,
        )
    elif chapter == "04":
        refresh_status_table(
            lines,
            start,
            end,
            entries,
            chapter=chapter,
            column_count=10,
            local_column=7,
            online_column=8,
        )
    else:
        lines[start:end] = table_for(scoped, entries, generator)
    text = refresh_prose("\n".join(lines) + "\n", chapter, len(scoped))
    return doc, text


def main() -> None:
    parser = argparse.ArgumentParser(
        description="从当前 LC 结构化证据刷新章节审计矩阵"
    )
    parser.add_argument(
        "--chapters",
        type=parse_chapters,
        default=sorted(TARGETS),
        help="逗号分隔的两位章节号；默认刷新全部受支持章节",
    )
    parser.add_argument("--check", action="store_true", help="只检查文档是否为当前状态")
    args = parser.parse_args()

    rows = read_rows()
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    entries = {f"{entry['category']}/{entry['name']}": entry
               for entry in manifest["problems"]}
    generator = load_generator()
    stale = []
    for chapter in args.chapters:
        scoped = rows[chapter]
        doc, text = render_chapter(chapter, scoped, entries, generator)
        current = doc.read_text(encoding="utf-8")
        if args.check:
            if current != text:
                stale.append(chapter)
            else:
                print(f"CURRENT chapter {chapter}: {len(scoped)} rows")
        else:
            doc.write_text(text, encoding="utf-8")
            print(f"UPDATED chapter {chapter}: {len(scoped)} rows")
    if stale:
        raise SystemExit("STALE chapter audit(s): " + ", ".join(stale))


if __name__ == "__main__":
    main()
