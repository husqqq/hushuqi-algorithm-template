#!/usr/bin/env python3
"""Regression checks for the chapter 02 source generator command modes."""

from __future__ import annotations

import hashlib
import subprocess
import sys
import unittest
from pathlib import Path


WORKSPACE = Path(__file__).resolve().parents[3]
ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "tools" / "generate_ch02_lc.py"
TRACKED = (
    ROOT / "manifest.json",
    ROOT / "other" / "longest_increasing_subsequence" / "main.cpp",
    ROOT / "other" / "longest_increasing_subsequence" / "meta.json",
)


def snapshot() -> dict[Path, str | None]:
    return {
        path: hashlib.sha256(path.read_bytes()).hexdigest() if path.is_file() else None
        for path in TRACKED
    }


class GenerateChapter02Test(unittest.TestCase):
    def run_tool(self, *args: str) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            [sys.executable, str(SCRIPT), *args],
            cwd=WORKSPACE,
            text=True,
            capture_output=True,
            check=False,
        )

    def test_check_is_read_only(self) -> None:
        before = snapshot()
        result = self.run_tool("--check")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(before, snapshot())

    def test_help_and_missing_mode_are_read_only(self) -> None:
        before = snapshot()
        self.assertEqual(self.run_tool("--help").returncode, 0)
        self.assertNotEqual(self.run_tool().returncode, 0)
        self.assertEqual(before, snapshot())


if __name__ == "__main__":
    unittest.main()
