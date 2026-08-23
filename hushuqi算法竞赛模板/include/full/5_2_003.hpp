#pragma once

#include "5_2_002.hpp"

template <class Dist = long long>
optional<vector<optional<Dist>>> spfa(
    int n, const vector<DEdge> &edges, int s)
{
    // n 是点数，edges 是边表，s 是源点；有可达负环时返回空。
    vector<vector<pair<int, Dist>>> e(n);
    for (auto [u, v, w] : edges)
    {
        e[u].push_back({v, w});
    }
    vector<optional<Dist>> d(n);
    vector<int> in(n), len(n);
    queue<int> q;
    d[s] = 0;
    in[s] = 1;
    q.push(s);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        in[u] = 0;
        for (auto [v, w] : e[u])
        {
            Dist nd = *d[u] + w;
            if (d[v].has_value() && *d[v] <= nd)
            {
                continue;
            }
            d[v] = nd;
            len[v] = len[u] + 1;
            if (len[v] >= n)
            {
                return nullopt;
            }
            if (!in[v])
            {
                in[v] = 1;
                q.push(v);
            }
        }
    }
    return d;
}
