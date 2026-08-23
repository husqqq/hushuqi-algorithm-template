#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<int> bfs(const vector<vector<int>> &g, int s)
{
    // g 是无权邻接表，s 是起点；返回最少边数，不可达点为 -1。
    vector<int> d(g.size(), -1);
    queue<int> q;
    d[s] = 0;
    q.push(s);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v : g[u])
        {
            if (d[v] == -1)
            {
                d[v] = d[u] + 1;
                q.push(v);
            }
        }
    }
    return d;
}

vector<int> dfs(const vector<vector<int>> &g, int s)
{
    // g 是邻接表，s 是起点；返回递归 DFS 首次访问顺序。
    vector<int> ord, vis(g.size());
    auto work = [&](auto &&self, int u) -> void
    {
        vis[u] = 1;
        ord.push_back(u);
        for (int v : g[u])
        {
            if (!vis[v])
            {
                self(self, v);
            }
        }
    };
    work(work, s);
    return ord;
}
