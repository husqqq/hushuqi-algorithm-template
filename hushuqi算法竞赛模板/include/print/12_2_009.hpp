#pragma once

#include "12_1_001.hpp"

template <class Acc = long long>
Acc treeMWIS(const vector<vector<int>> &g, const vector<Acc> &w, int root = 0)
{
    int n = g.size();
    assert((int)w.size() == n);
    if (n == 0)
    {
        return 0;
    }
    assert(0 <= root && root < n);
    int deg = 0;
    for (int u = 0; u < n; u++)
    {
        deg += g[u].size();
        for (int v : g[u])
        {
            assert(0 <= v && v < n);
        }
    }
    assert(deg == 2 * (n - 1));
    vector<int> par(n, -2);
    int seen = 0;
    auto dfs = [&](auto &&self, int u, int p) -> array<Acc, 2>
    {
        par[u] = p;
        seen++;
        array<Acc, 2> f{0, w[u]};
        for (int v : g[u])
        {
            if (v == p)
            {
                continue;
            }
            assert(par[v] == -2);
            auto h = self(self, v, u);
            f[0] += max(h[0], h[1]);
            f[1] += h[0];
        }
        return f;
    };
    auto f = dfs(dfs, root, -1);
    assert(seen == n);
    return max(f[0], f[1]);
}
