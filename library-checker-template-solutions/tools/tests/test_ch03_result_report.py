#!/usr/bin/env python3
import hashlib
import importlib.util
import json
import sys
import tempfile
import unittest
from pathlib import Path


TOOLS = Path(__file__).resolve().parents[1]
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))
SPEC = importlib.util.spec_from_file_location(
    "ch03_result_report",
    TOOLS / "ch03_result_report.py",
)
report = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = report
SPEC.loader.exec_module(report)


class ResultReportTest(unittest.TestCase):
    def write_json(self, path, value):
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(value, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")

    def source_meta(self, source_sha256, attempts, validation):
        return {
            "schema_version": 3,
            "name": "p",
            "title": "P",
            "category": "data_structure",
            "provenance": {
                "kind": "template_solution",
                "source": "data_structure/p/main.cpp",
                "source_sha256": source_sha256,
            },
            "reference": {"kind": "read_only_baseline"},
            "validation": validation,
            "submission_attempts": attempts,
            "notes": [],
        }

    def setup_case(self):
        directory = tempfile.TemporaryDirectory()
        workspace = Path(directory.name)
        root = workspace / "library-checker-template-solutions"
        source = root / "data_structure/p/main.cpp"
        source.parent.mkdir(parents=True)
        source.write_text("int main() { return 0; }\n", encoding="utf-8", newline="\n")
        source_sha256 = hashlib.sha256(source.read_bytes()).hexdigest()
        pre = self.source_meta(
            source_sha256,
            [],
            {
                "status": "local_only",
                "current_source_sha256": source_sha256,
                "current_source_online_verdict": None,
                "current_source_submission_id": None,
                "current_source_time": None,
                "current_source_memory": None,
                "current_source_submission_time": None,
                "current_source_is_latest": None,
                "server_source_sha256": None,
            },
        )
        pre_path = workspace / "pre-meta.json"
        self.write_json(pre_path, pre)
        attempt = {
            "submission_id": 42,
            "source_sha256": source_sha256,
            "server_source_sha256": source_sha256,
            "lang": "cpp20",
            "verdict": "AC",
            "last_status": "AC",
            "time": 0.1,
            "memory": 123,
            "submission_time": "2026-08-20T00:00:00Z",
            "is_latest": True,
        }
        post = self.source_meta(
            source_sha256,
            [attempt],
            {
                "status": "online_ac",
                "current_source_sha256": source_sha256,
                "current_source_online_verdict": "AC",
                "current_source_submission_id": 42,
                "current_source_time": 0.1,
                "current_source_memory": 123,
                "current_source_submission_time": "2026-08-20T00:00:00Z",
                "current_source_is_latest": True,
                "server_source_sha256": source_sha256,
            },
        )
        meta_path = root / "data_structure/p/meta.json"
        self.write_json(meta_path, post)
        audit = root / "docs" / "03-数据结构审计.md"
        audit.parent.mkdir(parents=True, exist_ok=True)
        audit.write_text("audit\n", encoding="utf-8")
        batch = {
            "schema_version": 2,
            "chapter": "03",
            "coverage_audit": {"path": "library-checker-template-solutions/docs/03-数据结构审计.md"},
            "authorization": {"scope": {"problem_id": "data_structure/p"}},
            "problems": [{
                "name": "p",
                "problem_id": "data_structure/p",
                "source_path": "library-checker-template-solutions/data_structure/p/main.cpp",
                "source_sha256": source_sha256,
                "meta_path": "library-checker-template-solutions/data_structure/p/meta.json",
                "meta_pre_submit_sha256": hashlib.sha256(pre_path.read_bytes()).hexdigest(),
            }],
        }
        batch_path = workspace / "batch.json"
        self.write_json(batch_path, batch)
        return directory, root, batch_path, pre_path, meta_path

    def test_online_ac_result_is_append_only_and_bound_to_batch(self):
        directory, root, batch, pre, _ = self.setup_case()
        with directory:
            result = report.make_result(root, batch, pre, "p")
        self.assertEqual(result["result"]["status"], "online_ac")
        self.assertEqual(result["result"]["submission_id"], 42)
        self.assertEqual(result["result"]["language"], "cpp20")
        self.assertEqual(result["meta_change_check"]["submission_attempt_delta"], 1)
        self.assertTrue(result["meta_change_check"]["submission_attempts_are_append_only"])
        self.assertEqual(result["meta_change_check"]["unexpected_json_pointers"], [])

    def test_unexpected_metadata_mutation_is_reported(self):
        directory, root, batch, pre, meta = self.setup_case()
        with directory:
            post = json.loads(meta.read_text(encoding="utf-8"))
            post["title"] = "changed"
            self.write_json(meta, post)
            result = report.make_result(root, batch, pre, "p")
        self.assertFalse(result["meta_change_check"]["unchanged_outside_validation_and_submission_attempts"])
        self.assertIn("/title", result["meta_change_check"]["unexpected_json_pointers"])

    def test_local_path_resolves_report_inputs_from_the_solution_root(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory) / "library-checker-template-solutions"
            target = root / ".local/ch03-batches/p.json"
            target.parent.mkdir(parents=True)
            target.write_text("{}\n", encoding="utf-8")
            self.assertEqual(
                report.local_path(root, Path(".local/ch03-batches/p.json"), "批次"),
                target.resolve(),
            )


if __name__ == "__main__":
    unittest.main()
