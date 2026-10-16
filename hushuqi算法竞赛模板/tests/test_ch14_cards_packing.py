"""Check poker scores, packing, grid paths and factor extensions through real copy paths."""
from __future__ import annotations

import collections
import itertools
import math
import random
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from check_test_variants import compiler, branch_source
from test_ch14_widths import expand


def poker(cards: tuple[int, ...] | list[int]) -> tuple[int, ...]:
    ranks = sorted((c // 4 + 2 for c in cards), reverse=True)
    counts = collections.Counter(ranks)
    groups = sorted(((v, k) for k, v in counts.items()), reverse=True)
    flush = len({c % 4 for c in cards}) == 1
    high = 5 if set(ranks) == {14, 2, 3, 4, 5} else (max(ranks) if len(counts) == 5 and max(ranks)-min(ranks) == 4 else 0)
    if high and flush: result = [8, high]
    elif groups[0][0] == 4: result = [7, groups[0][1], groups[1][1]]
    elif [g[0] for g in groups] == [3, 2]: result = [6, groups[0][1], groups[1][1]]
    elif flush: result = [5] + ranks
    elif high: result = [4, high]
    elif groups[0][0] == 3: result = [3, groups[0][1]] + sorted((x for x in ranks if counts[x] == 1), reverse=True)
    elif [g[0] for g in groups[:2]] == [2, 2]: result = [2] + sorted((x for x in counts if counts[x] == 2), reverse=True) + [x for x in counts if counts[x] == 1]
    elif groups[0][0] == 2: result = [1, groups[0][1]] + sorted((x for x in ranks if counts[x] == 1), reverse=True)
    else: result = [0] + ranks
    return tuple(result + [0] * (6-len(result)))


def first_fit(a: list[int], cap: int) -> tuple[int, list[int]]:
    rem, boxes = [], []
    for x in a:
        p = next((i for i, r in enumerate(rem) if r >= x), len(rem))
        if p == len(rem): rem.append(cap)
        rem[p] -= x
        boxes.append(p)
    return len(rem), boxes


def exact(a: list[int], cap: int) -> int:
    if not a: return 0
    a = sorted(a, reverse=True)
    best, loads = len(a), []
    def dfs(i: int) -> None:
        nonlocal best
        if len(loads) >= best: return
        if i == len(a):
            best = len(loads)
            return
        seen = set()
        for j in range(len(loads)):
            x = loads[j]
            if x not in seen and x + a[i] <= cap:
                seen.add(x)
                loads[j] += a[i]
                dfs(i+1)
                loads[j] -= a[i]
        loads.append(a[i])
        dfs(i+1)
        loads.pop()
    dfs(0)
    return best


def colorings(n: int, m: int):
    a = [[-1] * n for _ in range(n)]
    used = [set() for _ in range(n)]
    edges = list(itertools.combinations(range(n), 2))
    def dfs(i: int):
        if i == len(edges):
            yield tuple(tuple(row) for row in a)
            return
        u, v = edges[i]
        for c in range(m):
            if c in used[u] or c in used[v]: continue
            used[u].add(c); used[v].add(c)
            a[u][v] = a[v][u] = c
            yield from dfs(i+1)
            used[u].remove(c); used[v].remove(c)
    yield from dfs(0)


def factor_cases() -> list[tuple[int, tuple[tuple[int, ...], ...], int]]:
    cases = []
    for m in (1, 3, 5):
        full = list(colorings(m+1, m))
        for n in range(min(4, m+1)+1):
            possible = {tuple(tuple(row[:n]) for row in a[:n]) for a in full}
            for a in colorings(n, m):
                cases.append((m, a, int(a in possible)))
    rng = random.Random(14015)
    for m in (7, 9, 15):
        n = m+1
        a = [[-1]*n for _ in range(n)]
        for c in range(m):
            a[m][c] = a[c][m] = c
            for d in range(1, (m+1)//2):
                u, v = (c+d)%m, (c-d)%m
                a[u][v] = a[v][u] = c
        for _ in range(30):
            vs, cs = list(range(n)), list(range(m))
            rng.shuffle(vs); rng.shuffle(cs)
            k = rng.randrange(n+1)
            pre = tuple(tuple(-1 if u == v else cs[a[u][v]] for v in vs[:k]) for u in vs[:k])
            cases.append((m, pre, 1))
    return cases


BODY = r"""
signed main()
{
    static_assert(sizeof(int) == WIDTH);
    int t;
    cin >> t;
    while (t--)
    {
        int n; cin >> n;
        vector<int> r(n), s(n);
        for (int i = 0; i < n; i++) { cin >> r[i] >> s[i]; }
        auto z = pokerBest(r, s);
        if (n == 5) { assert(z == pokerScore(r, s)); }
        for (int x : z) { cout << x << ' '; }
        cout << '\n';
    }
    cin >> t;
    while (t--)
    {
        int n, cap; cin >> n >> cap;
        vector<int> a(n);
        for (int &x : a) { cin >> x; }
        auto [k, box] = bin_packing::firstFit(a, cap);
        cout << k << ' ';
        for (int x : box) { cout << x << ' '; }
        cout << bin_packing::firstFitDec(a, cap) << ' ' << bin_packing::exactBins(a, cap) << '\n';
    }
    int cap = numeric_limits<int>::max();
    assert(bin_packing::exactBins({cap/2+1, cap/2+1}, cap) == 2);
    assert(bin_packing::firstFitDec({cap/2+1, cap/2+1}, cap) == 2);
    cin >> t;
    Comb<MInt<17>> comb;
    while (t--)
    {
        int h, w, k; cin >> h >> w >> k;
        vector<array<int, 2>> ban(k);
        for (auto &p : ban) { cin >> p[0] >> p[1]; }
        cout << grid_count::gridPaths<MInt<17>>(h, w, ban, comb).val() << '\n';
    }
    cin >> t;
    while (t--)
    {
        int m, n; cin >> m >> n;
        vector<vector<int>> a(n, vector<int>(n));
        for (auto &row : a) { for (int &x : row) { cin >> x; } }
        auto z = factor_ext::extendFactor(a, m);
        cout << (bool)z << '\n';
        if (!z) continue;
        assert(z->size() == m+1);
        for (int i = 0; i <= m; i++)
        {
            assert((*z)[i][i] == -1);
            vector<bool> seen(m);
            for (int j = 0; j <= m; j++)
            {
                if (i < n && j < n) { assert((*z)[i][j] == a[i][j]); }
                if (i == j) continue;
                int c = (*z)[i][j];
                assert(0 <= c && c < m && !seen[c] && (*z)[j][i] == c);
                seen[c] = true;
            }
        }
    }
    assert(!factor_ext::extendFactor({}, numeric_limits<int>::max()));
    assert(!factor_ext::extendFactor({}, 0));
    assert(!factor_ext::extendFactor({}, 4));
    assert(!factor_ext::extendFactor({{0}}, 3));
    assert(!factor_ext::extendFactor({{-1, 0}, {1, -1}}, 3));
    assert(!factor_ext::extendFactor({{-1, 0, 0}, {0, -1, 1}, {0, 1, -1}}, 3));
    if (EXHAUSTIVE)
    {
        array<int, 9> hist{};
        set<array<int, 6>> scores;
        vector<int> r(5), s(5);
        for (int a = 0; a < 52; a++)
        {
            for (int b = a+1; b < 52; b++)
            {
                for (int c = b+1; c < 52; c++)
                {
                    for (int d = c+1; d < 52; d++)
                    {
                        for (int e = d+1; e < 52; e++)
                        {
                            array<int, 5> ids{a,b,c,d,e};
                            for (int i = 0; i < 5; i++) { r[i]=ids[i]/4+2; s[i]=ids[i]%4; }
                            auto z=pokerScore(r,s);
                            hist[z[0]]++;
                            scores.insert(z);
                        }
                    }
                }
            }
        }
        assert((hist == array<int,9>{1302540,1098240,123552,54912,10200,5108,3744,624,40}));
        assert(scores.size() == 7462);
    }
}
"""


def main() -> None:
    rng = random.Random(14004)
    hands = [rng.sample(range(52), 5) for _ in range(5000)]
    hands += [rng.sample(range(52), rng.randrange(6, 11)) for _ in range(600)]
    # Every rank multiset with one legal suit assignment, and explicit same-suit straights.
    for ranks in itertools.combinations_with_replacement(range(13), 5):
        if max(collections.Counter(ranks).values()) > 4: continue
        counts = collections.Counter()
        hand = []
        for r in ranks:
            hand.append(r*4+counts[r]); counts[r] += 1
        rng.shuffle(hand)
        hands.append(hand)
    for suit in range(4):
        hands.append([r*4+suit for r in (12,0,1,2,3)])
        hands.append([r*4+suit for r in (8,9,10,11,12)])
    data, expected = [str(len(hands))+"\n"], []
    for hand in hands:
        z = max(poker(sub) for sub in itertools.combinations(hand,5))
        data.append(str(len(hand))+" "+" ".join(f"{c//4+2} {c%4}" for c in hand)+"\n")
        expected.append(" ".join(map(str,z)))
    bins = [([],0),([0,0,0],0),([5,3,4,4,2],8),([6,4,4,2],8)]
    for _ in range(300):
        cap = rng.randrange(1, 30)
        bins.append(([rng.randrange(cap+1) for _ in range(rng.randrange(11))], cap))
    data.append(str(len(bins))+"\n")
    for a, cap in bins:
        data.append(f"{len(a)} {cap} "+" ".join(map(str,a))+"\n")
        k, box = first_fit(a,cap)
        expected.append(" ".join(map(str,[k,*box,first_fit(sorted(a,reverse=True),cap)[0],exact(a,cap)])))
    grids = []
    for h in range(1, 10):
        for w in range(1, 10):
            if h+w-2 >= 17: continue
            for _ in range(8):
                ban = [(i,j) for i in range(h) for j in range(w) if rng.randrange(5)==0]
                if ban: ban += [ban[0],ban[-1]]
                grids.append((h,w,ban))
    data.append(str(len(grids))+"\n")
    for h,w,ban in grids:
        data.append(f"{h} {w} {len(ban)} "+" ".join(f"{i} {j}" for i,j in ban)+"\n")
        dp = [[0]*w for _ in range(h)]
        blocked = set(ban)
        for i in range(h):
            for j in range(w):
                if (i,j) in blocked: continue
                dp[i][j] = (int(i==0 and j==0) + (dp[i-1][j] if i else 0) + (dp[i][j-1] if j else 0)) % 17
        expected.append(str(dp[-1][-1]))
    factors = factor_cases()
    data.append(str(len(factors))+"\n")
    for m,a,answer in factors:
        data.append(f"{m} {len(a)} "+" ".join(str(x) for row in a for x in row)+"\n")
        expected.append(str(answer))
    topics = ("14_1_004", "14_1_008", "14_1_009", "14_1_015", "8_1_001")
    sources = {}
    for branch in ("full", "print"):
        seen: set[Path] = set()
        sources[branch] = "\n".join(expand(ROOT/'include'/branch/(t+'.hpp'),seen) for t in topics)
    printed = []
    for chapter, topic in (("01-基础与工程.md","1.1.008"), ("03-数据结构.md","3.3.001"),
                           ("05-图论.md","5.6.001"), ("08-组合数学.md","8.1.001"),
                           *(("14-其他实用模板.md","14.1."+t) for t in ("004","008","009","015"))):
        text = (ROOT/chapter).read_text(encoding='utf-8')
        printed.append(re.search(rf'^### {re.escape(topic)} .*?^```cpp\n(.*?)^```',text,re.M|re.S)[1])
    sources['markdown'] = '\n'.join(printed)
    with tempfile.TemporaryDirectory(prefix='ch14-cards-packing-') as temp:
        for branch, source in sources.items():
            source = re.sub(r'^#define int long long\s*$', '', source, flags=re.M)
            for enabled in (False, True):
                stem = f'{branch}_{int(enabled)}'
                path = Path(temp)/(stem+'.cpp')
                exe = path.with_suffix('.exe')
                prefix = '#include <bits/stdc++.h>\nusing namespace std;\n'+('#define int long long\n' if enabled else '')
                prefix += f'#define WIDTH {8 if enabled else 4}\n#define EXHAUSTIVE {int(branch=="full" and enabled)}\n'
                path.write_text(prefix+source+BODY,encoding='utf-8')
                r = subprocess.run([compiler(),'-std=c++20','-O2',str(path),'-o',str(exe)],capture_output=True,timeout=180)
                assert r.returncode==0,(stem,r.stderr.decode(errors='replace')[:6000])
                r = subprocess.run([str(exe)],input=''.join(data),capture_output=True,text=True,timeout=180)
                assert r.returncode==0,(stem,r.stderr)
                actual = [' '.join(line.split()) for line in r.stdout.splitlines()]
                if actual != expected:
                    i=next((i for i,(a,b) in enumerate(zip(actual,expected)) if a!=b),min(len(actual),len(expected)))
                    raise AssertionError((stem,i,actual[i:i+1],expected[i:i+1]))
                print(f'{stem}: {len(hands)} poker, {len(bins)} packing, {len(grids)} grid, {len(factors)} factor cases OK',flush=True)
                if branch=='full' and enabled:
                    print('All 2598960 five-card hands: exact category counts and 7462 scores OK',flush=True)
        for branch in ('full','print'):
            for name in ('test_ch14_core.cpp','test_ch14_extra.cpp','test_supplements.cpp','test_extra_topics.cpp'):
                path=Path(temp)/(branch+'_'+name)
                path.write_text(branch_source(ROOT/'tests'/name,branch),encoding='utf-8')
                exe=path.with_suffix('.exe')
                subprocess.run([compiler(),'-std=c++20','-O2',str(path),'-o',str(exe)],check=True,timeout=180,capture_output=True)
                subprocess.run([str(exe)],check=True,timeout=180,capture_output=True)
                print(f'{branch}: {name} OK',flush=True)


if __name__ == '__main__':
    main()
