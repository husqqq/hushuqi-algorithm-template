#pragma once
#include <bits/stdc++.h>
using namespace std;

template <class T, class Add, class Mul>
vector<vector<T>> semiringMul(const vector<vector<T>> &a, const vector<vector<T>> &b,
                              T zero, Add add, Mul mul)
{
    // a、b 是待乘矩阵，zero 是加法单位元；add、mul 定义半环加法与乘法。
    int n = a.size();
    int m = b.size();
    int k = m ? b[0].size() : 0;
    // 调试检查，可删。
    for (const auto &r : a)
    {
        assert((int)r.size() == m);
    }
    // 调试检查，可删。
    for (const auto &r : b)
    {
        assert((int)r.size() == k);
    }
    vector c(n, vector<T>(k, zero));
    for (int i = 0; i < n; i++)
    {
        for (int z = 0; z < m; z++)
        {
            for (int j = 0; j < k; j++)
            {
                c[i][j] = add(c[i][j], mul(a[i][z], b[z][j]));
            }
        }
    }
    return c;
}

template <class T, class Add, class Mul>
vector<vector<T>> semiringPow(vector<vector<T>> a, unsigned long long b,
                              T zero, T one, Add add, Mul mul)
{
    // a 是方阵，b 是非负指数；zero、one 是加法与乘法单位元。
    int n = a.size();
    // 调试检查，可删。
    for (const auto &r : a)
    {
        assert((int)r.size() == n);
    }
    vector ans(n, vector<T>(n, zero));
    for (int i = 0; i < n; i++)
    {
        ans[i][i] = one;
    }
    while (b)
    {
        if (b & 1)
        {
            ans = semiringMul(ans, a, zero, add, mul);
        }
        b >>= 1;
        if (b)
        {
            a = semiringMul(a, a, zero, add, mul);
        }
    }
    return ans;
}
