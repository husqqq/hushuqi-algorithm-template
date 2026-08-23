#!/usr/bin/env python3
"""Render a read-only, TSV-scoped evidence snapshot for all template chapters."""

from __future__ import annotations

import argparse
import hashlib
import json
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
WORKSPACE = ROOT.parent
MAPPING = ROOT / "LC_TEMPLATE_MAPPING.tsv"
MANIFEST = ROOT / "manifest.json"
RECEIPT = ROOT / ".local" / "yosupo-local-verification.json"
SCOPED_RECEIPTS = ROOT / ".local" / "scoped-receipts"
DOCS = ROOT / "docs"
OUT = DOCS / "96-全章节审计快照.md"
COMMIT = "04c8de378bab67be926325de2871f0babb8e6451"


def file_sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def canonical_json_sha256(value: object) -> str:
    data = json.dumps(value, ensure_ascii=False, sort_keys=True,
                      separators=(",", ":")).encode("utf-8")
    return hashlib.sha256(data).hexdigest()


def read_json(path: Path) -> dict:
    value = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(value, dict):
        raise ValueError(f"{path}: JSON 根节点必须是对象")
    return value


def read_mapping() -> list[dict[str, str]]:
    rows = []
    for line_no, line in enumerate(MAPPING.read_text(encoding="utf-8").splitlines(), 1):
        if not line or line.startswith("#"):
            continue
        fields = line.split("\t")
        if len(fields) != 4:
            raise ValueError(f"{MAPPING}:{line_no}: TSV 必须是四列")
        chapter, item, problem, confidence = fields
        rows.append({"chapter": chapter, "item": item,
                     "problem": problem, "confidence": confidence})
    return rows


def chapter_doc(chapter: str) -> Path:
    matches = list(DOCS.glob(f"{chapter}-*审计.md"))
    if len(matches) != 1:
        raise ValueError(f"第 {chapter} 章审计文档数量异常：{len(matches)}")
    return matches[0]


def source_entries(manifest: dict) -> dict[str, dict]:
    result = {}
    for entry in manifest.get("problems", []):
        if not isinstance(entry, dict):
            raise ValueError("manifest.problems 含非对象条目")
        category, name = entry.get("category"), entry.get("name")
        if not isinstance(category, str) or not isinstance(name, str):
            raise ValueError("manifest 条目缺少 category/name")
        problem = f"{category}/{name}"
        if problem in result:
            raise ValueError(f"manifest 重复官方题：{problem}")
        result[problem] = entry
    return result


def receipt_records_from(path: Path, manifest: dict) -> dict[str, dict]:
    """Return current records from one full or single-problem local receipt."""
    if not path.is_file():
        return {}
    receipt = read_json(path)
    if (receipt.get("status") != "passed"
            or receipt.get("official_repository") != {"commit": COMMIT}
            or receipt.get("manifest_sha256") != canonical_json_sha256(manifest)):
        return {}
    records = receipt.get("problems")
    return records if isinstance(records, dict) else {}


def receipt_records(manifest: dict) -> dict[str, dict]:
    """Combine the default receipt with named scoped receipts for their own item."""
    records = receipt_records_from(RECEIPT, manifest)
    if not SCOPED_RECEIPTS.is_dir():
        return records
    for path in sorted(SCOPED_RECEIPTS.glob("*.json")):
        name = path.stem
        scoped = receipt_records_from(path, manifest)
        record = scoped.get(name)
        if isinstance(record, dict):
            records[name] = record
    return records


def row_state(problem: str, entries: dict[str, dict], receipts: dict[str, dict]) -> tuple[bool, bool, bool, bool]:
    """Return reference, current source, current receipt and current online-AC flags."""
    category, name = problem.split("/", 1)
    reference = (WORKSPACE / "library-checker-solutions" / category / name / "main.cpp").is_file()
    entry = entries.get(problem)
    if entry is None:
        return reference, False, False, False
    source = entry.get("source")
    source_sha = entry.get("source_sha256")
    source_path = ROOT / source if isinstance(source, str) else None
    current = bool(source_path and source_path.is_file() and isinstance(source_sha, str)
                   and file_sha256(source_path) == source_sha)
    record = receipts.get(name)
    local = bool(current and isinstance(record, dict) and record.get("status") == "passed"
                 and record.get("official_commit") == COMMIT
                 and record.get("source_sha256") == source_sha)
    meta_path = ROOT / entry.get("meta", "")
    meta = read_json(meta_path) if meta_path.is_file() else {}
    validation = meta.get("validation") if isinstance(meta, dict) else None
    online = bool(current and isinstance(validation, dict)
                  and validation.get("status") == "online_ac"
                  and validation.get("current_source_sha256") == source_sha
                  and validation.get("current_source_online_verdict") == "AC"
                  and validation.get("current_source_is_latest") is True
                  and validation.get("server_source_sha256") == source_sha
                  and isinstance(validation.get("current_source_submission_id"), int))
    return reference, current, local, online


def render() -> str:
    manifest = read_json(MANIFEST)
    if manifest.get("official_repository", {}).get("commit") != COMMIT:
        raise ValueError("manifest 官方题库 commit 不匹配")
    rows = read_mapping()
    entries = source_entries(manifest)
    receipts = receipt_records(manifest)
    by_chapter: dict[str, list[dict[str, str]]] = {
        f"{i:02d}": [] for i in range(1, 15)
    }
    for row in rows:
        by_chapter.setdefault(row["chapter"], []).append(row)

    text = [
        "# 全章节审计快照",
        "",
        "本快照由 `tools/audit_all_chapters.py` 从 `LC_TEMPLATE_MAPPING.tsv`、当前 "
        "manifest、逐题元数据、当前全量或单题本地收据、参考目录和章节审计文档重新计算。它只报告当前证据，"
        "不把候选数量或历史提交误写为章节 LC 闭环；当前本地验证与同哈希线上 AC 均可独立作为正确性证据。",
        "",
        f"- 官方题库 commit：`{COMMIT}`",
        f"- TSV 路由题数：`{len(rows)}`",
        f"- 当前 manifest 候选数：`{len(entries)}`",
        f"- 可用本地收据记录：`{len(receipts)}`",
        "",
        "| 章节 | TSV 范围 | 映射已审计 | 受控参考 | 当前候选 | 当前本地收据 | 当前线上 AC | 可置信验证 | 唯一标识矩阵 | 结论 |",
        "|---|---:|---:|---:|---:|---:|---:|---:|---:|---|",
    ]
    complete = 0
    trusted_total = 0
    for chapter, scoped in by_chapter.items():
        doc = chapter_doc(chapter)
        doc_text = doc.read_text(encoding="utf-8")
        states = [row_state(row["problem"], entries, receipts) for row in scoped]
        reference = sum(state[0] for state in states)
        source = sum(state[1] for state in states)
        local = sum(state[2] for state in states)
        online = sum(state[3] for state in states)
        verified = sum(state[2] or state[3] for state in states)
        mapped = sum(row["confidence"] == "audited" for row in scoped)
        # The workflow requires the full official identifier in the matrix.
        matrix = sum(f"`{row['problem']}`" in doc_text for row in scoped)
        if not scoped:
            conclusion = "无主责 LC 题；模板与测试仍须持续审阅"
        elif (reference == len(scoped) and source == len(scoped)
              and verified == len(scoped) and mapped == len(scoped)):
            conclusion = "章节 LC 闭环完成"
            complete += 1
        else:
            conclusion = "未完成，详见不可完全置信清单"
        trusted_total += sum(
            row["confidence"] == "audited" and all(state[:2])
            and (state[2] or state[3])
            for row, state in zip(scoped, states)
        )
        text.append(
            f"| {chapter} | {len(scoped)} | {mapped} | {reference} | {source} | "
            f"{local} | {online} | {verified} | {matrix} | {conclusion} |"
        )
    text.extend([
        "",
        "## 审计结论",
        "",
        f"1. 当前 `{trusted_total}/{len(rows)}` 道官方题均已通过映射审计、受控参考、当前候选，"
        "并至少具备当前本地验证或同哈希线上 AC 之一；"
        f"共有 `{complete}` 个含 LC 题的章节达到闭环标准。",
        "2. 第 12、13 章的 TSV 范围为零，不得据此跳过模板正文、公共依赖和章节测试审阅。",
        "3. `唯一标识矩阵` 只统计章节文档中按流程要求以反引号写出的完整 ``<category>/<problem>``；"
        "低于 TSV 范围时，文档虽可能提到裸题名，却不能作为可机械核验的逐题覆盖矩阵。",
        "4. 本文件是总览快照，不能替代各章逐题源码审阅、模板订正、生成、本地验证、性能门和单题线上提交。",
        "",
        "运行 `python library-checker-template-solutions/tools/refresh_untrusted_inventory.py` "
        "刷新逐题未完成原因；若全部可信，该工具会删除空清单目录。运行本工具的 `--check` 可验证快照未过期。",
    ])
    return "\n".join(text) + "\n"


def main() -> None:
    parser = argparse.ArgumentParser(description="生成或核验全章节 LC 审计快照")
    parser.add_argument("--check", action="store_true", help="只核验快照，不写入文件")
    args = parser.parse_args()
    text = render()
    if args.check:
        if not OUT.is_file() or OUT.read_text(encoding="utf-8") != text:
            raise SystemExit("全章节审计快照过期，请运行 audit_all_chapters.py 刷新")
        print("ALL_CHAPTER_AUDIT_CURRENT")
        return
    OUT.write_text(text, encoding="utf-8")
    print(f"UPDATED {OUT}")


if __name__ == "__main__":
    main()
