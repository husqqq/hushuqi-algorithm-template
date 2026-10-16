"""Keep dependency comments readable without duplicating explicit interface notes."""

import importlib.util
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch


WORKSPACE = Path(__file__).resolve().parents[3]
SPEC = importlib.util.spec_from_file_location(
    "sync_topic_docs_test", WORKSPACE / "hushuqi算法竞赛模板/tools/sync_topic_docs.py",
)
SYNC = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(SYNC)


class TopicDependencyTest(unittest.TestCase):
    def snippet(self, text):
        with tempfile.TemporaryDirectory(prefix="topic-dependency-test-") as directory:
            root = Path(directory)
            target = root / "include/print/3_3_004.hpp"
            target.parent.mkdir(parents=True)
            target.write_text(text, encoding="utf-8")
            with patch.object(SYNC, "ROOT", root):
                return SYNC.snippet(target.name)

    def test_explicit_dependency_keeps_its_interface_explanation(self):
        result = self.snippet(
            '#pragma once\n#include "3_3_002.hpp"\n\n'
            '// 依赖 3.3.002 的 LazySeg 和 Tag。\nstruct MinInfo {};\n'
        )
        self.assertEqual(result, '```cpp\n// 依赖 3.3.002 的 LazySeg 和 Tag。\nstruct MinInfo {};\n```')

    def test_distinct_dependencies_are_kept(self):
        result = self.snippet(
            '#include "3_3_001.hpp"\n#include "3_3_002.hpp"\nstruct Info {};\n'
        )
        self.assertIn('// 依赖 3.3.001 ', result)
        self.assertIn('// 依赖 3.3.002 ', result)

    def test_dependency_is_still_generated_when_no_explicit_note_exists(self):
        result = self.snippet('#include "3_3_002.hpp"\nstruct Info {};\n')
        self.assertEqual(result.count('// 依赖 3.3.002 '), 1)
        self.assertIn('struct Info {};', result)


if __name__ == "__main__":
    unittest.main()
