#pragma once

#include <bits/stdc++.h>
#include "1_1_008.hpp"
#include "7_1_001.hpp"
using namespace std;
#define int long long

unsigned long long triangles(int n, const vector<pair<int, int>> &e)
{
    // e 是简单无向图边集；返回三元环数量。
    vector<int> d(n);
    for (auto [u, v] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v); // 调试检查，可删。
        d[u]++;
        d[v]++;
    }
    vector<vector<int>> g(n);
    for (auto [u, v] : e)
    {
        if (pair{d[u], u} > pair{d[v], v})
        {
            swap(u, v);
        }
        g[u].push_back(v);
    }
    vector<int> vis(n);
    unsigned long long ans = 0;
    for (int u = 0; u < n; u++)
    {
        for (int v : g[u])
        {
            vis[v] = 1;
        }
        for (int v : g[u])
        {
            for (int w : g[v])
            {
                ans += vis[w];
            }
        }
        for (int v : g[u])
        {
            vis[v] = 0;
        }
    }
    return ans;
}

long long weightedTri(int n, const vector<long long> &val, const vector<pair<int, int>> &e, long long mod)
{
    // n 是点数，val 是每点权值，e 是简单无向边集，mod 是正模数；返回所有三元环三点权乘积之和模 mod。
    assert((int)val.size() == n && mod > 0); // 调试检查，可删。
    vector<int> d(n);
    for (auto [u, v] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v); // 调试检查，可删。
        d[u]++;
        d[v]++;
    }
    vector<vector<int>> g(n);
    for (auto [u, v] : e)
    {
        if (pair{d[u], u} > pair{d[v], v})
        {
            swap(u, v);
        }
        g[u].push_back(v);
    }
    vector<int> mark(n, -1);
    long long ans = 0;
    for (int u = 0; u < n; u++)
    {
        for (int v : g[u]) mark[v] = u;
        for (int v : g[u])
        {
            for (int w : g[v])
            {
                if (mark[w] == u)
                {
                    long long cur = mulMod(val[u], val[v], mod);
                    ans += mulMod(cur, val[w], mod);
                    if (ans >= mod)
                    {
                        ans -= mod;
                    }
                }
            }
        }
    }
    return (ans + mod) % mod;
}

inline int triWeight998(int n, const vector<int> &value,
                        const vector<pair<int, int>> &edges)
{
    // value 是点权，edges 是简单无向边；返回三元环三点权乘积和模 998244353。
    assert((int)value.size() == n); // 调试检查，可删。
    vector<int> deg(n), ord(n), rk(n);
    for (auto [u, v] : edges)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v); // 调试检查，可删。
        deg[u]++, deg[v]++;
    }
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int u, int v)
    {
        return pair{deg[u], u} < pair{deg[v], v};
    });
    for (int i = 0; i < n; i++) rk[ord[i]] = i;
    vector<Z> w(n);
    for (int i = 0; i < n; i++) w[i] = value[ord[i]];
    vector<vector<int>> g(n);
    for (auto [u, v] : edges)
    {
        u = rk[u], v = rk[v];
        if (u > v) swap(u, v);
        g[v].push_back(u);
    }
    vector<int> mark(n, -1);
    Z ans = 0;
    for (int u = 0; u < n; u++)
    {
        for (int v : g[u]) mark[v] = u;
        Z sum = 0;
        for (int v : g[u])
        {
            Z cur = 0;
            for (int x : g[v]) if (mark[x] == u) cur += w[x];
            sum += cur * w[v];
        }
        ans += sum * w[u];
    }
    return ans.val();
}

unsigned long long fourCycles(int n, const vector<pair<int, int>> &e)
{
    // e 是简单无向图边集；返回四元环数量。
    int blocks = (n + 63) / 64;
    vector a(n, vector<unsigned long long>(blocks));
    for (auto [u, v] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v); // 调试检查，可删。
        a[u][v >> 6] |= 1ULL << (v & 63);
        a[v][u >> 6] |= 1ULL << (u & 63);
    }
    unsigned long long ans = 0;
    for (int u = 0; u < n; u++)
    {
        for (int v = u + 1; v < n; v++)
        {
            long long c = 0;
            for (int k = 0; k < blocks; k++)
            {
                c += popcount(a[u][k] & a[v][k]);
            }
            ans += (unsigned long long)c * (c - 1) / 2;
        }
    }
    return ans / 2;
}

vector<long long> fourCycleCnt(int n, const vector<pair<int, int>> &edges)
{
    // n 是点数，edges 是允许重边的无向边；返回每条原边参与的四元环数量。
    int m = edges.size();
    vector<int> deg(n), ord(n), rk(n);
    for (auto [u, v] : edges)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v); // 调试检查，可删。
        deg[u]++;
        deg[v]++;
    }
    iota(ord.begin(), ord.end(), 0);
    stable_sort(ord.begin(), ord.end(), [&](int u, int v)
    {
        return deg[u] < deg[v];
    });
    for (int i = 0; i < n; i++) rk[ord[i]] = i;
    map<pair<int, int>, int> pos;
    vector<pair<int, int>> es;
    vector<vector<int>> group;
    for (int id = 0; id < m; id++)
    {
        auto [u, v] = edges[id];
        u = rk[u];
        v = rk[v];
        if (u < v) swap(u, v);
        auto [it, fresh] = pos.emplace(pair{u, v}, (int)es.size());
        if (fresh)
        {
            es.push_back({u, v});
            group.push_back({});
        }
        group[it->second].push_back(id);
    }
    vector<long long> mul(es.size()), ans(es.size());
    vector<vector<pair<int, int>>> g(n);
    for (int id = 0; id < (int)es.size(); id++)
    {
        auto [u, v] = es[id];
        mul[id] = group[id].size();
        g[u].push_back({v, id});
        g[v].push_back({u, id});
    }
    for (auto &a : g) sort(a.begin(), a.end());
    vector<long long> path(n);
    for (int top = n - 1; top >= 0; top--)
    {
        for (auto [mid, a] : g[top])
        {
            if (mid >= top) break;
            int lim = lower_bound(g[mid].begin(), g[mid].end(), pair{top, -1}) - g[mid].begin();
            for (int i = 0; i < lim; i++)
            {
                auto [bot, b] = g[mid][i];
                path[bot] += mul[a] * mul[b];
            }
        }
        for (auto [mid, a] : g[top])
        {
            if (mid >= top) break;
            int lim = lower_bound(g[mid].begin(), g[mid].end(), pair{top, -1}) - g[mid].begin();
            for (int i = 0; i < lim; i++)
            {
                auto [bot, b] = g[mid][i];
                long long other = path[bot] - mul[a] * mul[b];
                ans[a] += other * mul[b];
                ans[b] += other * mul[a];
            }
        }
        for (auto [mid, a] : g[top])
        {
            if (mid >= top) break;
            int lim = lower_bound(g[mid].begin(), g[mid].end(), pair{top, -1}) - g[mid].begin();
            for (int i = 0; i < lim; i++) path[g[mid][i].first] = 0;
        }
    }
    vector<long long> res(m);
    for (int i = 0; i < (int)es.size(); i++)
    {
        for (int id : group[i]) res[id] = ans[i];
    }
    return res;
}
