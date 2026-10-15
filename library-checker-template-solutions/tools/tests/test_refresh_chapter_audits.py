#!/usr/bin/env python3
"""Regression checks for chapter audit refresh routing."""

from __future__ import annotations

import argparse
import importlib.util
import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch


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
        source = ROOT / "data_structure" / "unionfind" / "main.cpp"
        entry = {"meta": str(meta.relative_to(ROOT)).replace("\\", "/"),
                 "source": str(source.relative_to(ROOT)).replace("\\", "/"),
                 "source_sha256": module.sha256_file(source)}
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
        audit = module.load_audit()
        manifest = json.loads(module.MANIFEST.read_text(encoding="utf-8"))
        local = audit.row_state("data_structure/unionfind", {"data_structure/unionfind": entry},
                                audit.receipt_records(manifest))[2]
        self.assertIn("`current_verified`" if local else "`missing`", lines[2])
        self.assertIn("`performance_passed` (1.013x)", lines[2])
        self.assertIn("`online_ac` #", lines[2])

    def test_missing_evidence_is_not_promoted(self) -> None:
        module = load_module()
        with tempfile.TemporaryDirectory(prefix="audit-status-") as directory:
            meta = Path(directory) / "meta.json"
            meta.write_text(json.dumps({"validation": {"status": "local_only"}}), encoding="utf-8")
            audit = SimpleNamespace(receipt_records=lambda _: {},
                                    row_state=lambda *_: (True, True, False, False))
            lines = ["header", "separator", "| `string/example` | api | header | ref | src | stale | old local | old online |"]
            with patch.object(module, "load_audit", return_value=audit):
                module.refresh_status_table(lines, 0, 3, {"string/example": {"meta": str(meta)}},
                                            chapter="04", column_count=10, local_column=7, online_column=8)
            self.assertNotIn("current_verified", lines[2])
            self.assertNotIn("online_ac", lines[2])
            self.assertIn("| stale |", lines[2])

    def test_ch03_prose_does_not_invent_full_receipt(self) -> None:
        module = load_module()
        text = "当前默认本地收据覆盖 0/47 题；"
        self.assertEqual(module.refresh_prose(text, "03", 47), text)

    def test_completion_prose_requires_both_local_and_online(self) -> None:
        module = load_module()
        with tempfile.TemporaryDirectory(prefix="audit-complete-") as directory:
            doc = Path(directory) / "audit.md"
            doc.write_text("| Official problem |\n|---|\n| `other/example` |\n", encoding="utf-8")
            audit = SimpleNamespace(receipt_records=lambda _: {},
                                    row_state=lambda *_: (True, True, False, True))
            before = doc.read_bytes()
            with patch.object(module, "chapter_doc", return_value=doc), \
                 patch.object(module, "load_audit", return_value=audit):
                with self.assertRaisesRegex(RuntimeError, "cannot assert complete"):
                    module.render_chapter("01", [("1.1.001", "other/example")], {}, None)
            self.assertEqual(doc.read_bytes(), before)


if __name__ == "__main__":
    unittest.main()
