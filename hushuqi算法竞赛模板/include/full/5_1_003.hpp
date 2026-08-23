#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

optional<vector<int>> topo(const vector<vector<int>> &g)
{
    // g 是有向邻接表；返回一种拓扑序，含环时返回空。
    vector<int> deg(g.size()), ord;
    for (const auto &e : g)
    {
        for (int v : e)
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
        for (int v : g[u])
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
    return ord;
}
