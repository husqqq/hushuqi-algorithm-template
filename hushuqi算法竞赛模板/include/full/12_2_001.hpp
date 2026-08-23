#pragma once

#include "12_1_001.hpp"

template<class Next, class Accept>
unsigned long long digitDP(long long n, int m, int s, Next next, Accept accept)
{
    if (n < 0)
    {
        return 0;
    }
    assert(m > 0 && 0 <= s && s < m);
    string a = to_string(n);
    vector dp(2, vector(2, vector<unsigned long long>(m)));
    dp[1][0][s] = 1;
    for (char c : a)
    {
        vector ndp(2, vector(2, vector<unsigned long long>(m)));
        for (int lim = 0; lim < 2; lim++)
        {
            for (int st = 0; st < 2; st++)
            {
                for (int q = 0; q < m; q++)
                {
                    for (int d = 0; d <= (lim ? c - '0' : 9); d++)
                    {
                        int nl = lim && d == c - '0';
                        int ns = st || d;
                        int nq = next(q, d, ns);
                        assert(0 <= nq && nq < m);
                        ndp[nl][ns][nq] += dp[lim][st][q];
                    }
                }
            }
        }
        dp.swap(ndp);
    }
    unsigned long long ans = 0;
    for (int lim = 0; lim < 2; lim++)
    {
        for (int st = 0; st < 2; st++)
        {
            for (int q = 0; q < m; q++)
            {
                if (accept(q, st))
                {
                    ans += dp[lim][st][q];
                }
            }
        }
    }
    return ans;
}
