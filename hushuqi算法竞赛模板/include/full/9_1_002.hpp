#pragma once
#include <bits/stdc++.h>
#include "../linear_algebra_field.hpp"
using namespace std;

template <class T> vector<vector<T>> matPow(vector<vector<T>> a, unsigned long long b)
{
    // a 是方阵，b 是非负指数；返回 a 的 b 次幂。
    int n = a.size();
    // 调试检查，可删。
    for (const auto &r : a)
    {
        assert((int)r.size() == n);
    }
    vector ans(n, vector<T>(n));
    for (int i = 0; i < n; i++)
    {
        ans[i][i] = 1;
    }
    auto mul = [&](const vector<vector<T>> &x, const vector<vector<T>> &y)
    {
        // x、y 是同阶方阵；返回普通矩阵乘积。
        vector z(n, vector<T>(n));
        for (int i = 0; i < n; i++)
        {
            for (int k = 0; k < n; k++)
            {
                for (int j = 0; j < n; j++)
                {
                    z[i][j] += x[i][k] * y[k][j];
                }
            }
        }
        return z;
    };
    while (b)
    {
        if (b & 1)
        {
            ans = mul(ans, a);
        }
        b >>= 1;
        if (b)
        {
            a = mul(a, a);
        }
    }
    return ans;
}
