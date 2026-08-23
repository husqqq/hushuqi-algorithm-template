#!/usr/bin/env python3
"""Regression checks for chapter audit refresh routing."""

from __future__ import annotations

import argparse
import importlib.util
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "tools" / "refresh_chapter_audits.py"


def load_module():
    spec = importlib.util.spec_from_file_location("refresh_chapter_audits", SCRIPT)
    module = importlib.util.module_from_spec(spec)
    assert spec.loader is not None
    spec.loader.exec_module(module)
    return module


class RefreshChapterAuditsTest(unittest.TestCase):
    def test_selected_chapters_are_normalized(self) -> None:
        module = load_module()
        self.assertEqual(
            module.parse_chapters("4,01,02,03,4"), ["01", "02", "03", "04"]
        )

    def test_unsupported_chapter_is_rejected(self) -> None:
        module = load_module()
        with self.assertRaises(argparse.ArgumentTypeError):
            module.parse_chapters("12")

    def test_ch03_refresh_preserves_performance_column(self) -> None:
        module = load_module()
        meta = ROOT / "data_structure" / "unionfind" / "meta.json"
        entry = {"meta": str(meta.relative_to(ROOT)).replace("\\", "/")}
        lines = [
            "| LC problem | Template | Header | Source | Evidence | Local | Performance | Online |",
            "|---|---|---|---|---|---|---|---|",
            "| `data_structure/unionfind` | api | hpp | cpp | evidence | `local_pending` | `performance_passed` (1.013x) | `online_ac` #1 |",
        ]
        module.refresh_status_table(
            lines,
            0,
            3,
            {"data_structure/unionfind": entry},
            chapter="03",
            column_count=10,
            local_column=6,
            online_column=8,
        )
        self.assertIn("`current_verified`", lines[2])
        self.assertIn("`performance_passed` (1.013x)", lines[2])
        self.assertIn("`online_ac` #", lines[2])


if __name__ == "__main__":
    unittest.main()
