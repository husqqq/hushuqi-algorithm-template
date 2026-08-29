#!/usr/bin/env python3
"""Generate the copy-oriented header branch and update printed chapter code."""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
INCLUDE = ROOT / "include"
FULL = INCLUDE / "full"
PRINT = INCLUDE / "print"
SUPPORT = PRINT / "support"
CHAPTERS = sorted(ROOT.glob("[0-9][0-9]-*.md"))

# Fixed machine words, natural overflow, an external ABI, or a fixed bit layout.
FIXED_TOPICS = {
    "1.1.006", "1.1.008",
    "14.1.019",
}
FIXED_HEADERS = {topic.replace(".", "_") + ".hpp" for topic in FIXED_TOPICS}
FIXED_SUPPORT = {
    "base.hpp",                 # SplitMix64 and the two-word i128 fallback
    "engineering.hpp",          # exact mt19937 checksum
    "linear_algebra_field.hpp", # 32-bit Montgomery reduction
}
HEADING = re.compile(r"^###\s+(\d+\.\d+\.\d{3})\s+", re.MULTILINE)
FIXED_WIDTH = re.compile(r"\b(?:u?int(?:8|16|32|64)_t)\b")
BIT_FUNCTIONS = (
    "countr_zero", "countl_zero", "popcount", "bit_width", "bit_floor",
    "bit_ceil", "has_single_bit", "rotl", "rotr",
)


def replace_general_types(text: str) -> str:
    """Apply the print rules outside explicitly fixed-width implementations."""
    # The C++20 bit functions require an unsigned argument. countl_zero also
    # observes the exact width, so retain an explicitly requested 32-bit cast.
    protected_lines = []
    for line in text.splitlines(keepends=True):
        if any(name in line for name in BIT_FUNCTIONS):
            line = line.replace("(uint32_t)", "(__PRINT_UINT32_CAST__)")
        protected_lines.append(line)
    text = "".join(protected_lines)
    # In these implementations uint64_t is a widening intermediate. Keep that
    # widening visible when the int macro is disabled.
    text = re.sub(r"\buint64_t\s*\(", "(unsigned long long)(", text)
    replacements = {
        "uint64_t": "unsigned long long",
        "int64_t": "int",
        "uint32_t": "int",
        "int32_t": "int",
        "uint16_t": "int",
        "int16_t": "int",
        "uint8_t": "int",
        "int8_t": "int",
        "u64": "int",
        "i64": "int",
        "u32": "int",
        "i32": "int",
    }
    for old, new in replacements.items():
        text = re.sub(rf"\b{old}\b", new, text)
    text = re.sub(r"\b(?:UINT32_MAX|INT32_MAX)\b", "numeric_limits<int>::max()", text)
    text = re.sub(r"\bint\(0\)", "(int)0", text)
    text = text.replace("__PRINT_UINT32_CAST__", "uint32_t")
    return text


def transform_header(text: str, name: str, *, support: bool = False) -> str:
    if (support and name in FIXED_SUPPORT) or (not support and name in FIXED_HEADERS):
        out = text
    elif name == "13_1_009.hpp":
        out = replace_general_types(text)
    elif name == "10_1_004.hpp":
        begin = "// LC_GENERATOR_CH14_ONLY_BEGIN"
        end = "// LC_GENERATOR_CH14_ONLY_END"
        if begin not in text or end not in text:
            raise ValueError("10_1_004.hpp lost its fixed-width Montgomery markers")
        left, rest = text.split(begin, 1)
        fixed, right = rest.split(end, 1)
        out = replace_general_types(left) + begin + fixed + end + replace_general_types(right)
    else:
        out = replace_general_types(text)
    if name == "10_1_011.hpp":
        out = out.replace("popcount(s)", "popcount((unsigned long long)s)")

    if support:
        out = re.sub(r'#include "full/([^"]+)"', r'#include "../\1"', out)
    else:
        out = re.sub(r'#include "\.\./([^"]+)"', r'#include "support/\1"', out)
    return out


def transform_chapter(text: str) -> str:
    matches = list(HEADING.finditer(text))
    if not matches:
        return text
    pieces = [text[: matches[0].start()]]
    for i, match in enumerate(matches):
        end = matches[i + 1].start() if i + 1 < len(matches) else len(text)
        section = text[match.start() : end]
        topic = match.group(1)
        if topic not in FIXED_TOPICS:
            section = replace_general_types(section)
            if topic == "10.1.011":
                section = section.replace("popcount(s)", "popcount((unsigned long long)s)")
        pieces.append(section)
    return "".join(pieces)


def expected_files() -> dict[Path, str]:
    files: dict[Path, str] = {}
    for source in sorted(FULL.glob("*.hpp")):
        files[PRINT / source.name] = transform_header(
            source.read_text(encoding="utf-8"), source.name
        )
    for source in sorted(INCLUDE.glob("*.hpp")):
        files[SUPPORT / source.name] = transform_header(
            source.read_text(encoding="utf-8"), source.name, support=True
        )
    return files


def write_if_changed(path: Path, text: str) -> bool:
    old = path.read_text(encoding="utf-8") if path.exists() else None
    if old == text:
        return False
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding="utf-8", newline="")
    return True


def check() -> int:
    errors: list[str] = []
    expected = expected_files()
    actual = set(PRINT.rglob("*.hpp")) if PRINT.exists() else set()
    for path in sorted(set(expected) - actual):
        errors.append(f"missing: {path.relative_to(ROOT)}")
    for path in sorted(actual - set(expected)):
        errors.append(f"extra: {path.relative_to(ROOT)}")
    for path, wanted in expected.items():
        if path.exists() and path.read_text(encoding="utf-8") != wanted:
            errors.append(f"stale: {path.relative_to(ROOT)}")
    for chapter in CHAPTERS:
        current = chapter.read_text(encoding="utf-8")
        if transform_chapter(current) != current:
            errors.append(f"stale chapter types: {chapter.name}")
    forbidden_alias = re.compile(r"\b(?:u32|i32|u64|i64)\b")
    for path in sorted(actual):
        text = path.read_text(encoding="utf-8")
        is_support = path.parent == SUPPORT
        if (is_support and path.name in FIXED_SUPPORT) or (
            not is_support and path.name in FIXED_HEADERS
        ):
            continue
        if forbidden_alias.search(text):
            errors.append(f"forbidden integer alias: {path.relative_to(ROOT)}")
        if path.name == "10_1_004.hpp":
            begin = "// LC_GENERATOR_CH14_ONLY_BEGIN"
            end = "// LC_GENERATOR_CH14_ONLY_END"
            left, rest = text.split(begin, 1)
            _, right = rest.split(end, 1)
            text = left + right
        for line_number, line in enumerate(text.splitlines(), 1):
            types = FIXED_WIDTH.findall(line)
            if not types:
                continue
            bit_cast_only = all(name == "uint32_t" for name in types) and any(
                function in line for function in BIT_FUNCTIONS
            )
            if not bit_cast_only:
                errors.append(
                    f"unapproved fixed-width type: {path.relative_to(ROOT)}:{line_number}"
                )
    if errors:
        print("\n".join(errors), file=sys.stderr)
        return 1
    topic_count = len(list(FULL.glob("*.hpp")))
    support_count = len(list(INCLUDE.glob("*.hpp")))
    print(f"print branch is current: {topic_count} topic headers, {support_count} support headers")
    return 0


def generate() -> int:
    changed = 0
    for path, text in expected_files().items():
        changed += write_if_changed(path, text)
    for chapter in CHAPTERS:
        text = chapter.read_text(encoding="utf-8")
        changed += write_if_changed(chapter, transform_chapter(text))
    print(f"updated {changed} files")
    return check()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true", help="verify without writing")
    args = parser.parse_args()
    return check() if args.check else generate()


if __name__ == "__main__":
    raise SystemExit(main())
