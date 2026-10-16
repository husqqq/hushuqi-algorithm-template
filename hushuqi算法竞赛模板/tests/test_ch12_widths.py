"""Compile the actual Hanoi and Slope Trick bodies with both ordinary int widths."""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]


class WidthRegressionTest(unittest.TestCase):
    def test_hanoi_and_slope_with_and_without_macro(self):
        compiler = shutil.which("g++") or shutil.which("clang++")
        self.assertIsNotNone(compiler, "C++20 compiler required")
        # Only strip repository includes and the common preamble. Algorithm bodies
        # are compiled unchanged; all temporary files stay outside the repository.
        with tempfile.TemporaryDirectory(prefix="ch12-widths-") as directory:
            folder = Path(directory)
            for branch in ("full", "print"):
                bodies = []
                for name in ("12_2_006.hpp", "12_3_007.hpp"):
                    body = (ROOT / "include" / branch / name).read_text(encoding="utf-8")
                    body = re.sub(r"(?m)^#(?:pragma once|include .*|define int long long)\s*$", "", body)
                    bodies.append(body)
                for macro in (False, True):
                    with self.subTest(branch=branch, macro=macro):
                        code = "#include <bits/stdc++.h>\nusing namespace std;\n"
                        if macro:
                            code += "#define int long long\n"
                        code += "\n".join(bodies) + r'''
signed main()
{
    assert((hanoi::kthMove(63, (1ULL << 62) + 1) == array<int, 3>{1, 1, 0}));
    assert((hanoi::kthMove(63, (1ULL << 63) - 1) == array<int, 3>{1, 0, 2}));
    SlopeTrick f;
    assert(f.eval(7) == 0 && !f.argmin().first && !f.argmin().second);
    f.addAbs(2);
    f.addAbs(5);
    assert(f.mn == 3 && f.eval(0) == 7 && f.eval(7) == 7);
    assert(f.argmin().first == 2 && f.argmin().second == 5);
    f.shift(1, 3);
    assert(f.mn == 3 && f.eval(2) == 5 && f.eval(9) == 5);
    assert(f.argmin().first == 3 && f.argmin().second == 8);
    f.prefixMin();
    assert(f.eval(9) == 3 && !f.argmin().second);
    f.addRight(4);
    assert(f.eval(9) == 8);
    f.suffixMin();
    assert(f.eval(-10) == 3 && !f.argmin().first);
}
'''
                        source = folder / f"{branch}-{int(macro)}.cpp"
                        binary = source.with_suffix(".exe")
                        source.write_text(code, encoding="utf-8")
                        built = subprocess.run(
                            [compiler, "-std=c++20", "-O2", str(source), "-o", str(binary)],
                            capture_output=True, text=True, encoding="utf-8", errors="replace",
                        )
                        self.assertEqual(built.returncode, 0, built.stderr)
                        ran = subprocess.run([str(binary)], capture_output=True)
                        self.assertEqual(ran.returncode, 0, ran.stderr.decode(errors="replace"))


if __name__ == "__main__":
    unittest.main()
