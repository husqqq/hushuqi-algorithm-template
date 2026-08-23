#pragma once

#include "10_2_005.hpp"
using namespace std;
#define int long long

template <class T> vector<T> bell(int n)
{
    // n 是要计算的最大下标；返回 B_0 到 B_n 的 Bell 数。
    assert(n >= 0); // 调试检查，可删。
    vector<vector<T>> a(n + 1, vector<T>(n + 1));
    vector<T> b(n + 1);
    a[0][0] = 1;
    b[0] = 1;
    for (int i = 1; i <= n; i++)
    {
        a[i][0] = a[i - 1][i - 1];
        b[i] = a[i][0];
        for (int j = 1; j <= i; j++)
        {
            a[i][j] = a[i][j - 1] + a[i - 1][j - 1];
        }
    }
    return b;
}

vector<Z> bellFast(int n)
{
    // n 是最大下标且 n<998244353；返回 B_0 到 B_n，复杂度 O(n log n)。
    assert(0 <= n && n < mod);
    vector<Z> fac(n + 1, 1), ifac(n + 1, 1);
    for (int i = 1; i <= n; i++) fac[i] = fac[i - 1] * i;
    ifac[n] = fac[n].inv();
    for (int i = n; i; i--) ifac[i - 1] = ifac[i] * i;
    Poly f(n + 1);
    for (int i = 1; i <= n; i++) f[i] = ifac[i];
    f = fpsExp(f, n + 1);
    for (int i = 0; i <= n; i++) f[i] *= fac[i];
    return f;
}
