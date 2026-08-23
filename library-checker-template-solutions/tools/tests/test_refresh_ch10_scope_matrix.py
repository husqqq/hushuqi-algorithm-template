#!/usr/bin/env python3

import importlib.util
import sys
import unittest
from pathlib import Path


TOOLS = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "refresh_ch10_scope_matrix", TOOLS / "refresh_ch10_scope_matrix.py"
)
matrix = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = matrix
SPEC.loader.exec_module(matrix)


class Chapter10ScopeMatrixTest(unittest.TestCase):
    def test_matrix_has_every_frozen_problem_once(self):
        block = matrix.block()
        rows = matrix.scoped_rows()
        self.assertEqual(len(rows), 53)
        for _, problem, _ in rows:
            self.assertEqual(block.count(f"`{problem}`"), 1)
            self.assertIn("`review_pending`", block)


if __name__ == "__main__":
    unittest.main()
