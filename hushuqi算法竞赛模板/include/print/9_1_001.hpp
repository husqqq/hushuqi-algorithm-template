#pragma once
#include <bits/stdc++.h>
#include "support/linear_algebra_field.hpp"
using namespace std;

template <class T> struct Mat
{
    // n、m 是行数与列数；a 始终应有 n 行，每行恰有 m 个元素。
    int n, m;
    vector<vector<T>> a;

    Mat(int n = 0, int m = 0) : n(n), m(m), a(n, vector<T>(m))
    {
        // 建立 n 行 m 列的零矩阵；不要直接改变 a 的行数或行宽。
    }

    static Mat eye(int n)
    {
        // n 是矩阵阶数；返回 n 阶单位矩阵。
        Mat a(n, n);
        for (int i = 0; i < n; i++)
        {
            a[i][i] = 1;
        }
        return a;
    }

    vector<T> &operator[](int x)
    {
        // x 是行下标；返回该行的可修改引用。
        return a[x];
    }

    const vector<T> &operator[](int x) const
    {
        // x 是行下标；返回该行的只读引用。
        return a[x];
    }

    friend Mat operator*(const Mat &x, const Mat &y)
    {
        // x、y 是乘法左右两侧的矩阵；返回维数相容时的普通乘积。
        // 调试检查，可删。
        assert((int)x.a.size() == x.n && (int)y.a.size() == y.n && x.m == y.n);
        // 调试检查，可删。
        for (const auto &r : x.a)
        {
            assert((int)r.size() == x.m);
        }
        // 调试检查，可删。
        for (const auto &r : y.a)
        {
            assert((int)r.size() == y.m);
        }
        Mat z(x.n, y.m);
        for (int i = 0; i < x.n; i++)
        {
            for (int k = 0; k < x.m; k++)
            {
                for (int j = 0; j < y.m; j++)
                {
                    z[i][j] += x[i][k] * y[k][j];
                }
            }
        }
        return z;
    }

    Mat pow(unsigned long long b) const
    {
        // b 是非负指数；返回当前方阵的 b 次幂。
        // 调试检查，可删。
        assert(n == m);
        Mat x = *this;
        Mat ans = eye(n);
        while (b)
        {
            if (b & 1)
            {
                ans = ans * x;
            }
            b >>= 1;
            if (b)
            {
                x = x * x;
            }
        }
        return ans;
    }
};
