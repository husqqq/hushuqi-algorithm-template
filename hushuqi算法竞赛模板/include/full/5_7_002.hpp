#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

pair<int, vector<pair<int, int>>>
weightMatch(const vector<vector<optional<long long>>> &w)
{
    // w[u][v] 是边权，空值表示无边；返回最大总权和一组匹配边。
    int n = w.size();
    assert(n <= 24); // 调试检查，可删：状态数必须能承受。
    for (const auto &row : w)
    {
        assert((int)row.size() == n); // 调试检查，可删。
    }
    size_t tot = 1ULL << n;
    vector<int> dp(tot);
    vector<int8_t> take(tot, -1);
    dp[0] = 0;
    for (size_t s = 1; s < tot; s++)
    {
        int u = countr_zero((unsigned long long)s);
        dp[s] = dp[s ^ (1ULL << u)];
        for (int v = u + 1; v < n; v++)
        {
            if ((s >> v & 1) && w[u][v].has_value())
            {
                int z = dp[s ^ (1ULL << u) ^ (1ULL << v)] + *w[u][v];
                if (z > dp[s])
                {
                    dp[s] = z;
                    take[s] = v;
                }
            }
        }
    }
    vector<pair<int, int>> e;
    for (size_t s = tot - 1; s;)
    {
        int u = countr_zero((unsigned long long)s), v = take[s];
        s ^= 1ULL << u;
        if (v >= 0)
        {
            s ^= 1ULL << v;
            e.push_back({u, v});
        }
    }
    return pair{dp.back(), e};
}
