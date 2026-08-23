#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class Dist = long long>
optional<vector<optional<Dist>>> dagLongest(
    const vector<vector<array<int, 2>>> &g, int s = -1)
{
    // g[u] 的每项是终点和边权，s 是指定起点；图不是 DAG 时返回空。
    vector<int> deg(g.size()), ord;
    for (const auto &e : g)
    {
        for (auto [v, w] : e)
        {
            deg[v]++;
        }
    }
    queue<int> q;
    for (int u = 0; u < (int)g.size(); u++)
    {
        if (deg[u] == 0)
        {
            q.push(u);
        }
    }
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        ord.push_back(u);
        for (auto [v, w] : g[u])
        {
            if (--deg[v] == 0)
            {
                q.push(v);
            }
        }
    }
    if ((int)ord.size() != (int)g.size())
    {
        return nullopt;
    }
    vector<optional<Dist>> d(g.size());
    if (s == -1)
    {
        fill(d.begin(), d.end(), 0);
    }
    else
    {
        d[s] = 0;
    }
    for (int u : ord)
    {
        if (!d[u].has_value())
        {
            continue;
        }
        for (auto [v, w] : g[u])
        {
            Dist nd = *d[u] + (Dist)w;
            if (!d[v].has_value() || nd > *d[v])
            {
                d[v] = nd;
            }
        }
    }
    return d;
}
