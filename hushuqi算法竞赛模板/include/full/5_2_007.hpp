#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template<class Dist = long long>
struct DualPath
{
    // dist 是最小穿越代价，path 是起始面到终止面的面序列。
    Dist dist;
    vector<int> path;
};

template<class Dist = long long>
optional<DualPath<Dist>> dualShortest(
    int faces, const vector<array<int, 3>> &dualEdges, int s, int t)
{
    // dualEdges 是相邻面与非负代价；不可达时返回空。
    assert(faces > 0 && 0 <= s && s < faces && 0 <= t && t < faces); // 调试检查，可删。
    vector<vector<array<int, 2>>> g(faces);
    for (auto [u, v, w] : dualEdges)
    {
        assert(0 <= u && u < faces && 0 <= v && v < faces && w >= 0); // 调试检查，可删。
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    vector<optional<Dist>> d(faces);
    vector<int> pre(faces, -1);
    priority_queue<pair<Dist, int>, vector<pair<Dist, int>>, greater<>> q;
    d[s] = 0;
    q.push({0, s});
    while (!q.empty())
    {
        auto [du, u] = q.top();
        q.pop();
        if (!d[u].has_value() || du != *d[u])
        {
            continue;
        }
        for (auto [v, w] : g[u])
        {
            Dist nd = du + (Dist)w;
            if (!d[v].has_value() || nd < *d[v])
            {
                d[v] = nd;
                pre[v] = u;
                q.push({nd, v});
            }
        }
    }
    if (!d[t].has_value())
    {
        return nullopt;
    }
    vector<int> path;
    for (int u = t; u != -1; u = pre[u])
    {
        path.push_back(u);
    }
    reverse(path.begin(), path.end());
    return DualPath<Dist>{*d[t], path};
}
