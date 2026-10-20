"""Check direct and macro-based header references used by the coverage audit."""

from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from check_non_lc_test_coverage import BEHAVIOR_CHECK, TEST_INCLUDE


class TestReferenceTest(unittest.TestCase):
    def test_direct_macro_and_comment(self):
        text = '''#include "../include/full/3_2_011.hpp"
#ifndef TEST_HEADER
#define TEST_HEADER "../include/print/5_3_008.hpp"
#endif
#include TEST_HEADER
// #include "../include/full/comment.hpp"
'''
        self.assertEqual(TEST_INCLUDE.findall(text), ["3_2_011.hpp", "5_3_008.hpp"])

    def test_behavior_checks(self):
        for statement in ("assert(ok);", "static_assert(true);", "exit(1);", "exit(EXIT_FAILURE);"):
            with self.subTest(statement=statement):
                self.assertIsNotNone(BEHAVIOR_CHECK.search(statement))
        self.assertIsNone(BEHAVIOR_CHECK.search("exit(0);"))


if __name__ == "__main__":
    unittest.main()
