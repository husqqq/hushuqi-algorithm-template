#include "../include/full/5_8_011.hpp"

signed main()
{
    mt19937 rng(19);
    for (int tc = 0; tc < 5000; tc++)
    {
        int n = 3, m = 5;
        vector<array<int, 5>> e;
        for (int i = 0; i < m; i++)
        {
            int u = rng() % n, v = rng() % n;
            if (u == v)
            {
                v = (v + 1) % n;
            }
            int l = rng() % 2, r = l + rng() % 2, c = (int)(rng() % 7) - 3;
            e.push_back({u, v, l, r, c});
        }
        long long best = (1LL << 60);
        function<void(int, array<int, 3>, long long)> dfs = [&](int i, array<int, 3> bal, long long cost)
        {
            if (i == m)
            {
                if (bal[0] == 0 && bal[1] == 0 && bal[2] == 0)
                {
                    best = min(best, cost);
                }
                return;
            }
            auto [u, v, l, r, c] = e[i];
            for (int x = l; x <= r; x++)
            {
                auto z = bal;
                z[u] -= x;
                z[v] += x;
                dfs(i + 1, z, cost + (long long)x * c);
            }
        };
        dfs(0, {0, 0, 0}, 0);
        MinCostBFlow<> g(n);
        for (auto [u, v, l, r, c] : e)
        {
            g.add(u, v, l, r, c);
        }
        auto got = g.solve();
        assert((best == (1LL << 60)) == !got);
        if (got)
        {
            assert(*got == best);
        }
    }
}
