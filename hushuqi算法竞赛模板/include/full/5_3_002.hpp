#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<int> kosarajuSCC(const vector<vector<int>> &g)
{
    // g 是有向邻接表；返回按缩点 DAG 拓扑序递增的 SCC 编号。
    int n = g.size(), cc = 0;
    vector<vector<int>> rg(n);
    for (int u = 0; u < n; u++)
    {
        for (int v : g[u])
        {
            rg[v].push_back(u);
        }
    }
    vector<int> ord, bel(n, -1);
    vector<bool> vis(n);
    auto dfs1 = [&](auto &&self, int u) -> void
    {
        vis[u] = true;
        for (int v : g[u])
        {
            if (!vis[v])
            {
                self(self, v);
            }
        }
        ord.push_back(u);
    };
    auto dfs2 = [&](auto &&self, int u) -> void
    {
        bel[u] = cc;
        for (int v : rg[u])
        {
            if (bel[v] == -1)
            {
                self(self, v);
            }
        }
    };
    for (int u = 0; u < n; u++)
    {
        if (!vis[u])
        {
            dfs1(dfs1, u);
        }
    }
    reverse(ord.begin(), ord.end());
    for (int u : ord)
    {
        if (bel[u] == -1)
        {
            dfs2(dfs2, u);
            cc++;
        }
    }
    return bel;
}
