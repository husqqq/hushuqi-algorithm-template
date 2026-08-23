#!/usr/bin/env python3
"""Validate that the LC routing table gives every frozen official problem one owner."""

from __future__ import annotations

import re
import subprocess
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
PROBLEMS = ROOT / ".local" / "library-checker-problems"
TEMPLATE_ROOT = ROOT.parent / "hushuqi算法竞赛模板"
COMMIT = "04c8de378bab67be926325de2871f0babb8e6451"
MAPPING = ROOT / "LC_TEMPLATE_MAPPING.tsv"
ID_RE = re.compile(r"^[a-z_]+/[A-Za-z0-9_]+$")
ITEM_RE = re.compile(r"^(?:[1-9]|1[0-4])\.\d{1,2}\.\d{3}$")
OWNER = {
    "sample": "01", "data_structure": "03", "string": "04", "graph": "05", "tree": "06",
    "number_theory": "07", "enumerative_combinatorics": "08", "linear_algebra": "09",
    "convolution": "10", "polynomial": "10", "set_power_series": "10", "geo": "11",
    "big_integer": "14",
}
OTHER_OWNER = {
    "consecutive_terms_of_linear_recurrent_sequence": "10", "find_linear_recurrence": "10",
    "kth_term_of_linearly_recurrent_sequence": "10", "longest_increasing_subsequence": "02",
    "sum_of_exponential_times_polynomial": "10", "sum_of_exponential_times_polynomial_limit": "10",
    "two_sat": "05",
}


def official_ids() -> set[str]:
    cmd = ["git", "-C", str(PROBLEMS), "ls-tree", "-r", "--name-only", COMMIT]
    result = subprocess.run(cmd, check=True, text=True, capture_output=True)
    ids: set[str] = set()
    for path in result.stdout.splitlines():
        parts = path.split("/")
        if len(parts) == 3 and parts[2] == "info.toml" and parts[0] != "test":
            ids.add(f"{parts[0]}/{parts[1]}")
    return ids


def mapping_rows() -> list[tuple[str, str, str, int]]:
    rows = []
    for line_no, raw in enumerate(MAPPING.read_text(encoding="utf-8").splitlines(), 1):
        if not raw or raw.startswith("#"):
            continue
        fields = raw.split("\t")
        if len(fields) != 4:
            raise ValueError(f"{MAPPING.name}:{line_no}: expected four tab-separated fields")
        chapter, item, problem, confidence = fields
        if not re.fullmatch(r"(?:0[1-9]|1[0-4])", chapter):
            raise ValueError(f"{MAPPING.name}:{line_no}: invalid chapter {chapter!r}")
        # chapter is the workflow owner; the actual reusable interface may live in another chapter.
        if not ITEM_RE.fullmatch(item):
            raise ValueError(f"{MAPPING.name}:{line_no}: invalid template item {item!r}")
        if not ID_RE.fullmatch(problem) or confidence not in {"audited", "classified"}:
            raise ValueError(f"{MAPPING.name}:{line_no}: invalid problem or confidence")
        rows.append((chapter, item, problem, line_no))
    return rows


def gap_rows() -> list[str]:
    # 缺口只在实际存在时建档；当前冻结映射没有缺口文件。
    return []


def template_items() -> set[str]:
    items: set[str] = set()
    for path in TEMPLATE_ROOT.glob("[0-9][0-9]-*.md"):
        items.update(re.findall(r"^### (\d+\.\d+\.\d+)\b", path.read_text(encoding="utf-8"), re.MULTILINE))
    return items


def expected_owner(problem: str) -> str:
    category, name = problem.split("/", 1)
    return OTHER_OWNER[name] if category == "other" else OWNER[category]


def main() -> int:
    if not PROBLEMS.is_dir():
        print(f"FAIL: frozen official checkout not found: {PROBLEMS}")
        return 2
    try:
        rows = mapping_rows()
    except ValueError as exc:
        print(f"FAIL: {exc}")
        return 2
    mapped = [row[2] for row in rows]
    gaps = gap_rows()
    all_claims = mapped + gaps
    duplicates = sorted(problem for problem, count in Counter(all_claims).items() if count > 1)
    unknown_items = sorted({row[1] for row in rows} - template_items())
    owner_mismatches = sorted(
        f"{problem} -> {chapter} (expected {expected_owner(problem)})"
        for chapter, _, problem, _ in rows
        if expected_owner(problem) != chapter
    )
    expected = official_ids()
    actual = set(all_claims)
    missing = sorted(expected - actual)
    unknown = sorted(actual - expected)
    if duplicates or missing or unknown or unknown_items or owner_mismatches:
        if duplicates:
            print("FAIL duplicate LC IDs:", ", ".join(duplicates))
        if missing:
            print("FAIL missing LC IDs:", ", ".join(missing))
        if unknown:
            print("FAIL IDs absent from frozen official set:", ", ".join(unknown))
        if unknown_items:
            print("FAIL template items absent from template Markdown:", ", ".join(unknown_items))
        if owner_mismatches:
            print("FAIL workflow-owner mismatches:", ", ".join(owner_mismatches))
        return 1
    print(
        f"PASS: {len(expected)} official problems; {len(mapped)} mapped; {len(gaps)} confirmed gaps; "
        f"{len(rows)} unique template assignments."
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
