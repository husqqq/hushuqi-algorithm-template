#pragma once

#include "13_1_001.hpp"

namespace edge_del
{
int treeSg(const vector<vector<int>> &g, int root = 0)
{
    int n = g.size();
    assert(n > 0 && 0 <= root && root < n);
    int deg = 0;
    for (int u = 0; u < n; u++)
    {
        for (int v : g[u])
        {
            assert(0 <= v && v < n);
            deg++;
        }
    }
    assert(deg == 2 * (n - 1));
    vector<int> par(n, -2);
    int seen = 0;
    auto dfs = [&](auto &&self, int u, int p) -> int
    {
        par[u] = p;
        seen++;
        int ans = 0;
        for (int v : g[u])
        {
            if (v == p)
            {
                continue;
            }
            assert(par[v] == -2);
            ans ^= self(self, v, u) + 1;
        }
        return ans;
    };
    int ans = dfs(dfs, root, -1);
    assert(seen == n);
    return ans;
}
}
