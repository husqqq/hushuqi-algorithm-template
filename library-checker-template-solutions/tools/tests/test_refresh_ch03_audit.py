import importlib.util
import re
import sys
import unittest
import tempfile
from unittest.mock import Mock, patch
from pathlib import Path


TOOLS = Path(__file__).resolve().parents[1]
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))


def load(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


audit = load(TOOLS / "refresh_ch03_audit.py", "refresh_ch03_audit_test")
generator = load(TOOLS / "generate_ch03_lc.py", "generate_ch03_lc_audit_test")


class AuditRenderTest(unittest.TestCase):
    def test_local_receipt_is_checked_per_problem(self):
        original_load = audit.load_module
        checked = []

        def load_with_single_receipt(path, name):
            module = original_load(path, name)
            if path.name == "verify_local.py":
                def require(_root, sources, _manifest):
                    self.assertEqual(len(sources), 1)
                    checked.append(sources[0].item.name)
                    if sources[0].item.name != "point_add_range_sum":
                        raise module.LocalVerifyError("other problem has no receipt")
                module.require_receipt = require
            return module

        receipt = {"status": "passed", "problems": {"point_add_range_sum": {
            "status": "passed", "source_sha256": audit.sha256_file(audit.ROOT / "data_structure/point_add_range_sum/main.cpp")}}}
        original_json = audit.read_json
        with patch.object(audit, "load_module", side_effect=load_with_single_receipt), patch.object(audit, "read_json", side_effect=lambda path: receipt if path == audit.RECEIPT else original_json(path)):
            text = audit.render()
        self.assertEqual(set(checked), {spec.name for spec in generator.SPECS})
        self.assertEqual(len(checked), len(generator.SPECS))
        self.assertIn("默认收据当前通过 `1/47`", text)

    def test_generation_status_preserves_unfinished_items(self):
        with tempfile.TemporaryDirectory(prefix="audit-generation-") as directory:
            path = Path(directory) / "main.cpp"
            path.write_bytes(b"current")
            gen = Mock()
            gen.generate_twice.return_value = (b"current", [])
            self.assertEqual(audit.generation_state(gen, None, Path(directory), path), "`generation_current`")
            gen.generate_twice.return_value = (b"new", [])
            self.assertEqual(audit.generation_state(gen, None, Path(directory), path), "`source_stale`")
            gen.generate_twice.side_effect = ValueError("stale dependency")
            self.assertEqual(audit.generation_state(gen, None, Path(directory), path), "`generation_failed`")

    def test_each_row_uses_its_own_topic(self):
        text = audit.render()
        section = re.search(
            r"(?ms)^## Coverage Status\s*$\n(.*?)(?=^## )",
            text,
        )
        self.assertIsNotNone(section)
        rows = {
            match.group(1): match.group(0)
            for match in re.finditer(
                r"(?m)^\| `data_structure/([A-Za-z0-9_]+)` \|.*$",
                section.group(1),
            )
        }
        self.assertEqual(set(rows), {spec.name for spec in generator.SPECS})
        for spec in generator.SPECS:
            expected = spec.topic or "1.4.002 STL deque"
            self.assertIn(f"`{expected}`", rows[spec.name])

    def test_online_summary_needs_strict_certification(self):
        class Submitter:
            class SubmissionError(Exception):
                pass

            @staticmethod
            def already_certified(_):
                return False

        prepared = type("Prepared", (), {"item": type("Item", (), {"name": "p"})()})()
        meta = {
            "validation": {
                "status": "online_ac",
                "current_source_submission_id": 1,
            }
        }
        self.assertEqual(audit.online_state(Submitter, prepared, meta), "`online_pending`")


if __name__ == "__main__":
    unittest.main()
