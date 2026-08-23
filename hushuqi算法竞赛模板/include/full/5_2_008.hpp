#pragma once

#include "5_2_001.hpp"

template<class Dist = long long>
pair<vector<optional<Dist>>, vector<int>> shortestTree(
    const vector<vector<WEdge>> &g, int s)
{
    // g 是非负权有向图；返回最短距离与一棵最短路径树的父节点。
    int n = g.size();
    assert(0 <= s && s < n); // 调试检查，可删。
    auto d = dijkstraSp<Dist>(g, s);
    vector<int> par(n, -1), vis(n);
    queue<int> q;
    vis[s] = 1;
    q.push(s);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (auto [v, w] : g[u])
        {
            Dist z = (Dist)w;
            if (!vis[v] && d[v].has_value() && *d[u] + z == *d[v])
            {
                vis[v] = 1;
                par[v] = u;
                q.push(v);
            }
        }
    }
    return {d, par};
}
