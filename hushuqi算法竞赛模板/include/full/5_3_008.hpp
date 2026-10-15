#pragma once

#include "5_8_001.hpp"
#include "5_8_007.hpp"

int edgeConn(int n, const vector<tuple<int, int, long long>> &e)
{
    // e 是非负权无向边；返回全局边连通度。
    vector a(n, vector<int>(n));
    for (auto [u, v, w] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && w >= 0); // 调试检查，可删。
        if (u != v)
        {
            a[u][v] += w;
            a[v][u] = a[u][v];
        }
    }
    return stoerWagner(move(a)).w;
}

template <class F = Flow<long long>>
int vertexConn(const vector<vector<int>> &g)
{
    // g 是简单无向邻接表；返回全局点连通度。
    int n = g.size();
    if (n <= 1)
    {
        return 0;
    }
    vector a(n, vector<bool>(n));
    int m = 0;
    for (int u = 0; u < n; u++)
    {
        for (int v : g[u])
        {
            assert(0 <= v && v < n); // 调试检查，可删。
            if (u != v && !a[u][v])
            {
                a[u][v] = true;
                a[v][u] = true;
                m++;
            }
        }
    }
    if (m == n * (n - 1) / 2)
    {
        return n - 1;
    }
    int ans = n - 1;
    for (int s = 0; s < n; s++)
    {
        for (int t = s + 1; t < n; t++)
        {
            if (a[s][t])
            {
                continue;
            }
            F f(2 * n);
            for (int u = 0; u < n; u++)
            {
                f.add(2 * u, 2 * u + 1, u == s || u == t ? n : 1);
            }
            for (int u = 0; u < n; u++)
            {
                for (int v = u + 1; v < n; v++)
                {
                    if (a[u][v])
                    {
                        f.add(2 * u + 1, 2 * v, n);
                        f.add(2 * v + 1, 2 * u, n);
                    }
                }
            }
            ans = min(ans, f.flow(2 * s + 1, 2 * t));
        }
    }
    return ans;
}
