#!/usr/bin/env python3

import importlib.util
import sys
import unittest
from pathlib import Path


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
            text, r"\| 03 \| 47 \| 47 \| 47 \| 47 \| \d+ \| \d+ \| 47 \| 47 \|"
        )
        self.assertRegex(
            text, r"\| 10 \| 53 \| 53 \| 53 \| 53 \| \d+ \| \d+ \| 53 \| 53 \|"
        )
        self.assertRegex(
            text, r"\| 02 \| 1 \| 1 \| 1 \| 1 \| \d+ \| \d+ \| 1 \| 1 \|"
        )
        self.assertIn("当前 `251/251` 道官方题均已通过", text)


if __name__ == "__main__":
    unittest.main()
