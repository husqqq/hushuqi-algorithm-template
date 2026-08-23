#pragma once

#include "12_1_001.hpp"

template <class Acc = long long>
Acc wqsPartition(const vector<int> &pre, int K)
{
    static_assert(numeric_limits<Acc>::is_integer && numeric_limits<Acc>::is_signed &&
                  sizeof(Acc) >= sizeof(long long));
    int n = (int)pre.size() - 1;
    assert(n >= 1 && 1 <= K && K <= n && pre[0] == 0);
    for (int i = 1; i <= n; i++)
    {
        assert(pre[i] > pre[i - 1]);
    }
    Acc total = pre[n];
    if (K == 1)
    {
        return total * total;
    }
    auto check = [&](Acc lambda)
    {
        vector<Acc> dp(n + 1);
        vector<int> cnt(n + 1);
        for (int i = 1; i <= n; i++)
        {
            bool found = false;
            for (int j = 0; j < i; j++)
            {
                Acc seg = (Acc)pre[i] - pre[j];
                Acc cur = dp[j] + seg * seg + lambda;
                int c = cnt[j] + 1;
                if (!found || cur < dp[i] || (cur == dp[i] && c > cnt[i]))
                {
                    dp[i] = cur;
                    cnt[i] = c;
                    found = true;
                }
            }
        }
        return pair<Acc, int>{dp[n], cnt[n]};
    };
    Acc lo = 0;
    Acc hi = total * total + 1;
    Acc best = 0;
    while (lo <= hi)
    {
        Acc mid = lo + (hi - lo) / 2;
        if (check(mid).second >= K)
        {
            best = mid;
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }
    return check(best).first - best * K;
}
