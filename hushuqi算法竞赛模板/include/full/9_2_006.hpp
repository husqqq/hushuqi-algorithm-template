#pragma once
#include <bits/stdc++.h>
using namespace std;

namespace frac_prog
{
inline long double maxRatio(const vector<long double> &a, const vector<long double> &b, int k)
{
    // a、b 是分子分母贡献且 b[i] 为正，k 是恰选数量；返回最大比值。
    int n = a.size();
    // 调试检查，可删。
    assert((int)b.size() == n && 1 <= k && k <= n);
    long double l = numeric_limits<long double>::infinity();
    long double r = -l;
    for (int i = 0; i < n; i++)
    {
        // 调试检查，可删。
        assert(b[i] > 0);
        l = min(l, a[i] / b[i]);
        r = max(r, a[i] / b[i]);
    }
    vector<long double> c(n);
    for (int it = 0; it < 100; it++)
    {
        long double mid = (l + r) / 2;
        for (int i = 0; i < n; i++)
        {
            c[i] = a[i] - mid * b[i];
        }
        nth_element(c.begin(), c.end() - k, c.end());
        long double s = accumulate(c.end() - k, c.end(), 0.0L);
        if (s >= 0)
        {
            l = mid;
        }
        else
        {
            r = mid;
        }
    }
    return (l + r) / 2;
}
}
