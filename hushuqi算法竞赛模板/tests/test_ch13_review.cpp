#include "../include/full/13_1_002.hpp"
#include "../include/full/13_1_004.hpp"
#include "../include/full/13_1_006.hpp"
#include "../include/full/13_1_010.hpp"
#include "../include/full/13_2_004.hpp"

signed main()
{
    // 无参数；独立枚举小局面、策略与固定种子随机图，成功返回 0。
    assert(nimKWin({1ULL << 63}, numeric_limits<int>::max()));
    assert(!nimKWin({1ULL << 63, 1ULL << 63, 1ULL << 63}, 2));
    assert(!nimKWin({}, 1) && antiNimWin({0, 0}));
    for (int k = 1; k <= 5; k++)
    {
        vector<array<bool, 2>> dp(256);
        dp[0][1] = true;
        for (int mask = 1; mask < 256; mask++)
        {
            vector<unsigned long long> a(4);
            for (int i = 0; i < 4; i++)
            {
                a[i] = mask >> (2 * i) & 3;
            }
            for (int sub = 0; sub < mask; sub++)
            {
                bool legal = true;
                int changed = 0;
                for (int i = 0; i < 4; i++)
                {
                    int x = sub >> (2 * i) & 3;
                    if (x > (int)a[i]) legal = false;
                    changed += x != (int)a[i];
                }
                if (!legal) continue;
                if (changed <= k) dp[mask][0] |= !dp[sub][0];
                if (changed == 1) dp[mask][1] |= !dp[sub][1];
            }
            assert(nimKWin(a, k) == dp[mask][0]);
            assert(antiNimWin(a) == dp[mask][1]);
            if (k == 1) assert(nimWin(a) == dp[mask][0]);
        }
    }
    // Fibonacci 博弈按完整的剩余量与当前上限递推。
    vector<vector<bool>> fib(151, vector<bool>(301));
    for (int n = 1; n <= 150; n++)
    {
        for (int lim = 0; lim <= 300; lim++)
        {
            for (int take = 1; take <= min(n, lim); take++)
            {
                if (!fib[n - take][2 * take]) fib[n][lim] = true;
            }
            assert(fib[n][lim] == (lim >= fib_game::zeckMin(n)));
        }
    }
    mt19937_64 rng(20261016);
    assert(sg({}).empty() && gameGraph({}).empty());
    for (int tc = 0; tc < 1000; tc++)
    {
        int n = rng() % 30 + 1;
        vector<int> p(n), want(n, -1);
        iota(p.begin(), p.end(), 0);
        shuffle(p.begin(), p.end(), rng);
        vector<vector<int>> g(n);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < i; j++)
            {
                if (rng() % 3 == 0) g[p[i]].push_back(p[j]);
            }
            if (!g[p[i]].empty()) g[p[i]].push_back(g[p[i]].back());
        }
        auto calc = [&](auto &&self, int u) -> int
        {
            // self 是递归自身，u 是顶点；用集合返回独立 mex 结果。
            if (want[u] >= 0) return want[u];
            set<int> seen;
            for (int v : g[u])
            {
                seen.insert(self(self, v));
            }
            int x = 0;
            while (seen.contains(x))
            {
                x++;
            }
            return want[u] = x;
        };
        for (int u = 0; u < n; u++)
        {
            calc(calc, u);
        }
        assert(sg(g) == want);
    }
    // 独立枚举双方全部位置策略，并模拟终局或重复状态；不使用逆图传播。
    for (int tc = 0; tc < 500; tc++)
    {
        int n = rng() % 4 + 1;
        vector<vector<int>> g(n);
        vector<vector<int>> strat(1, vector<int>(n, -1));
        for (int u = 0; u < n; u++)
        {
            int d = rng() % 3;
            for (int i = 0; i < d; i++)
            {
                g[u].push_back(rng() % n);
            }
            if (d == 0) continue;
            auto prev = strat;
            strat.clear();
            for (auto a : prev)
            {
                for (int v : g[u])
                {
                    a[u] = v;
                    strat.push_back(a);
                }
            }
        }
        auto got = gameGraph(g);
        for (int start = 0; start < n; start++)
        {
            int best = -1;
            for (const auto &a : strat)
            {
                int worst = 1;
                for (const auto &b : strat)
                {
                    vector<bool> seen(2 * n);
                    int u = start, turn = 0, result = 0;
                    while (!seen[2 * u + turn])
                    {
                        if (g[u].empty())
                        {
                            result = turn ? 1 : -1;
                            break;
                        }
                        seen[2 * u + turn] = true;
                        u = turn ? b[u] : a[u];
                        turn ^= 1;
                    }
                    worst = min(worst, result);
                }
                best = max(best, worst);
            }
            assert(got[start] == best);
        }
    }
    const auto mx = numeric_limits<unsigned long long>::max();
    // 逆移位构造 next() 恰好等于指定输出的非零种子，覆盖拒绝区间边缘。
    auto seedFor = [](unsigned long long x)
    {
        // x 是期望的非零输出；返回逆 xorshift 得到的前一状态。
        unsigned long long y = x;
        for (int i = 17; i < 64; i += 17)
        {
            y ^= x << i;
        }
        x = y;
        for (int i = 7; i < 64; i += 7)
        {
            y ^= x >> i;
        }
        x = y;
        for (int i = 13; i < 64; i += 13)
        {
            y ^= x << i;
        }
        return y;
    };
    for (auto bound : vector<unsigned long long>{1, 2, 3, 100, 1ULL << 63, mx - 1, mx})
    {
        for (auto raw : vector<unsigned long long>{1, 2, 3, bound, mx - 1, mx})
        {
            auto seed = seedFor(raw);
            RNG check(seed);
            assert(check.next() == raw);
            RNG a(seed), b(seed);
            unsigned long long out;
            do
            {
                out = b.next();
            } while ((__int128)(out - 1) >= (__int128)(mx / bound) * bound);
            assert(a(bound) == (out - 1) % bound && a.s == b.s);
        }
    }
    // 缩小到 8 位非零均匀源，穷尽检查每个上界的接受原像计数。
    for (int bound = 1; bound <= 255; bound++)
    {
        vector<int> count(bound);
        for (int raw = 1; raw <= 255; raw++)
        {
            int x = raw - 1;
            if (x < 255 - 255 % bound) count[x % bound]++;
        }
        assert(*min_element(count.begin(), count.end()) == 255 / bound);
        assert(*max_element(count.begin(), count.end()) == 255 / bound);
    }
    cout << "OK\n";
}
