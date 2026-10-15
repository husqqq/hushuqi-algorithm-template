#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<pair<int, int>> bipolarOrd(const vector<vector<int>> &g, int s, int t)
{
    // g 是简单无向图；返回以 s 为唯一源、t 为唯一汇的边方向，无解返回空。
    int n = g.size();
    vector<unsigned long long> adj(n);
    for (int u = 0; u < n; u++)
    {
        for (int v : g[u])
        {
            adj[u] |= 1ULL << v;
        }
    }
    for (int u = 0; u < n; u++)
    {
        for (int v = 0; v < n; v++)
        {
        }
    }
    if (!(adj[s] >> t & 1))
    {
        return {};
    }
    vector<int> p{s};
    vector<bool> use(n);
    use[s] = true;
    use[t] = true;
    auto dfs = [&](auto &&self) -> bool
    {
        if ((int)p.size() == n - 1)
        {
            p.push_back(t);
            unsigned long long left = 1ULL << s;
            for (int i = 1; i + 1 < n; i++)
            {
                int u = p[i];
                unsigned long long right = 0;
                for (int j = i + 1; j < n; j++)
                {
                    right |= 1ULL << p[j];
                }
                if (!(adj[u] & left) || !(adj[u] & right))
                {
                    p.pop_back();
                    return false;
                }
                left |= 1ULL << u;
            }
            return true;
        }
        for (int v = 0; v < n; v++)
        {
            if (!use[v])
            {
                use[v] = true;
                p.push_back(v);
                if (self(self))
                {
                    return true;
                }
                p.pop_back();
                use[v] = false;
            }
        }
        return false;
    };
    if (!dfs(dfs))
    {
        return {};
    }
    vector<int> at(n);
    for (int i = 0; i < n; i++)
    {
        at[p[i]] = i;
    }
    vector<pair<int, int>> ans;
    for (int u = 0; u < n; u++)
    {
        for (int v : g[u])
        {
            if (u < v)
            {
                ans.push_back(at[u] < at[v] ? pair{u, v} : pair{v, u});
            }
        }
    }
    return ans;
}
