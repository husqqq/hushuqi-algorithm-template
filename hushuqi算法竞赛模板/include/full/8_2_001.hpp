#pragma once

#include <bits/stdc++.h>
#include "../combinatorics_mod.hpp"
using namespace std;
#define int long long

array<int, 2> fibPair(unsigned long long n, int mod)
{
    // n 是下标，mod 是正模数；返回 {F_n,F_{n+1}} mod mod。
    assert(mod > 0); // 调试检查，可删。
    if (n == 0)
    {
        return {0, 1 % mod};
    }
    auto [a, b] = fibPair(n >> 1, mod);
    int t = b >= mod - b ? b - (mod - b) : b + b;
    t -= a;
    if (t < 0)
    {
        t += mod;
    }
    int c = combMulMod(a, t, mod);
    int x = combMulMod(a, a, mod);
    int y = combMulMod(b, b, mod);
    int d = x >= mod - y ? x - (mod - y) : x + y;
    if (n & 1)
    {
        int next = c >= mod - d ? c - (mod - d) : c + d;
        return {d, next};
    }
    return {c, d};
}

int lucasNumber(unsigned long long n, int mod)
{
    // n 是下标，mod 是正模数；返回 Lucas 数 L_n mod mod。
    assert(mod > 0); // 调试检查，可删。
    if (n == 0)
    {
        return 2 % mod;
    }
    auto [fn, fn1] = fibPair(n, mod);
    int ans = fn1 >= mod - fn1 ? fn1 - (mod - fn1) : fn1 + fn1;
    ans -= fn;
    return ans < 0 ? ans + mod : ans;
}
