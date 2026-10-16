"""Check binomial probabilities against independent high precision log Gamma."""
from __future__ import annotations

import math
import os
import random
import re
import subprocess
import tempfile
from pathlib import Path

import mpmath as mp

from test_ch13_moments import exact, hx
from test_ch14_widths import expand
from check_test_variants import branch_source, compiler

ROOT = Path(__file__).resolve().parents[1]
mp.mp.dps = 160

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
    int n, k;
    string s;
    while (cin >> n >> k >> s)
    {
        long double p = strtold(s.c_str(), nullptr);
        cout << hexfloat << binomialPmf(n, k, p) << ' ' << binomialPmf(n, k, p, true) << '\n';
    }
}
'''


def real(value):
    f = exact(value)
    return mp.mpf(f.numerator) / f.denominator


def reference(n, k, p):
    p = real(p)
    if k < 0 or k > n or (p == 0 and k != 0) or (p == 1 and k != n):
        return mp.ninf
    if p in (0, 1):
        return mp.mpf(0)
    if n <= 500:
        choose = mp.log(math.comb(n, k))
    else:
        # Direct log Gamma subtraction is safe here with 160 decimal digits.
        choose = mp.loggamma(n + 1) - mp.loggamma(k + 1) - mp.loggamma(n - k + 1)
    return choose + k * mp.log(p) + (n - k) * mp.log1p(-p)


def main():
    if os.name == 'nt':
        import ctypes
        ctypes.windll.kernel32.SetErrorMode(0x8003)
    doc = (ROOT / '13-博弈论、概率与杂项数学.md').read_text(encoding='utf-8')
    section = re.search(r'^### 13\.2\.002 .*?(?=^<a id="132003")', doc, re.M | re.S)[0]
    code, example = re.findall(r'^```cpp\n(.*?)^```', section, re.M | re.S)
    sources = {b: expand(ROOT / 'include' / b / '13_2_002.hpp', set()) for b in ('full', 'print')}
    sources['markdown'] = '#include <bits/stdc++.h>\nusing namespace std;\n#define int long long\n' + code
    with tempfile.TemporaryDirectory(prefix='ch13-binomial-') as work:
        temp = Path(work)

        def build(name, source, release=False):
            src = temp / (name + '.cpp')
            exe = src.with_suffix('.exe')
            src.write_text(source, encoding='utf-8')
            command = [compiler(), '-std=c++20', '-O2', str(src), '-o', str(exe)]
            if release:
                command.append('-DNDEBUG')
            result = subprocess.run(command, capture_output=True, timeout=180)
            assert result.returncode == 0, result.stderr.decode(errors='replace')
            return exe

        exe = build('probe', sources['full'] + BODY)
        bits, mine, maxe = map(int, subprocess.check_output([str(exe), 'probe'], text=True).split())
        tiny = hx(1, mine - bits)
        cases = []
        for n in range(17):
            for k in range(-1, n + 2):
                for p in (hx(0), hx(1), hx(1, -1), hx(1, -5), hx(3, -2)):
                    cases.append((n, k, p))
        rng = random.Random(132002)
        for _ in range(400):
            n = rng.randint(1, 500)
            cases.append((n, rng.randint(0, n), hx(rng.randint(1, (1 << 20) - 1), -20)))
        sizes = (10**6, 10**9, (1 << 31) - 1, 10**12, 10**15, 10**18, (1 << 63) - 1)
        for n in sizes:
            for p in (hx(1, -1), hx(1, -5), hx(31, -5), hx(349525, -20),
                      hx((1 << (bits - 1)) + 1, -bits), hx((1 << bits) - 1, -bits)):
                value = real(p)
                mean = n * value
                sd = mp.sqrt(n * value * (1 - value))
                for shift in (-80, -8, -1, 0, 1, 8, 80):
                    k = min(n, max(0, int(mp.nint(mean + shift * sd))))
                    cases.append((n, k, p))
                for k in (0, 1, n - 1, n):
                    cases.append((n, k, p))
            for p in (tiny, hx(1, -maxe // 2), hx(1, -100)):
                for k in (0, 1, 2, n):
                    cases.append((n, k, p))
        cases.extend((n, k, tiny) for n in (1, 2, 16, 32) for k in range(min(n + 1, 4)))
        for _ in range(400):
            n = rng.randint(1, (1 << 63) - 1)
            p = hx(rng.randint(1, (1 << bits) - 1), -bits)
            value = real(p)
            shift = rng.choice((-80, -8, -1, 0, 1, 8, 80))
            k = int(mp.nint(n * value + shift * mp.sqrt(n * value * (1 - value))))
            cases.append((n, min(n, max(0, k)), p))
        answers = {case: reference(*case) for case in cases}
        floor = real(tiny)
        logfloor = mp.log(floor)
        configs = dict(sources)
        for branch in ('full', 'print'):
            aggregate = ROOT / 'include' / ('supplement_math.hpp' if branch == 'full' else 'print/support/supplement_math.hpp')
            topic = ROOT / 'include' / branch / '13_2_002.hpp'
            for reverse in (False, True):
                seen = set()
                paths = [topic, aggregate] if reverse else [aggregate, topic]
                configs[f'aggregate_{branch}_{reverse}'] = ''.join(expand(p, seen) for p in paths)
        invalid = ('-1 0 0x1p-1\n', '5 2 -0x1p-1\n', '5 2 0x1p+1\n', '5 2 nan\n', '5 2 inf\n')
        for name, source in configs.items():
            for macro in ((False, True) if name in sources else (True,)):
                variant = source if macro else source.replace('#define int long long', '')
                selected = [case for case in cases if case[0] <= ((1 << (63 if macro else 31)) - 1)]
                data = ''.join(f'{n} {k} {p}\n' for n, k, p in selected)
                for release in ((False, True) if name in sources else (False,)):
                    label = f'{name}_{macro}_{release}'
                    exe = build(label, variant + BODY, release)
                    result = subprocess.run([str(exe)], input=data, text=True, capture_output=True, timeout=90, check=True)
                    rows = result.stdout.splitlines()
                    assert len(rows) == len(selected), label
                    for index, (row, case) in enumerate(zip(rows, selected)):
                        value, logvalue = row.split()
                        prob = real(value)
                        assert 0 <= prob <= 1, (label, index, case, row)
                        want = answers[case]
                        if want == mp.ninf:
                            assert prob == 0 and logvalue == '-inf', (label, index, case)
                            continue
                        got = real(logvalue)
                        assert abs(got - want) <= mp.mpf('2e-13') * max(1, abs(want)), (label, index, case, row, mp.nstr(want, 30))
                        if want < logfloor - 4:
                            assert prob == 0, (label, index, case)
                        else:
                            target = mp.exp(want)
                            assert abs(prob - target) <= max(target * mp.mpf('2e-12'), floor * mp.mpf('0.6')), (label, index, case, row, mp.nstr(target, 30))
                    if name in sources and not release:
                        for bad in invalid:
                            assert subprocess.run([str(exe)], input=bad, text=True, capture_output=True, timeout=10).returncode != 0
                        sample = build(label + '_sample', variant + example)
                        subprocess.run([str(sample)], check=True, capture_output=True, timeout=30)
                    print(f'AC {label}: {len(selected)} high precision cases', flush=True)
        for branch in ('full', 'print'):
            exe = build('fixed_' + branch, branch_source(ROOT / 'tests/test_ch13_binomial.cpp', branch))
            subprocess.run([str(exe)], capture_output=True, check=True, timeout=30)
            print('AC fixed', branch, flush=True)
    print('BINOMIAL_OK temporary directory cleaned', flush=True)


if __name__ == '__main__':
    main()
