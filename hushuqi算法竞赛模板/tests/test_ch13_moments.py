"""Compare real probability-moment entry points with exact rational arithmetic."""
from __future__ import annotations

import os
import random
import re
import subprocess
import sys
import tempfile
from fractions import Fraction as F
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.dont_write_bytecode = True
sys.path.insert(0, str(ROOT / 'tools'))
from check_test_variants import branch_source, compiler
from test_ch14_widths import expand


def hx(m: int, e: int = 0) -> str:
    return ('-' if m < 0 else '') + f'0x{abs(m):x}p{e:+}'


def exact(s: str) -> F:
    match = re.fullmatch(r'(-?)0x([0-9a-f]+)(?:\.([0-9a-f]+))?p([+-]?\d+)', s)
    assert match, s
    sign, whole, frac, exponent = match.groups()
    frac = frac or ''
    exponent = int(exponent) - 4 * len(frac)
    mantissa = int(whole + frac, 16) * (-1 if sign else 1)
    return F(mantissa << exponent) if exponent >= 0 else F(mantissa, 1 << -exponent)


def oracle(xs: list[str], ps: list[str]) -> tuple[F, F]:
    rows = [(exact(x), exact(p)) for x, p in zip(xs, ps) if exact(p)]
    total = sum(p for _, p in rows)
    mean = sum(x * p for x, p in rows) / total
    # Unlike machine floating point, Fraction evaluates the cancelling squares exactly.
    var = sum(x * x * p for x, p in rows) / total - mean * mean
    return mean, var


BODY = r'''
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
    while (cin >> n)
    {
        vector<long double> x(n), p(n);
        for (auto &v : x)
        {
            string s; cin >> s; v = strtold(s.c_str(), nullptr);
        }
        for (auto &v : p)
        {
            string s; cin >> s; v = strtold(s.c_str(), nullptr);
        }
        auto ans = exp_var::moments(x, p);
        cout << hexfloat << ans[0] << ' ' << ans[1] << '\n';
    }
    vector<long double> x(100000), p(100000, 1E-5L);
    long double base = ldexpl(1, numeric_limits<long double>::digits - 5);
    for (int i = 0; i < (int)x.size(); i++)
    {
        x[i] = base + i % 2;
    }
    auto large = exp_var::moments(x, p);
    assert(base <= large[0] && large[0] <= base + 1);
    assert(fabsl(large[1] - 0.25L) < 2E-13L);
}
'''


def main():
    if os.name == 'nt':
        import ctypes
        ctypes.windll.kernel32.SetErrorMode(0x8003)
    text = (ROOT / '13-博弈论、概率与杂项数学.md').read_text(encoding='utf-8')
    section = re.search(r'^### 13\.2\.003 .*?(?=^<a id="132004")', text, re.M | re.S)[0]
    code, example = re.findall(r'^```cpp\n(.*?)^```', section, re.M | re.S)
    sources = {b: expand(ROOT / 'include' / b / '13_2_003.hpp', set()) for b in ('full', 'print')}
    sources['markdown'] = '#include <bits/stdc++.h>\nusing namespace std;\n#define int long long\n' + code
    cxx = compiler()
    with tempfile.TemporaryDirectory(prefix='ch13-moments-') as tmp:
        temp = Path(tmp)

        def build(name, code):
            src = temp / (name + '.cpp'); exe = src.with_suffix('.exe')
            src.write_text(code, encoding='utf-8')
            cmd = [cxx, '-std=c++20', '-O2', str(src), '-o', str(exe)]
            if os.name == 'nt':
                cmd.append('-Wl,--stack,1073741824')
            result = subprocess.run(cmd, capture_output=True, timeout=180)
            assert result.returncode == 0, result.stderr.decode('utf-8', errors='replace')
            return exe

        probe = build('probe', sources['full'] + BODY)
        bits, mine, maxe = map(int, subprocess.check_output([str(probe), 'probe'], text=True).split())
        tiny = hx(1, mine - bits)
        huge = hx((1 << bits) - 1, maxe - bits)
        half = hx((1 << bits) - 1, maxe - bits - 1)
        offset = 1 << (bits - 5)
        cases = [
            ([hx(offset), hx(offset + 1)], [hx(1, -5), hx(31, -5)]),
            ([hx(offset + 1), hx(offset)], [hx(31, -5), hx(1, -5)]),
            ([hx(0), hx(2)], [hx(1, -2), hx(3, -2)]),
            ([huge], [hx(1)]), ([tiny], [hx(1)]),
            ([huge, huge], [hx(1, -1)] * 2),
            ([huge, '-' + huge], [hx(1, -1)] * 2),
            (['-' + half, half], [tiny, hx(1)]),
            ([half, '-' + half], [hx(1), tiny]),
            ([hx(0), hx(1, maxe // 2 + 100)], [hx(1), tiny]),
            (['nan', 'inf', tiny], [hx(0), hx(0), hx(1)]),
            ([hx(-7)], [hx((1 << 42) + 1, -42)]),
            ([hx(0), tiny], [hx(1, -1)] * 2),
            # Exact mean 3/8 is smaller than the rounded large products' spacing.
            ([hx(10 * (1 << (bits - 4)) + 1), hx(-6 * (1 << (bits - 4)))], [hx(3, -3), hx(5, -3)]),
        ]
        for e in (0, 100, maxe // 2 - 5, maxe - 2):
            for last in (hx(1), hx(1, -100), tiny):
                cases.append(([hx(1, e), hx(-1, e), last], [hx(1, -2), hx(1, -2), hx(1, -1)]))
        rng = random.Random(132003)
        for kind in range(3):
            for _ in range(600):
                n = rng.randint(1, 20)
                cuts = sorted([0, 1024] + [rng.randrange(1025) for _ in range(n - 1)])
                ps = [hx(b - a, -10) for a, b in zip(cuts, cuts[1:])]
                if kind == 0:
                    xs = [hx(rng.randint(-1024, 1024), rng.randint(-32, 32)) for _ in ps]
                elif kind == 1:
                    base = offset * rng.choice((-1, 1))
                    xs = [hx(base + rng.randint(-100, 100)) for _ in ps]
                else:
                    xs = [hx(rng.randint(-1023, 1023), rng.randint(mine - bits, maxe - 10)) for _ in ps]
                cases.append((xs, ps))
        # Every positive probability is paired with a finite, exactly representable binary value.
        answers = [oracle(*row) for row in cases]
        data = ''.join(f'{len(x)}\n' + ' '.join(x) + '\n' + ' '.join(p) + '\n' for x, p in cases)
        invalid = ['0\n\n\n', '1\n0x1p+0\n0x0p+0\n',
                   '1\n0x1p+0\n-0x1p+0\n', '1\n0x1p+0\n0x1p-1\n',
                   '1\n0x1p+0\nnan\n', '1\n0x1p+0\ninf\n',
                   '1\nnan\n0x1p+0\n', '1\ninf\n0x1p+0\n']
        configurations = dict(sources)
        for branch in ('full', 'print'):
            aggregate = ROOT / 'include' / ('supplement_math.hpp' if branch == 'full' else 'print/support/supplement_math.hpp')
            topic = ROOT / 'include' / branch / '13_2_003.hpp'
            for reverse in (False, True):
                seen = set()
                order = [topic, aggregate] if reverse else [aggregate, topic]
                configurations[f'aggregate_{branch}_{reverse}'] = ''.join(expand(p, seen) for p in order)
        for name, source in configurations.items():
            macros = (False, True) if name in sources else (True,)
            for macro in macros:
                variant = source if macro else source.replace('#define int long long', '')
                label = f'{name}_{macro}'
                exe = build(label, variant + BODY)
                result = subprocess.run([str(exe)], input=data, text=True, capture_output=True, check=True, timeout=90)
                rows = result.stdout.splitlines()
                assert len(rows) == len(cases), label
                for index, (line, want) in enumerate(zip(rows, answers)):
                    fields = line.split()
                    assert len(fields) == 2, (label, index, fields)
                    actual_mean = exact(fields[0])
                    assert abs(actual_mean - want[0]) <= max(exact(tiny) * 8, abs(want[0]) / 10**13), (label, index, cases[index], fields)
                    if fields[1] == 'inf':
                        assert want[1] >= exact(huge) * F(999999999999, 10**12), (label, index)
                    else:
                        actual_var = exact(fields[1])
                        assert actual_var >= 0
                        assert abs(actual_var - want[1]) <= max(exact(tiny) * 8, want[1] / 10**12), (label, index, cases[index], fields)
                if name in sources:
                    for case in invalid:
                        assert subprocess.run([str(exe)], input=case, text=True, capture_output=True).returncode != 0
                    mismatch = build(label + '_mismatch', variant + 'signed main(){exp_var::moments({1},{1,0});}\n')
                    assert subprocess.run([str(mismatch)], capture_output=True).returncode != 0
                    sample = build(label + '_sample', variant + example)
                    subprocess.run([str(sample)], check=True, capture_output=True)
                print(f'AC {label}: {len(cases)} exact-rational cases, 100000-item fixed array', flush=True)
        for name in ('test_ch13.cpp', 'test_supplements.cpp'):
            for branch in ('full', 'print'):
                exe = build(name + branch, branch_source(ROOT / 'tests' / name, branch))
                subprocess.run([str(exe)], check=True, capture_output=True, timeout=120)
                print(f'AC regression {name} {branch}', flush=True)
    print('MOMENTS_OK temporary directory cleaned', flush=True)


if __name__ == '__main__':
    main()
