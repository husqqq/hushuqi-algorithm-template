import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
SCRIPT = ROOT / "tools" / "f_cmp.py"


class FloatCompareTest(unittest.TestCase):
    def run_case(self, actual, expected):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            x = root / "actual.txt"
            y = root / "expected.txt"
            x.write_text(actual, encoding="utf-8")
            y.write_text(expected, encoding="utf-8")
            result = subprocess.run(
                [sys.executable, str(SCRIPT), str(x), str(y)],
                capture_output=True,
                check=False,
            )
        return result.returncode

    def test_accepts_values_inside_tolerance(self):
        self.assertEqual(self.run_case("0.0000009\n", "0\n"), 0)
        self.assertEqual(self.run_case("1 2.000001\n", "1 2\n"), 0)

    def test_rejects_values_outside_tolerance(self):
        self.assertNotEqual(self.run_case("0.0000011\n", "0\n"), 0)
        self.assertNotEqual(self.run_case("1 2\n", "1\n"), 0)


if __name__ == "__main__":
    unittest.main()
