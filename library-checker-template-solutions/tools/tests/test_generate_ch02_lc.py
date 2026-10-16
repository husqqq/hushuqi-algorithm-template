#!/usr/bin/env python3
"""Regression checks for the chapter 02 source generator command modes."""

from __future__ import annotations

import hashlib
import copy
import importlib.util
import json
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch


WORKSPACE = Path(__file__).resolve().parents[3]
ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "tools" / "generate_ch02_lc.py"
TRACKED = (
    ROOT / "manifest.json",
    ROOT / "other" / "longest_increasing_subsequence" / "main.cpp",
    ROOT / "other" / "longest_increasing_subsequence" / "meta.json",
)
SPEC = importlib.util.spec_from_file_location("generate_ch02_lc", SCRIPT)
assert SPEC and SPEC.loader
GEN = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(GEN)


def snapshot() -> dict[Path, str | None]:
    return {
        path: hashlib.sha256(path.read_bytes()).hexdigest() if path.is_file() else None
        for path in TRACKED
    }


class GenerateChapter02Test(unittest.TestCase):
    def run_tool(self, *args: str) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            [sys.executable, "-X", "utf8", str(SCRIPT), *args],
            cwd=WORKSPACE,
            text=True,
            encoding="utf-8",
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

    def test_inputs_cover_actual_dependencies(self) -> None:
        inputs = {item["path"]: item["sha256"] for item in GEN.template_inputs()}
        prefix = "hushuqi算法竞赛模板/"
        self.assertEqual(set(inputs), {prefix + name for name in (
            GEN.CHAPTER, "include/full/2_2_010.hpp", "include/print/2_2_010.hpp",
            "include/topic_common.hpp", "include/common.hpp", GEN.TEST,
        )})
        for name, digest in inputs.items():
            self.assertEqual(digest, GEN.sha256(WORKSPACE / name))

    def test_markdown_drift_stops_write_before_mutation(self) -> None:
        with tempfile.TemporaryDirectory(prefix="ch02-sync-") as directory:
            root = Path(directory)
            template, out = root / "template", root / "out"
            for item in GEN.template_inputs():
                source = WORKSPACE / item["path"]
                target = template / source.relative_to(GEN.TEMPLATE)
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(source, target)
            chapter = template / GEN.CHAPTER
            chapter.write_text(chapter.read_text(encoding="utf-8").replace(
                "pre[i] = at[p - 1];", "pre[i] = -1;"), encoding="utf-8")
            with patch.object(GEN, "TEMPLATE", template), patch.object(GEN, "OUT", out):
                with self.assertRaisesRegex(ValueError, "differs"):
                    GEN.write_current({"problems": []})
            self.assertFalse(out.exists())

    def test_expansion_rejects_include_outside_header_tree(self) -> None:
        with tempfile.TemporaryDirectory(prefix="ch02-include-") as directory:
            template = Path(directory)
            (template / "include").mkdir()
            source = template / "include" / "test.hpp"
            source.write_text('#include "../outside.hpp"\n', encoding="utf-8")
            (template / "outside.hpp").write_text("sensitive\n", encoding="utf-8")
            with patch.object(GEN, "TEMPLATE", template):
                with self.assertRaisesRegex(ValueError, "outside template"):
                    GEN.expand(source, set())

    def test_evidence_preserved_only_for_unchanged_source(self) -> None:
        digest = hashlib.sha256(GEN.generated_source()).hexdigest()
        with tempfile.TemporaryDirectory(prefix="ch02-meta-") as directory:
            source = Path(directory) / "main.cpp"
            previous = GEN.expected_meta(digest)
            previous["validation"]["status"] = "online_ac"
            previous["submission_attempts"] = [{"submission_id": 123, "source_sha256": digest}]
            (source.parent / "meta.json").write_text(json.dumps(previous), encoding="utf-8")
            unchanged = GEN.current_meta(source, digest, False)
            changed = GEN.current_meta(source, "0" * 64, True)
            self.assertEqual(unchanged["validation"]["status"], "online_ac")
            self.assertEqual(changed["validation"]["status"], "local_only")
            self.assertEqual(changed["validation"]["current_source_sha256"], "0" * 64)
            self.assertEqual(changed["submission_attempts"], previous["submission_attempts"])

    def test_check_uses_recorded_runner_but_rejects_proof_tampering(self) -> None:
        with tempfile.TemporaryDirectory(prefix="ch02-runner-") as directory:
            with patch.object(GEN, "OUT", Path(directory)), patch.object(GEN, "ROOT", WORKSPACE):
                manifest = {"problems": []}
                GEN.write_current(manifest)
                target = GEN.OUT / "other" / GEN.NAME / "meta.json"
                digest = manifest["problems"][0]["source_sha256"]
                previous = GEN.expected_meta(digest, runner="Python 3.12.14")
                target.write_text(json.dumps(previous), encoding="utf-8")
                with patch.object(GEN.platform, "python_version", return_value="3.13.14"):
                    self.assertEqual(GEN.check_current(manifest), [])
                for field, value in (("runner", None), ("runner", "Python 3.11.0"),
                                     ("sha256", "0" * 64), ("generation_evidence_sha256", "0" * 64)):
                    altered = copy.deepcopy(previous)
                    altered["provenance"]["evidence"]["generator"][field] = value
                    target.write_text(json.dumps(altered), encoding="utf-8")
                    self.assertTrue(GEN.check_current(manifest), field)
                altered = copy.deepcopy(previous)
                altered["provenance"]["evidence"]["generator"]["generation_evidence"]["inputs"][0]["sha256"] = "0" * 64
                target.write_text(json.dumps(altered), encoding="utf-8")
                self.assertTrue(GEN.check_current(manifest))

    def test_stale_hashes_reset_current_verdict_even_without_source_write(self) -> None:
        digest = hashlib.sha256(GEN.generated_source()).hexdigest()
        with tempfile.TemporaryDirectory(prefix="ch02-stale-") as directory:
            source = Path(directory) / "main.cpp"
            for field, key in (("provenance", "source_sha256"), ("validation", "current_source_sha256")):
                previous = GEN.expected_meta(digest)
                previous["validation"]["status"] = "online_ac"
                previous["submission_attempts"] = [{"submission_id": 123}]
                previous[field][key] = "0" * 64
                source.with_name("meta.json").write_text(json.dumps(previous), encoding="utf-8")
                current = GEN.current_meta(source, digest, False)
                self.assertEqual(current["validation"]["status"], "local_only")
                self.assertEqual(current["submission_attempts"], previous["submission_attempts"])


if __name__ == "__main__":
    unittest.main()
