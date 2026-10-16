"""直接测试第十三章真实代码在两种普通整数宽度下的行为。"""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]


class Chapter13RegressionTest(unittest.TestCase):
    def test_widths_and_dag_contract(self):
        compiler = shutil.which("g++") or shutil.which("clang++")
        self.assertIsNotNone(compiler, "C++20 compiler required")
        slow = (ROOT / "tests/test_ch13_nim_product.cpp").read_text(encoding="utf-8")
        slow = slow[slow.index("unsigned long long slowProduct"):slow.index("signed main()")]
        checks = r'''
signed main(signed argc, char **argv)
{
#ifdef _WIN32
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    _set_error_mode(_OUT_TO_STDERR);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#endif
    if (argc > 1)
    {
        int mode = atoi(argv[1]);
        vector<vector<pair<int, long double>>> g(2);
        vector<long double> cost(2);
        vector<int> order{1, 0};
        g[0] = {{1, 0.5L}};
        if (mode == 1) order = {0, 1};
        if (mode == 2) order = {1, 1};
        if (mode == 3) order = {1, 2};
        if (mode == 4) g[0][0].first = 2;
        if (mode == 5) g[0][0].second = -0.1L;
        if (mode == 6) g[0][0].second = 1.1L;
        if (mode == 7) g[0][0].second = numeric_limits<long double>::infinity();
        if (mode == 8) cost[0] = numeric_limits<long double>::quiet_NaN();
        expectedDAG(g, cost, order);
        return 0;
    }
    assert(ntt.back()[0] == 2281701377LL);
    assert(expectedDAG({}, {}, {}).empty());
    assert((expectedDAG({{{1, 0.25L}, {2, 0.5L}}, {}, {}},
                        {-2, 4, 8}, {2, 1, 0}) == vector<long double>{3, 4, 8}));
    mt19937_64 rng(20261016);
    vector<unsigned long long> edge{0, 1, 2, 1ULL << 31, 0xffffffffULL,
                                   1ULL << 32, 1ULL << 63, ~0ULL};
    for (auto a : edge)
    {
        assert(nimProduct(a, 1) == a);
        for (auto b : edge)
        {
            assert(nimProduct(a, b) == slowProduct(a, b));
        }
    }
    for (int tc = 0; tc < 1000; tc++)
    {
        unsigned long long a = rng(), b = rng();
        assert(nimProduct(a, b) == slowProduct(a, b));
        a &= 0xffffffffULL;
        b &= 0xffffffffULL;
        assert(nimProduct32(a, b) == slowProduct(a, b, 32));
        assert(NimProduct64::p32high(a, b) ==
               slowProduct(slowProduct(a, b, 32), 1ULL << 31, 32));
        int n = rng() % 8 + 1;
        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        shuffle(order.begin(), order.end(), rng);
        vector<vector<pair<int, long double>>> g(n);
        vector<long double> cost(n);
        for (int i = 0; i < n; i++)
        {
            cost[i] = (int)(rng() % 21) - 10;
            for (int j = 0; j < i; j++)
            {
                if (rng() % 2) g[order[i]].push_back({order[j], 1.0L / n});
            }
        }
        auto got = expectedDAG(g, cost, order);
        // 枚举从起点出发的每条路径，以到达概率加权访问代价。
        auto paths = [&](auto &&self, int u, long double prob) -> long double
        {
            long double ans = prob * cost[u];
            for (auto [v, p] : g[u])
            {
                ans += self(self, v, prob * p);
            }
            return ans;
        };
        for (int u = 0; u < n; u++)
        {
            assert(abs(got[u] - paths(paths, u, 1)) < 1E-12L);
        }
    }
}
'''
        with tempfile.TemporaryDirectory(prefix="ch13-widths-") as directory:
            folder = Path(directory)
            for branch in ("full", "print", "markdown"):
                bodies = []
                for topic in ("13_1_009", "13_2_005", "13_3_006"):
                    if branch == "markdown":
                        doc = (ROOT / "13-博弈论、概率与杂项数学.md").read_text(encoding="utf-8")
                        section = doc.split("### " + topic.replace("_", "."), 1)[1]
                        body = section.split("```cpp\n", 1)[1].split("```", 1)[0]
                    else:
                        body = (ROOT / "include" / branch / (topic + ".hpp")).read_text(encoding="utf-8")
                    body = re.sub(r"(?m)^#(?:pragma once|include .*|define int long long)\s*$", "", body)
                    bodies.append(body)
                for macro in (False, True):
                    with self.subTest(branch=branch, macro=macro):
                        code = "#include <bits/stdc++.h>\n#ifdef _WIN32\n#include <windows.h>\n#endif\nusing namespace std;\n"
                        if macro:
                            code += "#define int long long\n"
                        source = folder / f"{branch}-{int(macro)}.cpp"
                        binary = source.with_suffix(".exe")
                        source.write_text(code + "\n".join(bodies) + slow + checks, encoding="utf-8")
                        built = subprocess.run(
                            [compiler, "-std=c++20", "-O2", str(source), "-o", str(binary)],
                            capture_output=True, text=True, encoding="utf-8", errors="replace",
                        )
                        self.assertEqual(built.returncode, 0, built.stderr)
                        ran = subprocess.run([str(binary)], cwd=folder, capture_output=True, timeout=60)
                        self.assertEqual(ran.returncode, 0, ran.stderr.decode(errors="replace"))
                        for mode in range(1, 9):
                            bad = subprocess.run([str(binary), str(mode)], cwd=folder, capture_output=True, timeout=30)
                            self.assertNotEqual(bad.returncode, 0, f"missing contract check: {mode}")


if __name__ == "__main__":
    unittest.main()
