#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

unsigned __int128 addMod128(unsigned __int128 x, unsigned __int128 y,
                            unsigned __int128 mod)
{
    // x、y 均小于正模数 mod；返回 (x+y) mod mod，计算中不发生无符号回绕。
    return x >= mod - y ? x - (mod - y) : x + y;
}

unsigned __int128 subMod128(unsigned __int128 x, unsigned __int128 y,
                            unsigned __int128 mod)
{
    // x、y 均小于正模数 mod；返回 (x-y) mod mod。
    return x >= y ? x - y : mod - (y - x);
}

unsigned __int128 mulMod128(unsigned __int128 x, unsigned __int128 y,
                            unsigned __int128 mod)
{
    // x、y 均小于正模数 mod；返回 (x*y) mod mod。
    if (mod <= ((unsigned __int128)1 << 64))
    {
        return x * y % mod;
    }
    unsigned __int128 r = 0;
    while (y)
    {
        if (y & 1)
        {
            r = addMod128(r, x, mod);
        }
        y >>= 1;
        if (y)
        {
            x = addMod128(x, x, mod);
        }
    }
    return r;
}

void xorFwt(vector<unsigned __int128> &a, unsigned __int128 mod)
{
    // a 的各项均小于正模数 mod；原地完成异或 FWT。
    int n = a.size();
    for (int len = 1; len < n; len <<= 1)
    {
        for (int l = 0; l < n; l += len << 1)
        {
            for (int i = 0; i < len; i++)
            {
                unsigned __int128 x = a[l + i];
                unsigned __int128 y = a[l + len + i];
                a[l + i] = addMod128(x, y, mod);
                a[l + len + i] = subMod128(x, y, mod);
            }
        }
    }
}

vector<long long> xorConv(const vector<long long> &a, const vector<long long> &b,
                          long long mod)
{
    // a、b 是等长整数序列，mod 是正模数；返回最小非负代表元组成的异或卷积。
    assert(mod > 0 && !a.empty() && a.size() == b.size() &&
           has_single_bit(a.size())); // 调试检查，可删。
    int n = a.size();
    unsigned __int128 lifted = (unsigned __int128)(unsigned long long)mod * n;
    vector<unsigned __int128> x(n);
    vector<unsigned __int128> y(n);
    for (int i = 0; i < n; i++)
    {
        long long u = a[i] % mod;
        long long v = b[i] % mod;
        if (u < 0)
        {
            u += mod;
        }
        if (v < 0)
        {
            v += mod;
        }
        x[i] = (unsigned long long)u;
        y[i] = (unsigned long long)v;
    }
    xorFwt(x, lifted);
    xorFwt(y, lifted);
    for (int i = 0; i < n; i++)
    {
        x[i] = mulMod128(x[i], y[i], lifted);
    }
    xorFwt(x, lifted);
    vector<long long> c(n);
    for (int i = 0; i < n; i++)
    {
        assert(x[i] % n == 0); // 调试检查，可删。
        c[i] = (long long)(x[i] / n % (unsigned long long)mod);
    }
    return c;
}
