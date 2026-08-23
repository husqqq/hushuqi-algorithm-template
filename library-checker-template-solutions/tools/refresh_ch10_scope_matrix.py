#!/usr/bin/env python3
"""Keep Chapter 10's authoritative LC scope matrix separate from old notes."""

from __future__ import annotations

import argparse
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
MAPPING = ROOT / "LC_TEMPLATE_MAPPING.tsv"
AUDIT = ROOT / "docs" / "10-多项式审计.md"
START = "<!-- CH10_SCOPE_MATRIX_START -->"
END = "<!-- CH10_SCOPE_MATRIX_END -->"


def scoped_rows() -> list[tuple[str, str, str]]:
    rows = []
    for line_no, line in enumerate(MAPPING.read_text(encoding="utf-8").splitlines(), 1):
        if not line or line.startswith("#"):
            continue
        fields = line.split("\t")
        if len(fields) != 4:
            raise ValueError(f"{MAPPING}:{line_no}: TSV 必须是四列")
        chapter, item, problem, confidence = fields
        if chapter == "10":
            rows.append((item, problem, confidence))
    if len(rows) != len({problem for _, problem, _ in rows}):
        raise ValueError("第 10 章 TSV 出现重复官方题")
    return sorted(rows, key=lambda row: row[1])


def block() -> str:
    rows = scoped_rows()
    text = [
        START,
        "## 当前官方覆盖矩阵",
        "",
        "该表是第 10 章当前冻结范围的唯一逐题矩阵，来自 `LC_TEMPLATE_MAPPING.tsv` 的 `chapter=10`。"
        "下方旧审计正文保留历史对照和性能材料，但不能把历史候选、历史本地验证或旧 AC 当成当前闭环。"
        "本轮尚未重新按源码顺序完成每题对照，因此所有行保持 `review_pending`；按流程完成后再逐项填入真实接口、参考、候选与验证状态。",
        "",
        "| Official problem | State | Template mapping / API | Header / dependency / test | Reference source | Current source | Source evidence | Local state | Online state |",
        "|---|---|---|---|---|---|---|---|---|",
    ]
    for _, problem, _ in rows:
        text.append(
            f"| `{problem}` | `review_pending` | `null` | `null` | `null` | `null` | `null` | `null` | `null` |"
        )
    text.extend([END, ""])
    return "\n".join(text)


def render() -> str:
    original = AUDIT.read_text(encoding="utf-8")
    start = original.find(START)
    end = original.find(END)
    if start == -1 and end == -1:
        return original.rstrip() + "\n\n" + block()
    if start == -1 or end == -1 or end < start:
        raise ValueError("第 10 章覆盖矩阵标记不完整")
    end += len(END)
    return original[:start] + block().rstrip() + original[end:]


def main() -> None:
    parser = argparse.ArgumentParser(description="刷新第 10 章官方范围矩阵")
    parser.add_argument("--check", action="store_true", help="只核验，不写入文件")
    args = parser.parse_args()
    text = render()
    if args.check:
        if AUDIT.read_text(encoding="utf-8") != text:
            raise SystemExit("docs/10-多项式审计.md 的当前范围矩阵已过期")
        print("CH10_SCOPE_MATRIX_CURRENT")
        return
    AUDIT.write_text(text, encoding="utf-8")
    print(f"UPDATED {AUDIT}")


if __name__ == "__main__":
    main()
