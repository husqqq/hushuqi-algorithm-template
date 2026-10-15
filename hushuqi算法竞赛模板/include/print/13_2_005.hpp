#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<long double> expectedDAG(const vector<vector<pair<int, long double>>> &g,
                                const vector<long double> &cost,
                                const vector<int> &order)
{
    // g[u] 是从 u 指向后继 v 的 {v,概率}；cost[u] 是访问 u 时支付的期望即时代价；
    // order 是全部顶点的逆拓扑排列；返回各状态期望总代价，缺失概率表示直接结束。
    assert(g.size() == cost.size() && order.size() == g.size());
    vector<long double> e(cost.size());
    for (int u : order)
    {
        e[u] = cost[u];
        for (auto [v, p] : g[u])
        {
            e[u] += p * e[v];
        }
    }
    return e;
}
