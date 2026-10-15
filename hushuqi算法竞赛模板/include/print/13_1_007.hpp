#pragma once

#include "5_3_004.hpp"

// 知识点 13.1.007：树与一般无向图删边 Green Hackenbush 的 SG 值。
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

int graphSg(int n, const vector<array<int, 2>> &ed, int root = 0)
{
    // n 是点数，ed[i] 两维为无向边端点（每条边只列一次），root 是唯一地面根。
    // 返回 Green Hackenbush 的 SG；允许重边、自环，与根不连通的部分不参与。
    assert(n > 0 && 0 <= root && root < n);
    EdgeBCC bcc(n);
    for (auto [u, v] : ed)
    {
        bcc.addEdge(u, v);
    }
    bcc.build();
    auto bel = bcc.comps();
    int cnt = *max_element(bel.begin(), bel.end()) + 1;
    vector<vector<int>> tr(cnt);
    vector<int> val(cnt); // 每个边双内部融合后的自环数奇偶，即局部 SG。
    for (auto [u, v] : ed)
    {
        int a = bel[u], b = bel[v];
        if (a == b)
        {
            val[a] ^= 1;
        }
        else
        {
            tr[a].push_back(b);
            tr[b].push_back(a);
        }
    }
    auto dfs = [&](auto &&self, int u, int p) -> int
    {
        // self 是递归自身，u 是边双编号，p 是父边双；返回带局部自环的接地子树 SG。
        int ans = val[u];
        for (int v : tr[u])
        {
            if (v != p) ans ^= self(self, v, u) + 1;
        }
        return ans;
    };
    return dfs(dfs, bel[root], -1);
}
}
