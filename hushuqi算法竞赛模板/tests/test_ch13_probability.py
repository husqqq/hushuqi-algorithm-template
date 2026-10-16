"""Check probability APIs through real headers, Markdown and legacy aggregates."""
from __future__ import annotations

import decimal
import math
import os
import random
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.dont_write_bytecode = True
sys.path.insert(0, str(ROOT / "tools"))
from check_test_variants import branch_source, compiler
from test_ch14_widths import expand
from test_ch14_numeric import coeff, unhex

D = decimal.Decimal
decimal.getcontext().prec = 200

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
    int mode, n;
    while (cin >> mode >> n)
    {
        vector<long double> p(n), v(n);
        for (auto &x : p)
        {
            string s; cin >> s; x = strtold(s.c_str(), nullptr);
        }
        for (auto &x : v)
        {
            string s; cin >> s; x = strtold(s.c_str(), nullptr);
        }
        auto ans = mode ? prob_formula::bayesLog(p, v) : prob_formula::bayes(p, v);
        for (auto x : ans)
        {
            assert(isfinite(x) && x >= 0 && x <= 1);
            cout << hexfloat << x << ' ';
        }
        cout << '\n';
    }
}
'''


def expected(mode, prior, values):
    p = list(map(unhex, prior))
    if not mode:
        w = [x * unhex(y) for x, y in zip(p, values)]
    else:
        logs = [None if y == '-inf' else unhex(y) for y in values]
        shift = max(y for x, y in zip(p, logs) if x and y is not None)
        # Decimal has a wider exponent range than the tested machine arithmetic.
        # The omitted terms are far below the absolute comparison tolerance.
        w = [D(0) if not x or y is None or y - shift < -2000
             else x * (y - shift).exp() for x, y in zip(p, logs)]
    total = sum(w)
    return [x / total for x in w]


def main():
    text = (ROOT / '13-博弈论、概率与杂项数学.md').read_text(encoding='utf-8')
    section = re.search(r'^### 13\.2\.001 .*?(?=^<a id="132002")', text, re.M | re.S)[0]
    implementation, example = re.findall(r'^```cpp\n(.*?)^```', section, re.M | re.S)
    sources = {b: expand(ROOT / 'include' / b / '13_2_001.hpp', set())
               for b in ('full', 'print')}
    sources['markdown'] = '#include <bits/stdc++.h>\nusing namespace std;\n#define int long long\n' + implementation
    cxx = compiler()
    with tempfile.TemporaryDirectory(prefix='ch13-probability-') as tmp:
        temp = Path(tmp)

        def build(name, source):
            src, exe = temp / (name + '.cpp'), temp / (name + '.exe')
            src.write_text(source, encoding='utf-8')
            command = [cxx, '-std=c++20', '-O2', str(src), '-o', str(exe)]
            if os.name == 'nt':
                command.append('-Wl,--stack,1073741824')
            subprocess.run(command, check=True, capture_output=True)
            return exe

        probe = build('probe', sources['full'] + BODY)
        bits, mine, maxe = map(int, subprocess.check_output([str(probe), 'probe'], text=True).split())
        assert bits >= 53
        e = maxe * 3 // 4
        tiny = coeff(1, mine - bits)
        huge = f'0x{(1 << bits) - 1:x}p{maxe - bits:+}'
        cases = [
            (0, ['0x1p-1', '0x1p-1'], ['0x1p+0', '0x4p+0']),
            (0, [coeff(1, -e), '0x1p+0'], [coeff(1, -e), '0x0p+0']),
            (0, [tiny, '0x1p+0'], [tiny, '0x0p+0']),
            (0, ['0x1p-1', '0x1p-1'], [huge, tiny]),
            (0, ['0x1p-2', '0x3p-2'], [tiny, tiny]),
            (1, ['0x1p-1', '0x1p-1'], [coeff(-10000), coeff(-10001)]),
            (1, ['0x1p-2', '0x3p-2'], [huge, huge]),
            (1, ['0x1p-2', '0x3p-2'], ['-' + huge, '-' + huge]),
            (1, ['0x0p+0', '0x1p-2', '0x3p-2'], [huge, coeff(-10000), coeff(-10000)]),
            (1, ['0x1p-2', '0x3p-2'], ['-inf', '0x0p+0']),
            (1, [tiny, '0x1p+0'], ['0x0p+0', '-inf']),
            (1, ['0x1p-1', '0x1p-1'], [huge, '-' + huge]),
        ]
        rng = random.Random(132001)
        for mode in (0, 1):
            for _ in range(1000):
                n = rng.randint(1, 20)
                cuts = sorted([0, 1024] + [rng.randrange(1025) for _ in range(n - 1)])
                prior = [coeff(b - a, -10) for a, b in zip(cuts, cuts[1:])]
                values = [coeff(rng.randint(1, 1024), rng.randint(-e, e)) if not mode
                          else coeff(rng.randint(-100000, 100000), rng.randint(-8, 8))
                          for _ in prior]
                cases.append((mode, prior, values))
        data = ''.join(f'{m} {len(p)}\n' + ' '.join(p) + '\n' + ' '.join(v) + '\n'
                       for m, p, v in cases)
        answers = [expected(*row) for row in cases]
        invalid = [
            '0 0\n\n\n', '0 1\n-0x1p+0\n0x1p+0\n',
            '0 1\n0x1p-1\n0x1p+0\n', '0 1\n0x1p+0\n0x0p+0\n',
            '0 1\n0x1p+0\n-0x1p+0\n', '0 1\nnan\n0x1p+0\n',
            '0 1\ninf\n0x1p+0\n', '0 1\n0x1p+0\ninf\n',
            '1 1\n0x1p+0\ninf\n', '1 1\n0x1p+0\nnan\n',
            '1 1\n0x1p+0\n-inf\n', '1 2\n0x0p+0 0x1p+0\n0x0p+0 -inf\n',
        ]
        for branch, source in sources.items():
            for macro in (False, True):
                code = source if macro else source.replace('#define int long long', '')
                name = f'{branch}_{macro}'
                exe = build(name, code + BODY)
                result = subprocess.run([str(exe)], input=data, text=True, capture_output=True, check=True)
                rows = result.stdout.splitlines()
                assert len(rows) == len(cases)
                for index, (row, answer) in enumerate(zip(rows, answers)):
                    got = list(map(unhex, row.split()))
                    assert len(got) == len(answer)
                    assert abs(sum(got) - 1) < D('1e-14'), (name, index)
                    for x, y in zip(got, answer):
                        assert abs(x - y) <= D('2e-12') * max(D('1e-100'), abs(y)), (name, index, x, y)
                for case in invalid:
                    assert subprocess.run([str(exe)], input=case, text=True, capture_output=True).returncode != 0
                mismatch = build(name + '_mismatch', code + '''
signed main() { prob_formula::bayes({1}, {1, 1}); }
''')
                assert subprocess.run([str(mismatch)], capture_output=True).returncode != 0
                sample = build(name + '_sample', code + example)
                subprocess.run([str(sample)], check=True, capture_output=True)
                print(f'AC {name}: {len(cases)} reference cases, 13 invalid inputs, fixed example', flush=True)

        for branch in ('full', 'print'):
            aggregate = ROOT / 'include' / ('supplement_math.hpp' if branch == 'full' else 'print/support/supplement_math.hpp')
            topics = [ROOT / 'include' / branch / f'13_2_00{i}.hpp' for i in (1, 2, 3)]
            for reverse in (False, True):
                seen = set()
                paths = [aggregate] + topics if not reverse else topics + [aggregate]
                code = ''.join(expand(p, seen) for p in paths)
                exe = build(f'aggregate_{branch}_{reverse}', code + '''
signed main()
{
    auto v = exp_var::moments({1E18L, 1E18L + 1}, {0.5L, 0.5L});
    assert(fabsl(v[1] - 0.25L) < 1E-12L);
    auto p = prob_formula::bayesLog({0.25L, 0.75L}, {-10000, -10000});
    assert(fabsl(p[0] - 0.25L) < 1E-12L);
    assert(probDist::geomMoments(0.5L)[0] == 2);
    cout << hexfloat << probDist::binomialPmf(100000, 50000, 0.5L);
}
''')
                got = unhex(subprocess.check_output([str(exe)], text=True).strip())
                exact = D(math.comb(100000, 50000)) / D(2) ** 100000
                assert abs(got - exact) < exact * D('1e-11'), (got, exact)
                print(f'AC aggregate {branch}, reverse={reverse}', flush=True)
        for name in ('test_ch13.cpp', 'test_supplements.cpp'):
            for branch in ('full', 'print'):
                exe = build(name + branch, branch_source(ROOT / 'tests' / name, branch))
                subprocess.run([str(exe)], check=True, capture_output=True, timeout=120)
                print(f'AC regression {name} {branch}', flush=True)
    print('PROBABILITY_OK temporary directory cleaned', flush=True)


if __name__ == '__main__':
    main()
