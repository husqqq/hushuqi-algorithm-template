#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace roots_unity
{
constexpr int mod = 998244353;

int qpow(int a, int n)
{
    // a 是底数，n 是非负指数；返回模 mod 幂。
    assert(n >= 0); // 调试检查，可删。
    a %= mod;
    if (a < 0)
    {
        a += mod;
    }
    int r = 1;
    while (n)
    {
        if (n & 1)
        {
            r = r * a % mod;
        }
        a = a * a % mod;
        n >>= 1;
    }
    return r;
}

int rootFilter(const vector<int> &a, int m, int rem)
{
    // a 是多项式系数，m 是整除 mod-1 的正整数，rem 是余数；返回次数同余 rem 的系数和。
    assert(m > 0 && (mod - 1) % m == 0); // 调试检查，可删。
    rem %= m;
    if (rem < 0)
    {
        rem += m;
    }
    int w = qpow(3, (mod - 1) / m);
    int ans = 0;
    for (int j = 0; j < m; j++)
    {
        int x = qpow(w, j);
        int y = 0;
        for (auto it = a.rbegin(); it != a.rend(); ++it)
        {
            int c = *it % mod;
            if (c < 0)
            {
                c += mod;
            }
            y = (y * x + c) % mod;
        }
        int e = ((m - rem) % m) * j % (mod - 1);
        ans = (ans + y * qpow(w, e)) % mod;
    }
    return ans * qpow(m, mod - 2) % mod;
}
}
