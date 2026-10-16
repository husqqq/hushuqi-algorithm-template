#!/usr/bin/env python3
"""Regression checks for the chapter 04 source generator command modes."""

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
SCRIPT = ROOT / "tools" / "generate_ch04_lc.py"
TRACKED = (
    ROOT / "manifest.json",
    *(ROOT / "string" / name / filename
      for name in (
          "aho_corasick", "eertree", "enumerate_palindromes",
          "longest_common_substring", "lyndon_factorization",
          "number_of_substrings", "palindromes_in_deque", "runenumerate",
          "suffixarray", "wildcard_pattern_matching", "zalgorithm",
      )
      for filename in ("main.cpp", "meta.json")),
)


def snapshot() -> dict[Path, str]:
    return {path: hashlib.sha256(path.read_bytes()).hexdigest() for path in TRACKED}


def generator():
    spec = importlib.util.spec_from_file_location("ch04_generator_test", SCRIPT)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


class GenerateChapter04Test(unittest.TestCase):
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

    def test_scoped_check_is_read_only(self) -> None:
        before = snapshot()
        result = self.run_tool("--check", "--only", "aho_corasick,eertree,number_of_substrings,longest_common_substring,runenumerate,palindromes_in_deque,zalgorithm,enumerate_palindromes,lyndon_factorization,suffixarray")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(before, snapshot())

    def test_unknown_scope_is_read_only(self) -> None:
        before = snapshot()
        for value in ("missing", ""):
            self.assertNotEqual(self.run_tool("--write", "--only", value).returncode, 0)
        self.assertEqual(before, snapshot())

    def test_scoped_write_preserves_other_sources(self) -> None:
        module = generator()
        with tempfile.TemporaryDirectory(prefix="template-ch04-test-") as td:
            out = Path(td)
            other = out / "string/longest_common_substring"
            other.mkdir(parents=True)
            (other / "main.cpp").write_bytes(b"untouched\n")
            (other / "meta.json").write_bytes(b"{}\n")
            manifest = {"problems": [{"name": "longest_common_substring", "sentinel": True}]}
            with patch.object(module, "OUT", out):
                module.write_current(manifest, ["zalgorithm"])
                self.assertEqual(module.check_current(manifest, ["zalgorithm"]), [])
            self.assertEqual((other / "main.cpp").read_bytes(), b"untouched\n")
            self.assertEqual((other / "meta.json").read_bytes(), b"{}\n")
            self.assertIn({"name": "longest_common_substring", "sentinel": True}, manifest["problems"])

    def test_documentation_mismatch_is_rejected(self) -> None:
        module = generator()
        with tempfile.TemporaryDirectory(prefix="template-ch04-doc-test-") as td:
            target = Path(td)
            for relative in ["目录.md", "04-字符串.md", "tools/sync_topic_docs.py", "include/print/4_3_004.hpp"]:
                dest = target / relative
                dest.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(module.TEMPLATE / relative, dest)
            with patch.object(module, "TEMPLATE", target):
                module.check_documentation("4_3_004.hpp")
                header = target / "include/print/4_3_004.hpp"
                header.write_text(header.read_text(encoding="utf-8") + "\n// 故意制造正文差异\n", encoding="utf-8")
                with self.assertRaisesRegex(ValueError, "documentation differs"):
                    module.check_documentation("4_3_004.hpp")

    def test_changed_source_preserves_history_and_resets_verdict(self) -> None:
        module = generator()
        with tempfile.TemporaryDirectory(prefix="template-ch04-history-") as td:
            source = Path(td) / "main.cpp"
            previous = {"submission_attempts": [{"submission_id": 123}],
                        "validation": {"status": "online_ac"}}
            source.with_name("meta.json").write_text(json.dumps(previous), encoding="utf-8")
            name = "wildcard_pattern_matching"
            data = module.generated_source(name, *module.SPECS[name])
            digest = hashlib.sha256(data).hexdigest()
            result = module.current_meta_or_reset(name, source, digest, True)
            self.assertEqual(result["submission_attempts"], previous["submission_attempts"])
            self.assertEqual(result["validation"]["status"], "local_only")

    def test_commented_dependencies_are_expanded(self) -> None:
        module = generator()
        with tempfile.TemporaryDirectory(prefix="template-ch04-include-") as td:
            target = Path(td)
            (target / "dep.hpp").write_text("#pragma once\nint value;\n", encoding="utf-8")
            (target / "main.hpp").write_text('#include "dep.hpp" // 中文依赖\n', encoding="utf-8")
            self.assertEqual(module.expand(target / "main.hpp", set()), "int value;\n")

    def test_check_uses_recorded_runner_but_rejects_proof_tampering(self) -> None:
        module = generator()
        name = "zalgorithm"
        with tempfile.TemporaryDirectory(prefix="ch04-runner-") as directory:
            with patch.object(module, "OUT", Path(directory)):
                manifest = {"problems": []}
                module.write_current(manifest, [name])
                target = module.OUT / "string" / name / "meta.json"
                digest = manifest["problems"][0]["source_sha256"]
                previous = module.expected_meta(name, digest, runner="Python 3.12.14")
                target.write_text(json.dumps(previous), encoding="utf-8")
                with patch.object(module.platform, "python_version", return_value="3.13.14"):
                    self.assertEqual(module.check_current(manifest, [name]), [])
                for field, value in (("runner", None), ("runner", "Python 3.11.0"), ("sha256", "0" * 64)):
                    altered = copy.deepcopy(previous)
                    altered["provenance"]["evidence"]["generator"][field] = value
                    target.write_text(json.dumps(altered), encoding="utf-8")
                    self.assertTrue(module.check_current(manifest, [name]), field)
                for field in ("evidence_sha256", "template_inputs"):
                    altered = copy.deepcopy(previous)
                    proof = altered["provenance"]["evidence"]
                    if field == "template_inputs":
                        proof[field][0]["sha256"] = "0" * 64
                    else:
                        proof[field] = "0" * 64
                    target.write_text(json.dumps(altered), encoding="utf-8")
                    self.assertTrue(module.check_current(manifest, [name]), field)

    def test_stale_hashes_reset_current_verdict_even_without_source_write(self) -> None:
        module = generator()
        name = "zalgorithm"
        digest = hashlib.sha256(module.generated_source(name, *module.SPECS[name])).hexdigest()
        with tempfile.TemporaryDirectory(prefix="ch04-stale-") as directory:
            source = Path(directory) / "main.cpp"
            for field, key in (("provenance", "source_sha256"), ("validation", "current_source_sha256")):
                previous = module.expected_meta(name, digest)
                previous["validation"]["status"] = "online_ac"
                previous["submission_attempts"] = [{"submission_id": 123}]
                previous[field][key] = "0" * 64
                source.with_name("meta.json").write_text(json.dumps(previous), encoding="utf-8")
                current = module.current_meta_or_reset(name, source, digest, False)
                self.assertEqual(current["validation"]["status"], "local_only")
                self.assertEqual(current["submission_attempts"], previous["submission_attempts"])
            previous = module.expected_meta(name, digest)
            previous["validation"]["status"] = "online_ac"
            source.with_name("meta.json").write_text(json.dumps(previous), encoding="utf-8")
            self.assertEqual(module.current_meta_or_reset(name, source, digest, False)["validation"]["status"], "online_ac")


if __name__ == "__main__":
    unittest.main()
