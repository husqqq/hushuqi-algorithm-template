// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 16a 最短路 1


#include <bits/stdc++.h>
using namespace std;
#define int long long

using WEdge = array<int, 2>;

template <class Dist = long long>
vector<optional<Dist>> dijkstraSp(const vector<vector<WEdge>> &g, int s)
{
    // g 是稀疏图邻接表，s 是源点；返回最短距离，不可达点为空。
    int n = g.size();
    vector<optional<Dist>> d(n);
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
            assert(w >= 0); // 调试检查，可删：Dijkstra 要求非负边权。
            Dist z = (Dist)w;
            Dist nd = du + z;
            if (!d[v].has_value() || nd < *d[v])
            {
                d[v] = nd;
                q.push({nd, v});
            }
        }
    }
    return d;
}

template <class Dist = long long>
vector<optional<Dist>> dijkstraDn(
    const vector<vector<optional<int>>> &w, int s)
{
    // w 是邻接矩阵，空值表示缺边；返回最短距离，不可达点为空。
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
