"""Check the actual online statistics entries against exact rational moments."""
from __future__ import annotations

import os
import random
import re
import subprocess
import tempfile
from fractions import Fraction as F
from pathlib import Path

from test_ch13_moments import exact, hx
from test_ch14_widths import expand
from check_test_variants import branch_source, compiler

ROOT = Path(__file__).resolve().parents[1]

BODY = r'''
signed main(signed argc, char **argv)
{
    if (argc > 1)
    {
        cout << numeric_limits<long double>::digits << ' '
             << numeric_limits<long double>::max_exponent << '\n';
        return 0;
    }
    auto emit = [](const exp_var::OnlineMom &a)
    {
        // a 是统计结果；向标准输出写入权重、均值和总体方差，无返回值。
        if (auto r = a.get()) cout << hexfloat << a.w << ' ' << (*r)[0] << ' ' << (*r)[1] << '\n';
        else cout << "empty\n";
    };
    int n;
    while (cin >> n)
    {
        vector<long double> x(n), w(n);
        for (auto &v : x)
        {
            string s; cin >> s; v = strtold(s.c_str(), nullptr);
        }
        for (auto &v : w)
        {
            string s; cin >> s; v = strtold(s.c_str(), nullptr);
        }
        exp_var::OnlineMom a, b, c;
        for (int i = 0; i < n; i++)
        {
            a.add(x[i], w[i]);
            emit(a);
            (i < n / 2 ? b : c).add(x[i], w[i]);
        }
        auto saved = c;
        b.merge(c);
        if (c.w != saved.w || c.base != saved.base || c.mean != saved.mean || c.m2 != saved.m2) return 2;
        emit(b);
        a = {};
        for (int i = n; i-- > 0;)
        {
            a.add(x[i], w[i]);
        }
        emit(a);
        auto tree = [&](auto &&self, int l, int r) -> exp_var::OnlineMom
        {
            // self 是递归自身，[l,r) 是观测范围；返回平衡树次序的合并统计。
            exp_var::OnlineMom ans;
            if (l == r) return ans;
            if (r - l == 1) ans.add(x[l], w[l]);
            else
            {
                int m = (l + r) / 2;
                ans = self(self, l, m);
                ans.merge(self(self, m, r));
            }
            return ans;
        };
        a = tree(tree, 0, n);
        emit(a);
        auto prev = a.get();
        a.merge({});
        if (a.get() != prev) return 3;
        a.merge(a);
        emit(a);
        exp_var::OnlineMom empty;
        empty.merge(a);
        if (empty.get() != a.get() || empty.w != a.w) return 4;
    }
}
'''


def main():
    if os.name == 'nt':
        import ctypes
        ctypes.windll.kernel32.SetErrorMode(0x8003)
    text = (ROOT / '13-博弈论、概率与杂项数学.md').read_text(encoding='utf-8')
    section = re.search(r'^### 13\.2\.008 .*?(?=^<a id="133")', text, re.M | re.S)[0]
    code, example = re.findall(r'^```cpp\n(.*?)^```', section, re.M | re.S)
    sources = {b: expand(ROOT / 'include' / b / '13_2_008.hpp', set()) for b in ('full', 'print')}
    sources['markdown'] = '#include <bits/stdc++.h>\nusing namespace std;\n#define int long long\n' + code
    with tempfile.TemporaryDirectory(prefix='ch13-online-') as tmp:
        temp = Path(tmp)

        def build(name, source, release=False):
            src = temp / (name + '.cpp')
            exe = src.with_suffix('.exe')
            src.write_text(source, encoding='utf-8')
            cmd = [compiler(), '-std=c++20', '-O2', str(src), '-o', str(exe)]
            if release:
                cmd.append('-DNDEBUG')
            proc = subprocess.run(cmd, capture_output=True, timeout=180)
            assert proc.returncode == 0, proc.stderr.decode(errors='replace')
            return exe

        probe = build('probe', sources['full'] + BODY)
        bits, maxe = map(int, subprocess.check_output([str(probe), 'probe'], text=True).split())
        offset = 1 << (bits - 5)
        e = 3 * maxe // 4
        cases = [([], []), (['nan', 'inf'], [hx(0)] * 2),
                 ([hx(offset), hx(offset + 1)], [hx(1, -5), hx(31, -5)]),
                 ([hx(offset + 1), hx(offset)], [hx(31, -5), hx(1, -5)]),
                 ([hx(0), hx(1, e)], [hx(1, e), hx(1, -e)]),
                 ([hx(1, e), hx(0)], [hx(1, -e), hx(1, e)]),
                 ([hx(0), hx(2)], [hx(1, -2), hx(3, -2)]),
                 ([hx(-7)] * 8, [hx(1, i - 4) for i in range(8)])]
        rng = random.Random(132008)
        for kind in range(3):
            for _ in range(240):
                n = rng.randint(1, 24)
                span = min(1000, maxe // 4)
                scale = rng.choice([-span, 0, span])
                weights = [hx(rng.randint(0, 32), scale - 5) for _ in range(n)]
                if kind == 0:
                    xs = [hx(rng.randint(-1000, 1000), rng.randint(-20, 20)) for _ in range(n)]
                elif kind == 1:
                    base = offset * rng.choice([-1, 1])
                    xs = [hx(base + rng.randint(-100, 100)) for _ in range(n)]
                else:
                    xs = [hx(rng.randint(-32, 32), rng.randint(-100, 100)) for _ in range(n)]
                cases.append((xs, weights))
        expected = []
        for xs, weights in cases:
            w = s = ss = asum = F(0)
            prefixes = []
            for x, p in zip(xs, weights):
                p = exact(p)
                if p:
                    x = exact(x)
                    w += p; s += p * x; ss += p * x * x; asum += p * abs(x)
                row = (w, s / w, ss / w - (s / w)**2, asum / w) if w else None
                prefixes.append(row)
            final = prefixes[-1] if prefixes else None
            expected.extend(prefixes + [final] * 3)
            expected.append((2 * final[0], *final[1:]) if final else None)
        data = ''.join(f'{len(xs)}\n' + ' '.join(xs) + '\n' + ' '.join(ws) + '\n' for xs, ws in cases)
        invalid = [('0', '-1'), ('0', 'nan'), ('0', 'inf'), ('nan', '1'), ('inf', '1'), ('-inf', '1')]
        for name, source in sources.items():
            for macro in (False, True):
                variant = source if macro else source.replace('#define int long long', '')
                for release in (False, True):
                    label = f'{name}_{macro}_{release}'
                    exe = build(label, variant + BODY, release)
                    result = subprocess.run([str(exe)], input=data, text=True, capture_output=True, timeout=90, check=True)
                    rows = result.stdout.splitlines()
                    assert len(rows) == len(expected), (label, len(rows), len(expected))
                    for index, (line, want) in enumerate(zip(rows, expected)):
                        if want is None:
                            assert line == 'empty', (label, index, line)
                            continue
                        values = [exact(s) for s in line.split()]
                        weight, mean, var, absmean = want
                        assert abs(values[0] - weight) <= weight * F(1, 10**14), (label, index, 'weight')
                        # Absolute rounding bound for means permits cancellation, but scales with machine precision.
                        bound = max(abs(mean) / 10**13, absmean * F(1, 1 << (bits - 10)))
                        assert abs(values[1] - mean) <= bound, (label, index, 'mean', line)
                        assert values[2] >= 0
                        assert abs(values[2] - var) <= abs(var) / 10**12, (label, index, 'variance', line)
                    if not release:
                        for x, weight in invalid:
                            bad = f'1\n{x}\n{weight}\n'
                            assert subprocess.run([str(exe)], input=bad, text=True, capture_output=True, timeout=10).returncode != 0
                    sample = build(label + '_sample', variant + example)
                    subprocess.run([str(sample)], check=True, capture_output=True, timeout=20)
                    print(f'AC {label}: {len(cases)} datasets, {len(rows)} exact rational results', flush=True)
        for branch in ('full', 'print'):
            exe = build('fixed_' + branch, branch_source(ROOT / 'tests/test_ch13_online.cpp', branch))
            subprocess.run([str(exe)], check=True, capture_output=True, timeout=30)
            print(f'AC fixed {branch}: 100000 observations and moment API coexistence', flush=True)
    print('ONLINE_MOMENTS_OK temporary directory cleaned', flush=True)


if __name__ == '__main__':
    main()
