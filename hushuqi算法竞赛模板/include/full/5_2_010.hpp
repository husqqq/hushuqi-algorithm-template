#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct KEdge
{
    // u、v 是有向边端点，w 是非负边权。
    int u, v, w;
};

template<class Dist = long long>
optional<Dist> kthWalk(
    int n, const vector<KEdge> &e, int s, int t,
    unsigned long long k, bool keep0 = true)
{
    // k 从 1 开始；返回第 k 小有向游走长度，不存在时返回空。
    assert(k >= 1 && 0 <= s && s < n && 0 <= t && t < n); // 调试检查，可删。
    vector<vector<pair<int, int>>> g(n), rg(n);
    for (auto [u, v, w] : e)
    {
        assert(w >= 0); // 调试检查，可删。
        g[u].push_back({v, w});
        rg[v].push_back({u, w});
    }
    vector<optional<Dist>> h(n);
    priority_queue<pair<Dist, int>, vector<pair<Dist, int>>, greater<>> dq;
    h[t] = 0;
    dq.push({0, t});
    while (!dq.empty())
    {
        auto [d, u] = dq.top();
        dq.pop();
        if (!h[u].has_value() || d != *h[u])
        {
            continue;
        }
        for (auto [v, w] : rg[u])
        {
            Dist ew = (Dist)w;
            Dist nd = d + ew;
            if (!h[v].has_value() || nd < *h[v])
            {
                h[v] = nd;
                dq.push({nd, v});
            }
        }
    }
    if (!h[s].has_value())
    {
        return nullopt;
    }
    if (s == t && keep0)
    {
        if (k == 1)
        {
            return (Dist)0;
        }
        k--;
    }
    using State = tuple<Dist, Dist, int>;
    priority_queue<State, vector<State>, greater<>> q;
    vector<unsigned long long> cnt(n);
    for (auto [v, w] : g[s])
    {
        if (!h[v].has_value())
        {
            continue;
        }
        Dist ew = (Dist)w;
        Dist nd = ew;
        q.push({nd + *h[v], nd, v});
    }
    while (!q.empty())
    {
        auto [f, d, u] = q.top();
        q.pop();
        if (cnt[u] == k)
        {
            continue;
        }
        cnt[u]++;
        if (u == t && cnt[u] == k)
        {
            return d;
        }
        for (auto [v, w] : g[u])
        {
            if (h[v].has_value() && cnt[v] < k)
            {
                Dist ew = (Dist)w;
                Dist nd = d + ew;
                q.push({nd + *h[v], nd, v});
            }
        }
    }
    return nullopt;
}
