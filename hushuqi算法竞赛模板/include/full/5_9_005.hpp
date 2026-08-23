#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct MeanEdge
{
    // 端点与边权。
    int u, v;
    long double w;
};

optional<long double> minMeanCycle(int n, const vector<MeanEdge> &e)
{
    // n 是点数，e 是有向边；返回最小平均环权，无环返回空。
    vector f(n + 1, vector<optional<long double>>(n));
    for (int v = 0; v < n; v++)
    {
        f[0][v] = 0;
    }
    for (int k = 1; k <= n; k++)
    {
        for (auto [u, v, w] : e)
        {
            assert(0 <= u && u < n && 0 <= v && v < n && isfinite(w)); // 调试检查，可删。
            if (f[k - 1][u].has_value())
            {
                long double z = *f[k - 1][u] + w;
                if (!f[k][v].has_value() || z < *f[k][v])
                {
                    f[k][v] = z;
                }
            }
        }
    }
    optional<long double> ans;
    for (int v = 0; v < n; v++)
    {
        if (!f[n][v].has_value())
        {
            continue;
        }
        optional<long double> cur;
        for (int k = 0; k < n; k++)
        {
            if (f[k][v].has_value())
            {
                long double z = (*f[n][v] - *f[k][v]) / (n - k);
                if (!cur.has_value() || z > *cur)
                {
                    cur = z;
                }
            }
        }
        if (cur.has_value() && (!ans.has_value() || *cur < *ans))
        {
            ans = *cur;
        }
    }
    return ans;
}

struct RatioEdge
{
    // 端点、收益与严格为正的代价。
    int u, v;
    long double a, b;
};

// b 必须严格为正；求环上 sum(a)/sum(b) 的最小值。
optional<long double> minRatCycle(int n, const vector<RatioEdge> &e, int it = 100)
{
    // n 是点数，e 是有向边，it 是二分轮数；返回最小环比率，无环返回空。
    if (e.empty())
    {
        return nullopt;
    }
    assert(it >= 0); // 调试检查，可删。
    vector<vector<int>> g(n);
    vector<int> deg(n);
    long double lo = 0, hi = 0;
    bool first = true;
    for (auto [u, v, a, b] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n &&
               isfinite(a) && isfinite(b) && b > 0); // 调试检查，可删。
        g[u].push_back(v);
        deg[v]++;
        long double z = a / b;
        if (first)
        {
            lo = hi = z;
            first = false;
        }
        else
        {
            lo = min(lo, z);
            hi = max(hi, z);
        }
    }
    queue<int> q;
    for (int u = 0; u < n; u++)
    {
        if (deg[u] == 0)
        {
            q.push(u);
        }
    }
    int seen = 0;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        seen++;
        for (int v : g[u])
        {
            if (--deg[v] == 0)
            {
                q.push(v);
            }
        }
    }
    if (seen == n)
    {
        return nullopt;
    }
    auto has = [&](long double x)
    {
        vector<long double> d(n);
        for (int k = 0; k < n; k++)
        {
            bool any = false;
            for (auto [u, v, a, b] : e)
            {
                if (d[v] > d[u] + a - x * b)
                {
                    d[v] = d[u] + a - x * b;
                    any = true;
                }
            }
            if (!any)
            {
                return false;
            }
            if (k == n - 1)
            {
                return true;
            }
        }
        return false;
    };
    for (int z = 0; z < it; z++)
    {
        long double m = midpoint(lo, hi);
        if (has(m))
        {
            hi = m;
        }
        else
        {
            lo = m;
        }
    }
    return hi;
}
