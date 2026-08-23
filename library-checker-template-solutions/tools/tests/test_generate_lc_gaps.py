#!/usr/bin/env python3
"""Regression checks for the shared LC gap source generator."""

from __future__ import annotations

import importlib.util
import subprocess
import sys
import unittest
from pathlib import Path


WORKSPACE = Path(__file__).resolve().parents[3]
ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "tools" / "generate_lc_gaps.py"


def load_generator():
    spec = importlib.util.spec_from_file_location("generate_lc_gaps", SCRIPT)
    module = importlib.util.module_from_spec(spec)
    assert spec.loader is not None
    spec.loader.exec_module(module)
    return module


class GenerateLcGapsTest(unittest.TestCase):
    def test_missing_mode_is_rejected(self) -> None:
        result = subprocess.run(
            [sys.executable, str(SCRIPT), "--only", "convolution_F_2_64"],
            cwd=WORKSPACE,
            text=True,
            capture_output=True,
            check=False,
        )
        self.assertNotEqual(result.returncode, 0)

    def test_changed_source_preserves_submission_attempts(self) -> None:
        generator = load_generator()
        attempt = {"submission_id": 123, "source_sha256": "a" * 64}
        fresh = {
            "validation": {"status": "local_only", "current_source_sha256": "b" * 64},
            "submission_attempts": [],
        }
        old = {
            "validation": {"status": "online_ac", "current_source_sha256": "a" * 64},
            "submission_attempts": [attempt],
        }
        merged = generator.merge_history(fresh, old, "b" * 64)
        self.assertEqual(merged["validation"]["status"], "local_only")
        self.assertEqual(merged["submission_attempts"], [attempt])

    def test_same_source_preserves_validation(self) -> None:
        generator = load_generator()
        validation = {"status": "online_ac", "current_source_sha256": "c" * 64}
        fresh = {
            "validation": {"status": "local_only", "current_source_sha256": "c" * 64},
            "submission_attempts": [],
        }
        merged = generator.merge_history(
            fresh, {"validation": validation, "submission_attempts": []}, "c" * 64
        )
        self.assertIs(merged["validation"], validation)


if __name__ == "__main__":
    unittest.main()
