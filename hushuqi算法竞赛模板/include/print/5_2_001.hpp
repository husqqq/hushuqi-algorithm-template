#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

using WEdge = array<int, 2>; // 依次为有向边终点、非负边权。

template<class E, class Dist, class F>
void dijkstraInit(const vector<vector<E>> &g, vector<optional<Dist>> &d, F relax)
{
    // g[u] 每项的 [0] 为邻点、[1] 为非负边权，其余字段交给回调；d 为等长的初始代价表。
    // 原地求 min_s(d初值[s]+最短路(s,v))，无可达初值时仍为空；所有中间和须能用 Dist 表示。
    // 每次严格改善 d[v] 后调用 relax(u,边记录)，可记录前驱，但不得修改 g、d；无返回值。
    int n = g.size();
    assert((int)d.size() == n);
    priority_queue<pair<Dist, int>, vector<pair<Dist, int>>, greater<>> q;
    for (int u = 0; u < n; u++)
    {
        if (d[u]) q.push({*d[u], u});
    }
    while (!q.empty())
    {
        auto [du, u] = q.top();
        q.pop();
        if (!d[u] || du != *d[u]) continue;
        for (const auto &edge : g[u])
        {
            int v = edge[0];
            assert(edge[1] >= 0);
            Dist w = (Dist)edge[1];
            Dist nd = du + w;
            if (d[v] && *d[v] <= nd) continue;
            d[v] = nd;
            relax(u, edge);
            q.push({nd, v});
        }
    }
}

template <class Dist = long long>
vector<optional<Dist>> dijkstraSp(const vector<vector<WEdge>> &g, int s)
{
    // g 是非负权邻接表，s 是 [0,n) 内源点；返回最短距离，不可达点为空。
    int n = g.size();
    assert(0 <= s && s < n);
    vector<optional<Dist>> d(n);
    d[s] = 0;
    dijkstraInit(g, d, [](int, const WEdge &)
    {
        // 仅求距离，无需记录前驱。
    });
    return d;
}

template <class Dist = long long>
vector<optional<Dist>> dijkstraDn(
    const vector<vector<optional<int>>> &w, int s)
{
    // w 是非负权方阵，空值表示缺边，s 是 [0,n) 内源点；返回最短距离，不可达点为空。
    int n = w.size();
    vector<optional<Dist>> d(n);
    vector<bool> vis(n);
    d[s] = 0;
    for (int z = 0; z < n; z++)
    {
        int u = -1;
        for (int i = 0; i < n; i++)
        {
            if (!vis[i] && d[i].has_value() && (u < 0 || *d[i] < *d[u]))
            {
                u = i;
            }
        }
        if (u < 0)
        {
            break;
        }
        vis[u] = true;
        for (int v = 0; v < n; v++)
        {
            if (w[u][v].has_value())
            {
                assert(*w[u][v] >= 0); // 调试检查，可删：Dijkstra 要求非负边权。
                Dist z = (Dist)*w[u][v];
                Dist nd = *d[u] + z;
                if (!d[v].has_value() || nd < *d[v])
                {
                    d[v] = nd;
                }
            }
        }
    }
    return d;
}
