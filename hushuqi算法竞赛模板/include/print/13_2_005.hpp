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
    vector<bool> done(g.size()); // 后继必须已求值，每个点恰处理一次。
    for (int u : order)
    {
        assert(0 <= u && u < (int)g.size() && !done[u]);
        assert(isfinite(cost[u]));
        e[u] = cost[u];
        long double sum = 0;
        for (auto [v, p] : g[u])
        {
            assert(0 <= v && v < (int)g.size() && done[v]);
            assert(isfinite(p) && p >= 0);
            sum += p;
            e[u] += p * e[v];
        }
        assert(sum <= 1 + 1E-12L);
        done[u] = true;
    }
    return e;
}
