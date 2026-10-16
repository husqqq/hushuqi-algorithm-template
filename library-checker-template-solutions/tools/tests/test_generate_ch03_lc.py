"""Check migrated shared dependencies and read-only generator modes."""

from __future__ import annotations

import hashlib
import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

TOOLS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(TOOLS))
import generate_ch03_lc as gen

WORKSPACE = TOOLS.parents[1]
ROOT = TOOLS.parent
SCRIPT = TOOLS / "generate_ch03_lc.py"


def snapshot():
    paths = [ROOT / "manifest.json"]
    for spec in gen.SPECS:
        paths.extend((ROOT / gen.source_path(spec), ROOT / gen.meta_path(spec)))
    return {p: hashlib.sha256(p.read_bytes()).hexdigest() for p in paths}


class GenerateChapter03Test(unittest.TestCase):
    def run_tool(self, *args):
        return subprocess.run(
            [sys.executable, "-X", "utf8", str(SCRIPT), *args], cwd=WORKSPACE,
            capture_output=True, text=True, encoding="utf-8", check=False,
        )

    def test_all_official_routes_generate_deterministically(self):
        gen.validate_spec_coverage(WORKSPACE)
        for spec in gen.SPECS:
            with self.subTest(problem=spec.name):
                source, _ = gen.generate_twice(WORKSPACE, spec)
                self.assertIn(b"signed main()", source)
                self.assertNotIn(b'#include "', source)

    def test_modular_routes_track_the_canonical_component(self):
        for spec in gen.SPECS:
            if spec.extra_markdown != gen.MODDOC:
                continue
            with self.subTest(problem=spec.name):
                source, headers = gen.generate_twice(WORKSPACE, spec)
                report = gen.describe(WORKSPACE, spec, source, headers)
                paths = [item["path"] for item in report["template_inputs"]]
                self.assertTrue(any(p.endswith("/01-基础与工程.md") for p in paths))
                self.assertTrue(any(p.endswith("/full/1_1_008.hpp") for p in paths))
                self.assertFalse(any(p.endswith("/7_1_007.hpp") for p in paths))
                self.assertIn("tests/test_ch01_mint.cpp", spec.tests)
                self.assertEqual(source.count(b"struct MInt"), 1)

    def test_generated_modular_adapter_composition(self):
        spec = gen.selected("range_affine_range_sum")[0]
        source, _ = gen.generate_twice(WORKSPACE, spec)
        with tempfile.TemporaryDirectory(prefix="ch03-generator-test-") as directory:
            cpp = Path(directory) / "main.cpp"
            exe = Path(directory) / "main.exe"
            cpp.write_bytes(source)
            result = subprocess.run(
                ["g++", "-std=c++20", "-O2", str(cpp), "-o", str(exe)],
                capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=120,
            )
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run(
                [str(exe)], input="4 7\n1 2 3 4\n1 0 4\n0 1 3 2 3\n0 0 4 5 7\n1 0 4\n0 1 2 3 1\n1 0 4\n1 1 2\n",
                capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=10,
            )
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertEqual(result.stdout.split(), ["10", "133", "218", "127"])

    def test_read_modes_do_not_modify_candidates_or_index(self):
        before = snapshot()
        self.assertEqual(self.run_tool("--help").returncode, 0)
        self.assertNotEqual(self.run_tool().returncode, 0)
        self.assertNotEqual(self.run_tool("--check", "--write").returncode, 0)
        described = self.run_tool("--describe", "--only", "range_affine_range_sum")
        self.assertEqual(described.returncode, 0, described.stderr)
        self.assertEqual(json.loads(described.stdout)[0]["problem"], "range_affine_range_sum")
        # Stale candidates may fail the check; either result must be read-only.
        self.run_tool("--check", "--only", "range_affine_range_sum")
        self.assertEqual(before, snapshot())

    def test_check_rejects_stale_manifest_and_provenance(self):
        spec = gen.selected("range_affine_range_sum")[0]
        source, headers = gen.generate_twice(WORKSPACE, spec)
        report = gen.describe(WORKSPACE, spec, source, headers)
        expected = gen.metadata(WORKSPACE, spec, report, ["--write-index"])
        entry = {
            "name": spec.name, "title": spec.title, "category": "data_structure",
            "source": gen.source_path(spec), "source_sha256": report["source_sha256"],
            "meta": gen.meta_path(spec), "state": "template_solution",
        }
        with tempfile.TemporaryDirectory(prefix="ch03-index-test-") as directory:
            workspace = Path(directory)
            root = workspace / ROOT.name
            meta = root / gen.meta_path(spec)
            meta.parent.mkdir(parents=True)
            index = root / "manifest.json"
            index.write_text(json.dumps({"problems": [entry]}), encoding="utf-8")
            meta.write_text(json.dumps(expected), encoding="utf-8")
            with patch.object(gen, "metadata", return_value=expected):
                gen.check_index(workspace, spec, report)
                changed = json.loads(meta.read_text(encoding="utf-8"))
                changed["provenance"]["evidence"]["generator"]["sha256"] = "0" * 64
                meta.write_text(json.dumps(changed), encoding="utf-8")
                with self.assertRaisesRegex(ValueError, "metadata provenance"):
                    gen.check_index(workspace, spec, report)
                entry["source_sha256"] = "0" * 64
                index.write_text(json.dumps({"problems": [entry]}), encoding="utf-8")
                with self.assertRaisesRegex(ValueError, "manifest entry"):
                    gen.check_index(workspace, spec, report)

    def test_metadata_preserves_only_same_source_online_evidence(self):
        spec = gen.selected("range_affine_range_sum")[0]
        source, headers = gen.generate_twice(WORKSPACE, spec)
        report = gen.describe(WORKSPACE, spec, source, headers)
        old = {
            "provenance": {"source_sha256": report["source_sha256"]},
            "validation": {
                "status": "online_ac", "current_source_sha256": report["source_sha256"],
                "current_source_submission_id": 42, "current_source_online_verdict": "AC",
            },
            "submission_attempts": [{"submission_id": 42}],
        }
        with patch.object(gen, "read_text", return_value=json.dumps(old)):
            same = gen.metadata(WORKSPACE, spec, report, ["--write-index"])
            self.assertEqual(same["validation"]["status"], "online_ac")
            self.assertEqual(same["validation"]["current_source_submission_id"], 42)
            changed = gen.metadata(WORKSPACE, spec, report | {"source_sha256": "0" * 64}, ["--write-index"])
            self.assertEqual(changed["validation"]["status"], "local_only")
            self.assertIsNone(changed["validation"]["current_source_submission_id"])
            self.assertEqual(same["submission_attempts"], old["submission_attempts"])
            self.assertEqual(changed["submission_attempts"], old["submission_attempts"])

    def test_check_accepts_other_python_but_rejects_changed_evidence(self):
        spec = gen.selected("range_add_range_min")[0]
        source, headers = gen.generate_twice(WORKSPACE, spec)
        report = gen.describe(WORKSPACE, spec, source, headers)
        expected = gen.metadata(WORKSPACE, spec, report, ["--write-index"], runner="Python 3.13.14")
        entry = {
            "name": spec.name, "title": spec.title, "category": "data_structure",
            "source": gen.source_path(spec), "source_sha256": report["source_sha256"],
            "meta": gen.meta_path(spec), "state": "template_solution",
        }
        with tempfile.TemporaryDirectory(prefix="ch03-runner-test-") as directory:
            workspace = Path(directory)
            root = workspace / ROOT.name
            meta = root / gen.meta_path(spec)
            meta.parent.mkdir(parents=True)
            (root / "manifest.json").write_text(json.dumps({"problems": [entry]}), encoding="utf-8")
            # 引用真实输入和生成器；仅把待核验元数据隔离到临时目录。
            original = gen.metadata
            def metadata_at_source(_root, *args, **kwargs):
                return original(WORKSPACE, *args, **kwargs)
            with patch.object(gen, "metadata", side_effect=metadata_at_source), \
                    patch.object(gen.platform, "python_version", return_value="3.12.14"):
                meta.write_text(json.dumps(expected), encoding="utf-8")
                before = meta.read_bytes()
                gen.check_index(workspace, spec, report)
                self.assertEqual(meta.read_bytes(), before)
                mutations = (
                    ("runner", None), ("runner", ""), ("runner", 312),
                    ("runner", "Python unknown"), ("runner", "Python 3.12.14"),
                    ("sha256", "0" * 64),
                    ("generation_evidence_sha256", "0" * 64),
                )
                for key, value in mutations:
                    with self.subTest(key=key, value=value):
                        changed = json.loads(json.dumps(expected))
                        changed["provenance"]["evidence"]["generator"][key] = value
                        meta.write_text(json.dumps(changed), encoding="utf-8")
                        with self.assertRaisesRegex(ValueError, "metadata provenance"):
                            gen.check_index(workspace, spec, report)
                changed = json.loads(json.dumps(expected))
                changed["provenance"]["evidence"]["generator"]["generation_evidence"]["runner"] = "Python 3.12.14"
                meta.write_text(json.dumps(changed), encoding="utf-8")
                with self.assertRaisesRegex(ValueError, "metadata provenance"):
                    gen.check_index(workspace, spec, report)


if __name__ == "__main__":
    unittest.main()
