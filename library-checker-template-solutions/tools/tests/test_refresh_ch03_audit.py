import importlib.util
import re
import sys
import unittest
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
