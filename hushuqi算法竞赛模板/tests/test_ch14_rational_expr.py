"""Independent rational reconstruction and expression-tree checks on all copy paths."""
from __future__ import annotations

import math
import os
import random
import re
import subprocess
import sys
import tempfile
from fractions import Fraction
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from check_test_variants import compiler, branch_source


def code(text: str) -> str:
    return re.sub(r'^#(?:pragma once|include .*|define int long long)\s*$', '', text, flags=re.M)


def reconstruct_cases(bits: int) -> tuple[str, list[str]]:
    data, expected = [], []
    for m in range(3, 250):
        for b in range(1, math.isqrt((m - 1) // 2) + 1):
            answers = {}
            for q in range(1, b + 1):
                if math.gcd(q, m) != 1:
                    continue
                for p in range(-b, b + 1):
                    if math.gcd(p, q) == 1:
                        a = p * pow(q, -1, m) % m
                        assert a not in answers
                        answers[a] = f"{p}/{q}"
            for a in range(m):
                data.append(f"{a} {m} {b}\n")
                expected.append(answers.get(a, "N"))
    rng = random.Random(14002)
    limit = (1 << (bits - 1)) - 1
    for _ in range(400):
        m = limit - rng.randrange(100000)
        b = math.isqrt((m - 1) // 2)
        q = rng.randrange(1, b + 1)
        while math.gcd(q, m) != 1:
            q = rng.randrange(1, b + 1)
        p = rng.randrange(-b, b + 1)
        f = Fraction(p, q)
        data.append(f"{p * pow(q, -1, m) % m} {m} {b}\n")
        expected.append(f"{f.numerator}/{f.denominator}")
    return str(len(data)) + "\n" + "".join(data), expected


def expression_cases(bits: int) -> list[tuple[str, list[str], int | None]]:
    rng = random.Random(14003)
    hi, lo = (1 << (bits - 1)) - 1, -(1 << (bits - 1))

    def gen(depth: int) -> tuple[str, list[str], int | None]:
        if not depth or rng.randrange(4) == 0:
            x = rng.choice([rng.randrange(10000), hi, hi + 1, hi + 2])
            s = "0" * rng.randrange(3) + str(x)
            return s, [s], x if x <= hi + 1 else None
        if rng.randrange(3) == 0:
            s, post, x = gen(depth - 1)
            signs = "".join(rng.choice("+-") for _ in range(rng.randrange(1, 8)))
            neg = signs.count("-") % 2
            val = (-x if neg else x) if x is not None else None
            if val is not None and not lo <= val <= hi:
                val = None
            return " ".join(signs) + "(" + s + ")", post + ["u-" if neg else "u+"], val
        s, left, x = gen(depth - 1)
        t, right, y = gen(depth - 1)
        op = rng.choice("+-*/")
        val = None
        if x is not None and y is not None and lo <= x <= hi and lo <= y <= hi:
            if op == "+": val = x + y
            if op == "-": val = x - y
            if op == "*": val = x * y
            if op == "/" and y:
                val = (abs(x) // abs(y)) * (-1 if (x < 0) != (y < 0) else 1)
            if val is not None and not lo <= val <= hi:
                val = None
        return "(" + s + " " + op + " " + t + ")", left + right + [op], val

    cases = [
        ("2+3*4", ["2", "3", "4", "*", "+"], 14),
        ("8/3*3", ["8", "3", "/", "3", "*"], 6),
        ("8-3-2", ["8", "3", "-", "2", "-"], 3),
        ("2*-3", ["2", "3", "u-", "*"], -6),
        (" 1 +\t2\t", ["1", "2", "+"], 3),
        ("-" + str(hi + 1), [str(hi + 1), "u-"], lo),
        ("-(" + str(hi + 1) + ")", [str(hi + 1), "u-"], lo),
        ("--" + str(hi + 1), [str(hi + 1), "u+"], None),
        (str(hi + 1), [str(hi + 1)], None),
        (str(hi) + "+1-1", [str(hi), "1", "+", "1", "-"], None),
        ("(-" + str(hi + 1) + ")/-1", [str(hi + 1), "u-", "1", "u-", "/"], None),
        ("1/0", ["1", "0", "/"], None),
        ("9" * 10000, ["9" * 10000], None),
        ("-" * 10000 + "1", ["1", "u+"], 1),
        ("1" + "+1" * 10000, ["1", "1", "+"] + ["1", "+"] * 9999, 10001),
        ("(" * 300 + "1" + ")" * 300, ["1"], 1),
    ]
    for _ in range(800):
        s, post, val = gen(5)
        if val is not None and val > hi:
            val = None
        cases.append((s, post, val))
    return cases


BODY = r"""
signed main()
{
    static_assert(sizeof(int) == WIDTH);
    int n;
    cin >> n;
    while (n--)
    {
        int a, m, b;
        cin >> a >> m >> b;
        auto f = ratRecover(a, m, b);
        if (f) cout << f->x << '/' << f->y << '\n';
        else cout << "N\n";
    }
    cin >> n;
    while (n--)
    {
        int p, q, r, s;
        cin >> p >> q >> r >> s;
        Frac a(p, q), b(r, s);
        for (const Frac &f : {a+b, a-b, a*b, a/b})
        {
            cout << f.x << '/' << f.y << ' ';
        }
        cout << (a < b) << '\n';
    }
    Expr e;
    assert(e.a.empty() && e.rt == -1 && !e.value() && e.postfix().empty());
    string s;
    getline(cin, s);
    while (getline(cin, s))
    {
        bool ok = e.parse(s);
        cout << ok << '|';
        if (!ok)
        {
            assert(e.a.empty() && e.rt == -1 && e.postfix().empty() && !e.value());
            assert(!e.eval(s));
            cout << '\n';
            continue;
        }
        assert(e.rt == (int)e.a.size() - 1);
        vector<int> deg(e.a.size());
        for (int i = 0; i < (int)e.a.size(); i++)
        {
            const auto &[tok, ch] = e.a[i];
            for (int x : ch)
            {
                assert(x >= -1 && x < i);
                if (x >= 0) deg[x]++;
            }
            if (ch[0] < 0)
            {
                assert(ch[1] < 0 && !tok.empty());
                for (char c : tok) { assert(c >= '0' && c <= '9'); }
            }
            else if (ch[1] < 0) assert(tok == "u+" || tok == "u-");
            else assert(tok == "+" || tok == "-" || tok == "*" || tok == "/");
        }
        for (int i = 0; i < (int)deg.size(); i++) { assert(deg[i] == (i != e.rt)); }
        for (const string &tok : e.postfix()) { cout << tok << ' '; }
        cout << '|';
        auto v = e.value();
        if (v) cout << *v;
        else cout << 'N';
        assert(e.eval(s) == v);
        cout << '\n';
    }
}
"""


def main() -> None:
    chapter = (ROOT / "14-其他实用模板.md").read_text(encoding="utf-8")
    sources = {}
    for branch in ("full", "print"):
        sources[branch] = "\n".join(code((ROOT / "include" / branch / f"14_1_{t}.hpp").read_text(encoding="utf-8"))
                                     for t in ("002", "003"))
    sources["markdown"] = "\n".join(re.search(rf'^### 14\.1\.{t} .*?^```cpp\n(.*?)^```', chapter, re.M | re.S)[1]
                                           for t in ("002", "003"))
    bad = ["", " ", "1+", "()", "(1+2", "1+2)", "1a", "1 2", "2(3)", "*1", "1//2", "1**2", "(+)1", "++", ".5", "2^3"]
    rng = random.Random(14002)
    frac_data, frac_expected = [], []
    for _ in range(500):
        p, q, r, s = [rng.randrange(-100, 101) or 1 for _ in range(4)]
        a, b = Fraction(p, q), Fraction(r, s)
        frac_data.append(f"{p} {q} {r} {s}\n")
        frac_expected.append(" ".join(f"{x.numerator}/{x.denominator}" for x in (a+b, a-b, a*b, a/b)) + f" {int(a<b)}")
    with tempfile.TemporaryDirectory(prefix="ch14-rational-expr-") as temp:
        for enabled in (False, True):
            bits = 64 if enabled else 32
            rational, expected_rat = reconstruct_cases(bits)
            expr = expression_cases(bits)
            expected_expr = ["1|" + " ".join(post) + " |" + (str(val) if val is not None else "N") for _, post, val in expr]
            expected_expr += ["0|"] * len(bad)
            data = rational + str(len(frac_data)) + "\n" + "".join(frac_data)
            data += "\n".join(s for s, _, _ in expr) + "\n" + "\n".join(bad) + "\n"
            expected = expected_rat + frac_expected + expected_expr
            for branch, source in sources.items():
                stem = f"{branch}_{int(enabled)}"
                path = Path(temp) / (stem + ".cpp")
                exe = path.with_suffix(".exe")
                prefix = '#include <bits/stdc++.h>\nusing namespace std;\n' + ('#define int long long\n' if enabled else '') + f'#define WIDTH {bits//8}\n'
                path.write_text(prefix + source + BODY, encoding="utf-8")
                result = subprocess.run([compiler(), "-std=c++20", "-O2", *([] if os.name == "nt" else ["-fsanitize=undefined", "-fno-sanitize-recover=all"]), str(path), "-o", str(exe)], capture_output=True, timeout=180)
                assert result.returncode == 0, (stem, result.stderr.decode(errors="replace"))
                result = subprocess.run([str(exe)], input=data, text=True, capture_output=True, timeout=120)
                assert result.returncode == 0, (stem, result.stderr)
                actual = result.stdout.splitlines()
                if actual != expected:
                    i = next((i for i, (a,b) in enumerate(zip(actual, expected)) if a != b), min(len(actual), len(expected)))
                    raise AssertionError((stem, i, actual[i:i+1], expected[i:i+1]))
                print(f"{stem}: {len(expected_rat)} reconstructions, 500 fractions, {len(expected_expr)} expressions OK", flush=True)
        # Existing caller integration, using unchanged test source and real recursive includes.
        for branch in ("full", "print"):
            path = Path(temp) / f"core_{branch}.cpp"
            path.write_text(branch_source(ROOT / "tests/test_ch14_core.cpp", branch), encoding="utf-8")
            exe = path.with_suffix(".exe")
            subprocess.run([compiler(), "-std=c++20", "-O2", str(path), "-o", str(exe)], check=True, timeout=180)
            subprocess.run([str(exe)], check=True, timeout=120)
            print(f"{branch}: existing Chapter 14 callers OK", flush=True)


if __name__ == "__main__":
    main()
