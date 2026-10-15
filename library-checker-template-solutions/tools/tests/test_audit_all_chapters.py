#!/usr/bin/env python3

import importlib.util
import hashlib
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch


TOOLS = Path(__file__).resolve().parents[1]


def load(name: str):
    spec = importlib.util.spec_from_file_location(name, TOOLS / f"{name}.py")
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


audit = load("audit_all_chapters")


class AllChapterAuditTest(unittest.TestCase):
    def test_snapshot_keeps_full_frozen_scope(self):
        text = audit.render()
        self.assertIn("- TSV 路由题数：`251`", text)
        self.assertIn("| 03 | 47 |", text)
        self.assertIn("| 10 | 53 |", text)
        self.assertRegex(
            text, r"\| 03 \| 47 \| 47 \| 47 \| 47 \| \d+ \| \d+ \| \d+ \| 47 \|"
        )
        self.assertRegex(
            text, r"\| 10 \| 53 \| 53 \| 53 \| 53 \| \d+ \| \d+ \| \d+ \| 53 \|"
        )
        self.assertRegex(
            text, r"\| 02 \| 1 \| 1 \| 1 \| 1 \| \d+ \| \d+ \| 1 \| 1 \|"
        )
        self.assertRegex(text, r"当前 `\d+/251` 道官方题均已通过")

    def test_scoped_receipt_survives_unrelated_manifest_change(self):
        with tempfile.TemporaryDirectory(prefix="lc-audit-receipt-") as td:
            root = Path(td)
            scoped = root / "scoped"
            scoped.mkdir()
            source = root / "main.cpp"
            source.write_text("signed main() {}\n", encoding="utf-8")
            sha = hashlib.sha256(source.read_bytes()).hexdigest()
            manifest = {"unrelated_problem_revision": "new"}
            record = {"status": "passed", "official_commit": audit.COMMIT,
                      "source_sha256": sha, "verified_at": "2026-10-16"}
            receipt = {"status": "passed", "official_repository": {"commit": audit.COMMIT},
                       "manifest_sha256": "old manifest", "problems": {"p": record}}
            path = scoped / "batch-name.json"
            path.write_text(json.dumps(receipt), encoding="utf-8")
            entry = {"name": "p", "source": "main.cpp", "source_sha256": sha,
                     "meta": "absent.json"}
            with patch.object(audit, "ROOT", root), patch.object(audit, "RECEIPT", root / "none.json"), patch.object(audit, "SCOPED_RECEIPTS", scoped):
                records = audit.receipt_records(manifest)
                self.assertTrue(audit.row_state("graph/p", {"graph/p": entry}, records)[2])
                source.write_text("signed main() { return 1; }\n", encoding="utf-8")
                entry["source_sha256"] = hashlib.sha256(source.read_bytes()).hexdigest()
                self.assertFalse(audit.row_state("graph/p", {"graph/p": entry}, records)[2])
                receipt["official_repository"]["commit"] = "wrong commit"
                path.write_text(json.dumps(receipt), encoding="utf-8")
                self.assertEqual(audit.receipt_records(manifest), {})

    def test_default_receipt_requires_current_manifest(self):
        with tempfile.TemporaryDirectory(prefix="lc-audit-default-") as td:
            path = Path(td) / "receipt.json"
            receipt = {"status": "passed", "official_repository": {"commit": audit.COMMIT},
                       "manifest_sha256": "old manifest", "problems": {"p": {}}}
            path.write_text(json.dumps(receipt), encoding="utf-8")
            self.assertEqual(audit.receipt_records_from(path, {"revision": "new"}), {})


if __name__ == "__main__":
    unittest.main()
