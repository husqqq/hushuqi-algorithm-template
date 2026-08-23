#!/usr/bin/env python3
"""Require every executable topic header to have LC or direct local-test coverage."""

from __future__ import annotations

import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parent
FULL = ROOT / "include" / "full"
TESTS = ROOT / "tests"
MAPPING = REPO / "library-checker-template-solutions" / "LC_TEMPLATE_MAPPING.tsv"

DOCUMENTATION_ONLY = {
    "13_3_005.hpp", "13_3_007.hpp", "14_1_010.hpp", "14_1_011.hpp",
    "14_1_012.hpp", "14_1_013.hpp", "14_1_014.hpp",
}
TEST_INCLUDE = re.compile(r'#include\s+"\.\./include/(?:full/|print/)?([^"/]+\.hpp)"')
HEADER_INCLUDE = re.compile(r'#include\s+"(?:full/)?([^"/]+\.hpp)"')


def main() -> None:
    headers = {path.name: path for path in FULL.glob("*.hpp")}
    support = {path.name: path for path in (ROOT / "include").glob("*.hpp")}
    files = headers | support
    lc: set[str] = set()
    for line in MAPPING.read_text(encoding="utf-8").splitlines():
        if not line or line.startswith("#"):
            continue
        _, topic, *_ = line.split("\t")
        lc.add(topic.replace(".", "_") + ".hpp")

    direct: set[str] = set()
    covered: set[str] = set()

    def visit(name: str) -> None:
        if name in covered or name not in files:
            return
        covered.add(name)
        for dep in HEADER_INCLUDE.findall(files[name].read_text(encoding="utf-8")):
            visit(dep)

    for test in TESTS.glob("*.cpp"):
        text = test.read_text(encoding="utf-8")
        if "assert" not in text and "static_assert" not in text:
            continue
        for name in TEST_INCLUDE.findall(text):
            if name in headers:
                direct.add(name)
            visit(name)

    unknown_docs = DOCUMENTATION_ONLY - headers.keys()
    missing = sorted(headers.keys() - lc - direct - DOCUMENTATION_ONLY)
    if unknown_docs or missing:
        if unknown_docs:
            print("unknown documentation-only headers:", ", ".join(sorted(unknown_docs)))
        if missing:
            print("headers without LC or direct behavior-test coverage:", ", ".join(missing))
        raise SystemExit(1)
    print(
        f"coverage ok: {len(headers)} headers; {len(lc & headers.keys())} LC-mapped; "
        f"{len(direct)} directly included by behavior tests; "
        f"{len(covered & headers.keys())} reached through their dependency closure; "
        f"{len(DOCUMENTATION_ONLY)} documentation-only"
    )


if __name__ == "__main__":
    main()
