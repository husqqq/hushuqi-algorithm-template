#pragma once
#include <bits/stdc++.h>
#include "7_1_001.hpp"
using namespace std;

inline long long interpSeq(const vector<long long> &y, long long x, long long mod)
{
    // y[i]=f(i)，x 是查询点，mod 是素数；返回 f(x) 对 mod 的最小非负余数。
    int n = y.size();
    // 调试检查，可删。
    assert(n > 0 && n < mod);
    x %= mod;
    if (x < 0)
    {
        x += mod;
    }
    if (x < n)
    {
        long long ans = y[x] % mod;
        return ans < 0 ? ans + mod : ans;
    }
    vector<long long> fac(n, 1), ifac(n, 1), pre(n + 1, 1), suf(n + 1, 1);
    for (int i = 1; i < n; i++)
    {
        fac[i] = mulMod(fac[i - 1], i, mod);
    }
    ifac[n - 1] = powerMod(fac[n - 1], mod - 2, mod);
    for (int i = n - 1; i; i--)
    {
        ifac[i - 1] = mulMod(ifac[i], i, mod);
    }
    for (int i = 0; i < n; i++)
    {
        long long d = x >= i ? x - i : mod - (i - x);
        pre[i + 1] = mulMod(pre[i], d, mod);
    }
    for (int i = n - 1; i >= 0; i--)
    {
        long long d = x >= i ? x - i : mod - (i - x);
        suf[i] = mulMod(suf[i + 1], d, mod);
    }
    long long ans = 0;
    for (int i = 0; i < n; i++)
    {
        long long v = y[i] % mod;
        if (v < 0)
        {
            v += mod;
        }
        long long term = mulMod(v, pre[i], mod);
        term = mulMod(term, suf[i + 1], mod);
        term = mulMod(term, ifac[i], mod);
        term = mulMod(term, ifac[n - 1 - i], mod);
        if ((n - 1 - i) & 1)
        {
            term = term ? mod - term : 0;
        }
        ans = ans >= mod - term ? ans - (mod - term) : ans + term;
    }
    return ans;
}
