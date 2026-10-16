"""直接测试第十三章真实 RNG 代码的固定序列、边界和双版本一致性。"""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]


class Chapter13RngTest(unittest.TestCase):
    def test_all_sources_and_widths(self):
        compiler = shutil.which("g++") or shutil.which("clang++")
        self.assertIsNotNone(compiler, "C++20 compiler required")
        check = (ROOT / "tests/test_ch13_rng.cpp").read_text(encoding="utf-8")
        check = check[check.index("signed main()"):]
        with tempfile.TemporaryDirectory(prefix="ch13-rng-") as directory:
            folder = Path(directory)
            for branch in ("full", "print", "markdown"):
                if branch == "markdown":
                    doc = (ROOT / "13-博弈论、概率与杂项数学.md").read_text(encoding="utf-8")
                    body = doc.split("### 13.2.004", 1)[1].split("```cpp\n", 1)[1].split("```", 1)[0]
                else:
                    body = (ROOT / "include" / branch / "13_2_004.hpp").read_text(encoding="utf-8")
                body = re.sub(r"(?m)^#(?:pragma once|include .*|define int long long)\s*$", "", body)
                for macro in (False, True):
                    source = folder / f"{branch}-{int(macro)}.cpp"
                    binary = source.with_suffix(".exe")
                    prefix = "#include <bits/stdc++.h>\nusing namespace std;\n"
                    if macro:
                        prefix += "#define int long long\n"
                    source.write_text(prefix + body + "\n" + check, encoding="utf-8")
                    built = subprocess.run(
                        [compiler, "-std=c++20", "-O2", str(source), "-o", str(binary)],
                        capture_output=True, text=True, encoding="utf-8", errors="replace",
                    )
                    self.assertEqual(built.returncode, 0, built.stderr)
                    ran = subprocess.run([str(binary)], cwd=folder, capture_output=True, timeout=30)
                    self.assertEqual(ran.returncode, 0, ran.stderr.decode(errors="replace"))


if __name__ == "__main__":
    unittest.main()
