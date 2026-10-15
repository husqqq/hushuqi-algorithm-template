#pragma once

#include "12_1_001.hpp"

// 知识点 12.3.003：决策单调的分治优化。
namespace dc_dp
{
template <class Acc = long long, class Cost>
vector<Acc> divideLayer(const vector<Acc> &pre, int n, Cost w)
{
    // pre[j] 是上一层状态，n 是本层最大下标，w(j,i) 是转移代价；返回本层 dp，调用者必须保证最优决策单调。
    assert(n >= 0 && (int)pre.size() == n + 1); // 调试检查，可删。
    vector<Acc> dp(n + 1);
    auto solve = [&](auto &&self, int l, int r, int ql, int qr) -> void
    {
        // self 是递归自身，[l,r] 是待算区间，[ql,qr] 是候选决策范围；填充 dp，无返回值。
        if (l > r)
        {
            return;
        }
        int mid = l + (r - l) / 2;
        int best = ql;
        optional<Acc> val;
        for (int j = ql; j <= min(qr, mid - 1); j++)
        {
            Acc cur = pre[j] + (Acc)w(j, mid);
            if (!val || cur < *val)
            {
                val = cur;
                best = j;
            }
        }
        assert(val); // 候选区间非空，且 pre 中没有不可达状态。
        dp[mid] = *val;
        self(self, l, mid - 1, ql, best);
        self(self, mid + 1, r, best, qr);
    };
    solve(solve, 1, n, 0, n - 1);
    return dp;
}
}
