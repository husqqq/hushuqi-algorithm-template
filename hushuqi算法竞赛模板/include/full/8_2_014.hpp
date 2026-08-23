#pragma once

#include "10_2_005.hpp"

vector<Z> stirling1K(int n, int k)
{
    // n 是最大行号，k 是固定列号；返回 s(k,k) 到 s(n,k)，模数由 Z 决定。
    assert(0 <= k && k <= n && n < mod); // 调试检查，可删。
    Poly f(n + 1);
    for (int i = 1; i <= n; i++)
    {
        f[i] = (i & 1 ? Z(1) : Z(-1)) / Z(i);
    }
    Poly g = fpsPow(f, k, n + 1);
    Z fk = 1;
    for (int i = 1; i <= k; i++)
    {
        fk *= i;
    }
    Z ifk = fk.inv();
    Z fac = 1;
    vector<Z> ans(n - k + 1);
    for (int i = 0; i <= n; i++)
    {
        if (i >= k)
        {
            ans[i - k] = g[i] * fac * ifk;
        }
        fac *= i + 1;
    }
    return ans;
}
