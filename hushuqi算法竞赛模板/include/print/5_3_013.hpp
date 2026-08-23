#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<vector<int>> threeEdgeCC(const vector<vector<int>> &g)
{
    // g 是允许重边和自环的无向邻接表；返回任意顺序的三边连通点集划分。
    int n = g.size(), timer = 0;
    vector<int> in(n), out(n), low(n, n), deg(n), path(n, n), nxt(n);
    vector<char> vis(n);
    iota(nxt.begin(), nxt.end(), 0);
    auto find = [&](auto &&self, int x) -> int
    {
        return nxt[x] == x ? x : nxt[x] = self(self, nxt[x]);
    };
    auto join = [&](int x, int y)
    {
        x = find(find, x);
        y = find(find, y);
        if (x != y)
        {
            nxt[y] = x;
            deg[x] += deg[y];
        }
    };
    auto dfs = [&](auto &&self, int v, int p) -> void
    {
        vis[v] = true;
        in[v] = timer++;
        for (int w : g[v])
        {
            if (w == v)
            {
                continue;
            }
            if (w == p)
            {
                p = n;
                continue;
            }
            if (vis[w])
            {
                if (in[w] < in[v])
                {
                    deg[v]++;
                    low[v] = min(low[v], in[w]);
                }
                else
                {
                    deg[v]--;
                    int u = path[v];
                    while (u != n && in[u] <= in[w] && in[w] < out[u])
                    {
                        join(v, u);
                        u = path[u];
                    }
                    path[v] = u;
                }
                continue;
            }
            self(self, w, v);
            if (path[w] == n && deg[w] <= 1)
            {
                deg[v] += deg[w];
                low[v] = min(low[v], low[w]);
            }
            else
            {
                if (deg[w] == 0)
                {
                    w = path[w];
                }
                if (low[w] < low[v])
                {
                    low[v] = low[w];
                    swap(w, path[v]);
                }
                while (w != n)
                {
                    join(v, w);
                    w = path[w];
                }
            }
        }
        out[v] = timer;
    };
    for (int i = 0; i < n; i++)
    {
        if (!vis[i])
        {
            dfs(dfs, i, n);
        }
    }
    vector<vector<int>> ans(n);
    for (int i = 0; i < n; i++)
    {
        ans[find(find, i)].push_back(i);
    }
    ans.erase(remove_if(ans.begin(), ans.end(), [](const vector<int> &a)
                        { return a.empty(); }), ans.end());
    return ans;
}
