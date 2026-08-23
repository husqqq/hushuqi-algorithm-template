#pragma once

#include "10_1_003.hpp"
using namespace std;
#define int long long

template <class T> vector<vector<T>> stirling1(int n)
{
    // n 是最大行号；返回有符号第一类 Stirling 数 s(i,j) 的第 0..n 行。
    assert(n >= 0); // 调试检查，可删。
    vector<vector<T>> a(n + 1, vector<T>(n + 1));
    a[0][0] = 1;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            a[i][j] = a[i - 1][j - 1] - T(i - 1) * a[i - 1][j];
        }
    }
    return a;
}

template <class T> vector<vector<T>> stirling2(int n)
{
    // n 是最大行号；返回第二类 Stirling 数 S(i,j) 的第 0..n 行。
    assert(n >= 0); // 调试检查，可删。
    vector<vector<T>> a(n + 1, vector<T>(n + 1));
    a[0][0] = 1;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            a[i][j] = a[i - 1][j - 1] + T(j) * a[i - 1][j];
        }
    }
    return a;
}

vector<Z> stirling1F(int n)
{
    // n 是行号；返回有符号第一类 Stirling 数 s(n,0..n)，复杂度 O(n log^2 n)。
    assert(0 <= n && n <= 500000);
    auto build = [&](auto &&self, int l, int r) -> vector<Z>
    {
        if (l == r) return {-Z(l), Z(1)};
        int m = (l + r) / 2;
        return conv(self(self, l, m), self(self, m + 1, r));
    };
    if (n == 0) return {1};
    return build(build, 0, n - 1);
}

vector<Z> stirling2F(int n)
{
    // n 是行号；返回第二类 Stirling 数 S(n,0..n)，复杂度 O(n log n)。
    assert(0 <= n && n < mod);
    vector<Z> fac(n + 1, 1), ifac(n + 1, 1);
    for (int i = 1; i <= n; i++) fac[i] = fac[i - 1] * i;
    ifac[n] = fac[n].inv();
    for (int i = n; i; i--) ifac[i - 1] = ifac[i] * i;
    vector<Z> a(n + 1), b(n + 1);
    for (int i = 0; i <= n; i++)
    {
        a[i] = Z(i).pow(n) * ifac[i];
        b[i] = i & 1 ? -ifac[i] : ifac[i];
    }
    auto ans = conv(move(a), move(b));
    ans.resize(n + 1);
    return ans;
}
