#pragma once

#include "12_1_001.hpp"

// 知识点 12.3.002：满足 Knuth 条件的区间 DP。
namespace quad_opt
{
template <class Acc = long long, class Cost>
Acc knuth(int n, Cost w)
{
    // n 是序列长度，w(l,r) 返回半开区间代价；返回 dp[0][n]，调用者必须证明 Knuth 单调性条件。
    assert(n >= 0); // 调试检查，可删。
    vector<vector<Acc>> dp(n + 1, vector<Acc>(n + 1));
    vector<vector<int>> opt(n + 1, vector<int>(n + 1));
    for (int i = 0; i < n; i++)
    {
        opt[i][i] = i;
        opt[i][i + 1] = i + 1;
    }
    opt[n][n] = n;
    for (int len = 2; len <= n; len++)
    {
        for (int l = 0; l + len <= n; l++)
        {
            int r = l + len;
            optional<Acc> val;
            int lo = max(l + 1, opt[l][r - 1]);
            int hi = min(r - 1, opt[l + 1][r]);
            for (int k = lo; k <= hi; k++)
            {
                Acc cur = dp[l][k] + dp[k][r] + (Acc)w(l, r);
                if (!val || cur < *val)
                {
                    val = cur;
                    opt[l][r] = k;
                }
            }
            assert(val); // Knuth 条件下该区间至少有一个候选。
            dp[l][r] = *val;
        }
    }
    return dp[0][n];
}
}
