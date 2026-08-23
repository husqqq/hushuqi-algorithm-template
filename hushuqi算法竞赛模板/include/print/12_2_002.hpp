#pragma once

#include "12_1_001.hpp"

namespace bitmask_dp
{
int tsp(const vector<vector<int>> &w)
{
    int n = w.size();
    assert(1 <= n && n <= 22);
    for (auto &row : w)
    {
        assert((int)row.size() == n);
    }
    if (n == 1)
    {
        return 0;
    }
    vector<vector<int>> dp(1LL << n, vector<int>(n));
    vector<vector<char>> ok(1LL << n, vector<char>(n));
    ok[1][0] = true;
    for (int s = 1; s < (1LL << n); s++)
    {
        for (int u = 0; u < n; u++)
        {
            if (!(s >> u & 1) || !ok[s][u])
            {
                continue;
            }
            for (int v = 0; v < n; v++)
            {
                if (s >> v & 1)
                {
                    continue;
                }
                int ns = s | 1LL << v;
                int nd = dp[s][u] + w[u][v];
                if (!ok[ns][v] || nd < dp[ns][v])
                {
                    dp[ns][v] = nd;
                    ok[ns][v] = true;
                }
            }
        }
    }
    int ans = 0;
    bool found = false;
    for (int u = 0; u < n; u++)
    {
        if (ok.back()[u])
        {
            int cur = dp.back()[u] + w[u][0];
            if (!found || cur < ans)
            {
                ans = cur;
                found = true;
            }
        }
    }
    return ans;
}
}
