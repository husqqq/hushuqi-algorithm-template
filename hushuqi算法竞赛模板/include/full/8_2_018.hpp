#pragma once

#include "8_2_011.hpp"

Z lgvPaths(const vector<vector<pair<int, Z>>> &g, const vector<int> &ord,
           const vector<int> &s, const vector<int> &t)
{
    // g[u] 保存 DAG 的 {终点,边权}，ord 是全部顶点的正向拓扑排列；
    // s、t 是等长有序起终点表；返回顶点不相交路径组的带符号权和。
    int n = g.size(), k = s.size();
    assert((int)ord.size() == n && (int)t.size() == k);
    vector a(k, vector<Z>(k));
    for (int i = 0; i < k; i++)
    {
        vector<Z> dp(n);
        dp[s[i]] = 1;
        for (int u : ord)
        {
            for (auto [v, w] : g[u])
            {
                dp[v] += dp[u] * w;
            }
        }
        for (int j = 0; j < k; j++)
        {
            a[i][j] = dp[t[j]];
        }
    }
    return detField(move(a));
}
