#!/usr/bin/env python3

import importlib.util
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch


TOOLS = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "refresh_untrusted_inventory", TOOLS / "refresh_untrusted_inventory.py"
)
inventory = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = inventory
SPEC.loader.exec_module(inventory)


class UntrustedInventoryTest(unittest.TestCase):
    def row(self):
        return {
            "chapter": "01",
            "template_item": "1.1.001",
            "problem": "sample/aplusb",
            "mapping_confidence": "audited",
        }

    def inspect(self, *, local=False, online=False):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            source = root / "main.cpp"
            source.write_text("signed main() {}\n", encoding="utf-8")
            source_sha = inventory.file_sha256(source)
            meta = root / "meta.json"
            validation = {}
            if online:
                validation = {
                    "status": "online_ac",
                    "current_source_sha256": source_sha,
                    "current_source_online_verdict": "AC",
                    "current_source_is_latest": True,
                    "server_source_sha256": source_sha,
                    "current_source_submission_id": 1,
                }
            meta.write_text(
                inventory.json.dumps({"validation": validation}), encoding="utf-8"
            )
            entry = {
                "name": "aplusb",
                "source": "main.cpp",
                "source_sha256": source_sha,
                "meta": "meta.json",
            }
            receipts = {}
            if local:
                receipts["aplusb"] = {
                    "status": "passed",
                    "official_commit": inventory.COMMIT,
                    "source_sha256": source_sha,
                }
            with patch.object(inventory, "ROOT", root):
                return inventory.inspect_row(
                    self.row(), {"sample/aplusb": entry}, receipts
                )

    def test_local_or_online_evidence_is_fully_trusted(self):
        self.assertEqual(self.inspect(local=True)["reasons"], "fully_trusted")
        self.assertEqual(self.inspect(online=True)["reasons"], "fully_trusted")
        self.assertEqual(self.inspect()["reasons"], "verification_missing")

    def test_empty_inventory_removes_old_directory(self):
        with tempfile.TemporaryDirectory() as directory:
            out = Path(directory) / "不可完全置信清单"
            out.mkdir()
            (out / "old.md").write_text("stale", encoding="utf-8")
            manifest = {"official_repository": {"commit": inventory.COMMIT}}
            trusted = self.row() | {"reasons": "fully_trusted"}
            with (
                patch.object(inventory, "OUT", out),
                patch.object(inventory, "read_json", return_value=manifest),
                patch.object(inventory, "source_entries", return_value={}),
                patch.object(inventory, "receipt_records", return_value={}),
                patch.object(inventory, "read_mapping", return_value=[self.row()]),
                patch.object(inventory, "inspect_row", return_value=trusted),
            ):
                inventory.main()
            self.assertFalse(out.exists())


if __name__ == "__main__":
    unittest.main()
