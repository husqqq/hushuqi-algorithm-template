"""Check header redirection for direct and macro-based test includes."""
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from check_test_variants import INCLUDE, branch_source


class HeaderRedirectionTest(unittest.TestCase):
    def test_direct_and_macro_headers(self):
        text = '''#include "../include/full/3_2_011.hpp"
#ifndef INV_HEADER
#define INV_HEADER "../include/full/3_4_011.hpp"
#endif
#include INV_HEADER
#include "../include/common.hpp"
// "../include/full/comment.hpp" is not a directive.
'''
        with tempfile.TemporaryDirectory(prefix="test-header-redirection-") as directory:
            source = Path(directory) / "test.cpp"
            source.write_text(text, encoding="utf-8")
            for branch in ("full", "print"):
                with self.subTest(branch=branch):
                    actual = branch_source(source, branch)
                    self.assertIn(f'#include "{(INCLUDE / branch / "3_2_011.hpp").as_posix()}"', actual)
                    self.assertIn(f'#define INV_HEADER "{(INCLUDE / branch / "3_4_011.hpp").as_posix()}"', actual)
                    support = INCLUDE / "print" / "support" if branch == "print" else INCLUDE
                    self.assertIn(f'#include "{(support / "common.hpp").as_posix()}"', actual)
                    self.assertIn('#include INV_HEADER', actual)
                    self.assertIn('// "../include/full/comment.hpp"', actual)


if __name__ == "__main__":
    unittest.main()
