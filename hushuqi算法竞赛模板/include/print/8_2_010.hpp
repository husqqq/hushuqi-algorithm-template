#pragma once

#include <bits/stdc++.h>
#include "support/combinatorics_mod.hpp"
using namespace std;
#define int long long

namespace tree_count
{
int qpow(int a, int n, int mod)
{
    // a、n、mod 是底数、非负指数和正模数；返回模幂。
    assert(n >= 0 && mod > 0); // 调试检查，可删。
    a %= mod;
    if (a < 0)
    {
        a += mod;
    }
    int r = 1 % mod;
    while (n)
    {
        if (n & 1)
        {
            r = combMulMod(r, a, mod);
        }
        a = combMulMod(a, a, mod);
        n >>= 1;
    }
    return r;
}

array<int, 2> cayley(int n, int mod)
{
    // n 是有标号点数，mod 是正模数；返回 {无根树数, 根标号被区分的有根树数}。
    assert(n >= 1 && mod > 0); // 调试检查，可删。
    int unrooted = n == 1 ? 1 % mod : qpow(n % mod, n - 2, mod);
    int rooted = qpow(n % mod, n - 1, mod);
    return {unrooted, rooted};
}
}
