"""Check quadratic scaling, integer square roots and queens through actual copy paths."""
from __future__ import annotations

import decimal
import math
import random
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.dont_write_bytecode = True
sys.path.insert(0, str(ROOT / "tools"))
from check_test_variants import compiler, branch_source
from test_ch14_widths import expand

D = decimal.Decimal
decimal.getcontext().prec = 200
decimal.getcontext().Emax = 999999
decimal.getcontext().Emin = -999999


def unhex(text: str) -> D:
    """Decode the exact binary value printed by the tested C++ program."""
    match = re.fullmatch(r"(-?)0x([0-9a-f]+)(?:\.([0-9a-f]+))?p([+-]?\d+)", text)
    assert match, text
    sign, whole, frac, exponent = match.groups()
    frac = frac or ""
    value = D(int(whole + frac, 16)) * D(2) ** (int(exponent) - 4 * len(frac))
    return -value if sign else value


def coeff(mantissa: int, exponent: int = 0) -> str:
    return ("-" if mantissa < 0 else "") + f"0x{abs(mantissa):x}p{exponent:+}"


def oracle(row: tuple[str, str, str]) -> tuple[list[D], D]:
    a, b, c = map(unhex, row)
    if not a:
        return ([-c / b] if b else []), D(0)
    # Decide the discriminant sign exactly; Decimal rounding must not turn a repeated root negative.
    parts = []
    for value in row:
        mantissa, exponent = value.split("p")
        parts.append((int(mantissa, 16), int(exponent)))
    base = min(e for m, e in parts if m)
    ai, bi, ci = (m << (e - base) if m else 0 for m, e in parts)
    disc = D(bi * bi - 4 * ai * ci) * D(2) ** (2 * base)
    if disc < 0:
        return [], abs(disc) / (b * b + abs(4 * a * c))
    if disc == 0:
        return [-b / (2 * a)], D(0)
    # Arbitrary precision and an unbounded exponent remove the machine-range issue.
    q = -(b + (disc.sqrt() if b >= 0 else -disc.sqrt())) / 2
    return sorted([q / a, c / q]), disc / (b * b + abs(4 * a * c))


BODY = r"""
signed main(signed argc, char **argv)
{
    if (argc > 1)
    {
        cout << numeric_limits<long double>::digits << ' '
             << numeric_limits<long double>::min_exponent << ' '
             << numeric_limits<long double>::max_exponent << '\n';
        return 0;
    }
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string sa, sb, sc;
        cin >> sa >> sb >> sc;
        long double a = strtold(sa.c_str(), nullptr);
        long double b = strtold(sb.c_str(), nullptr);
        long double c = strtold(sc.c_str(), nullptr);
        auto roots = quadRoots(a, b, c);
        cout << roots.size();
        for (auto x : roots)
        {
            assert(isfinite(x));
            cout << ' ' << hexfloat << x;
        }
        cout << '\n';
    }
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string s;
        cin >> s;
        cout << bigSqrt(s) << '\n';
    }
    // Check the permutation and both diagonals, including every small residue class.
    for (int n = 1; n <= 2000; n++)
    {
        auto p = nQueens(n);
        if (n == 2 || n == 3)
        {
            assert(p.empty());
            continue;
        }
        assert((int)p.size() == n);
        vector<char> col(n), diag(2 * n), anti(2 * n);
        for (int i = 0; i < n; i++)
        {
            assert(0 <= p[i] && p[i] < n);
            assert(!col[p[i]] && !diag[i + p[i]] && !anti[i - p[i] + n]);
            col[p[i]] = diag[i + p[i]] = anti[i - p[i] + n] = true;
        }
    }
    for (int n : {99999, 100000, 100001})
    {
        auto p = nQueens(n);
        vector<char> col(n), diag(2 * n), anti(2 * n);
        for (int i = 0; i < n; i++)
        {
            assert(0 <= p[i] && p[i] < n);
            assert(!col[p[i]] && !diag[i + p[i]] && !anti[i - p[i] + n]);
            col[p[i]] = diag[i + p[i]] = anti[i - p[i] + n] = true;
        }
    }
    auto near = quadRoots(1, 2, nextafterl(1, 2));
    assert(near.size() == 1 && near[0] == -1);
    cout << "QUEENS_AND_NEAR_ROOT_OK\n";
}
"""


def main() -> None:
    text = (ROOT / "14-其他实用模板.md").read_text(encoding="utf-8")
    topics = ("016", "017", "018")
    sources = {}
    for branch in ("full", "print"):
        seen: set[Path] = set()
        sources[branch] = "".join(expand(ROOT / "include" / branch / f"14_1_{t}.hpp", seen) for t in topics)
    sources["markdown"] = "\n".join(
        re.search(rf"^### 14\.1\.{t} .*?^```cpp\n(.*?)^```", text, re.M | re.S)[1] for t in topics
    )
    # The aggregate header must coexist with the standalone headers without duplicate definitions.
    for branch in ("full", "print"):
        seen = set()
        aggregate = ROOT / "include" / ("extra_topics.hpp" if branch == "full" else "print/support/extra_topics.hpp")
        sources["aggregate_" + branch] = expand(aggregate, seen) + "".join(
            expand(ROOT / "include" / branch / f"14_1_{t}.hpp", seen) for t in topics
        )
        seen = set()
        sources["aggregate_topic_" + branch] = expand(ROOT / "include" / branch / "6_1_010.hpp", seen) + "".join(
            expand(ROOT / "include" / branch / f"14_1_{t}.hpp", seen) for t in topics
        )
    rng = random.Random(14017)
    square_inputs = ["0", "000000", "1", "2", "9999"]
    for digits in (1, 2, 3, 9, 30, 100, 500):
        for _ in range(12):
            root = rng.randrange(10 ** (digits - 1), 10 ** digits)
            square_inputs.extend("000" + str(root * root + delta) for delta in (-1, 0, 1))
    expected_squares = [str(math.isqrt(int(s))) for s in square_inputs]
    with tempfile.TemporaryDirectory(prefix="ch14-numeric-") as temp:
        for branch, source in sources.items():
            source = re.sub(r"^#define int long long\s*$", "", source, flags=re.M)
            settings = (True,) if branch.startswith("aggregate_") else (False, True)
            for enabled in settings:
                stem = f"{branch}_{int(enabled)}"
                path = Path(temp) / (stem + ".cpp")
                exe = path.with_suffix(".exe")
                prefix = "#include <bits/stdc++.h>\nusing namespace std;\n" + ("#define int long long\n" if enabled else "")
                path.write_text(prefix + source + BODY, encoding="utf-8")
                result = subprocess.run([compiler(), "-std=c++20", "-O2", str(path), "-o", str(exe)], capture_output=True, timeout=180)
                assert result.returncode == 0, (stem, result.stderr.decode(errors="replace"))
                bits, low_exp, high_exp = map(int, subprocess.check_output([str(exe), "limits"], text=True).split())
                p = high_exp * 3 // 4
                cases = [(coeff(a), coeff(b), coeff(c)) for a, b, c in
                         ((0, 0, 0), (0, 0, 1), (0, 2, -4), (1, 0, 0), (1, 2, 1),
                          (1, 0, 1), (1, -5, 6), (-1, 0, 1))]
                # Both failures reproduced in V1.3.59, plus overflow and subnormal boundaries.
                cases += [(coeff(1), coeff(1, -p), coeff(0)),
                          (coeff(1, p), coeff(0), coeff(-1, -p)),
                          (coeff(1, low_exp - bits), coeff(0), coeff(-1, low_exp - bits)),
                          (coeff(1, high_exp - 2), coeff(-1, high_exp - 2), coeff(1, high_exp - 4))]
                for shift in (-p, 0, p):
                    cases.append((coeff(1, shift), coeff(-3, shift), coeff(2, shift)))
                local_rng = random.Random(1401701)
                for _ in range(1000):
                    row = tuple(coeff(local_rng.randrange(1, 1024) * local_rng.choice((-1, 1)),
                                      local_rng.randrange(-p, p)) for _ in range(3))
                    roots, separation = oracle(row)
                    # Exclude roots outside the documented finite range and ill-conditioned near roots.
                    bound = D(2) ** (high_exp - 2)
                    if any(abs(x) > bound for x in roots) or (separation and separation < D("1e-10")):
                        continue
                    cases.append(row)
                expected = [oracle(row)[0] for row in cases]
                data = f"{len(cases)}\n" + "\n".join(" ".join(row) for row in cases)
                data += f"\n{len(square_inputs)}\n" + "\n".join(square_inputs) + "\n"
                result = subprocess.run([str(exe)], input=data, text=True, capture_output=True, timeout=180)
                assert result.returncode == 0, (stem, result.stderr)
                lines = result.stdout.splitlines()
                assert len(lines) == len(cases) + len(square_inputs) + 1, stem
                tiny = D(2) ** (low_exp - bits)
                for i, want in enumerate(expected):
                    fields = lines[i].split()
                    got = list(map(unhex, fields[1:]))
                    assert int(fields[0]) == len(got) == len(want), (stem, i, cases[i], fields, want)
                    for actual, reference in zip(got, want):
                        assert abs(actual - reference) <= D("2e-14") * abs(reference) + tiny, (stem, i, cases[i], actual, reference)
                assert lines[len(cases):-1] == expected_squares, stem
                assert lines[-1] == "QUEENS_AND_NEAR_ROOT_OK", stem
                print(f"{stem}: {len(cases)} quadratic, {len(square_inputs)} square roots, 2003 queens OK", flush=True)
        for branch in ("full", "print"):
            for name in ("test_ch14_extra.cpp", "test_ch14_decimal.cpp", "test_ch14_pi.cpp", "test_extra_topics.cpp"):
                path = Path(temp) / (branch + "_" + name)
                path.write_text(branch_source(ROOT / "tests" / name, branch), encoding="utf-8")
                exe = path.with_suffix(".exe")
                subprocess.run([compiler(), "-std=c++20", "-O2", str(path), "-o", str(exe)], check=True, capture_output=True, timeout=180)
                subprocess.run([str(exe)], check=True, capture_output=True, timeout=180)
                print(f"{branch}: {name} OK", flush=True)


if __name__ == "__main__":
    main()
