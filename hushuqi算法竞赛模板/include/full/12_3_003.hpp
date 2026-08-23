#pragma once

#include "12_1_001.hpp"

namespace dc_dp
{
template <class Acc = long long, class Cost>
vector<Acc> divideLayer(const vector<Acc> &pre, int n, Cost w)
{
    assert(n >= 0 && (int)pre.size() == n + 1);
    vector<Acc> dp(n + 1);
    auto solve = [&](auto &&self, int l, int r, int ql, int qr) -> void
    {
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
        assert(val);
        dp[mid] = *val;
        self(self, l, mid - 1, ql, best);
        self(self, mid + 1, r, best, qr);
    };
    solve(solve, 1, n, 0, n - 1);
    return dp;
}
}
