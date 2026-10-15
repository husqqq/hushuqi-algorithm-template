#pragma once

#include "13_1_001.hpp"

// 知识点 13.1.007：树上删边 Green Hackenbush 的 SG 值。
namespace edge_del
{
int treeSg(const vector<vector<int>> &g, int root = 0)
{
    // g 是无向树，root 是地面根；返回整个局面的 SG 值。
    int n = g.size();
    assert(n > 0 && 0 <= root && root < n); // 调试检查，可删。
    int deg = 0;
    for (int u = 0; u < n; u++)
    {
        for (int v : g[u])
        {
            assert(0 <= v && v < n); // 调试检查，可删。
            deg++;
        }
    }
    assert(deg == 2 * (n - 1)); // 调试检查，可删。
    vector<int> par(n, -2);
    int seen = 0;
    auto dfs = [&](auto &&self, int u, int p) -> int
    {
        // self 是递归自身，u 是当前点，p 是父点；返回接地子树的 SG 值。
        par[u] = p;
        seen++;
        int ans = 0;
        for (int v : g[u])
        {
            if (v == p)
            {
                continue;
            }
            assert(par[v] == -2); // 调试检查，可删。
            ans ^= self(self, v, u) + 1;
        }
        return ans;
    };
    int ans = dfs(dfs, root, -1);
    assert(seen == n); // 调试检查，可删。
    return ans;
}
}
