#pragma once
#include <bits/stdc++.h>
using namespace std;

struct LDLT
{
    // l 是单位下三角因子，满足 A=l*diag(d)*l^T；只使用其下三角部分。
    vector<vector<long double>> l;
    // d 是对角因子；每个元素都必须远离零枢轴。
    vector<long double> d;
    // eps 是判断对称误差和零枢轴的绝对容差。
    long double eps;
    // valid 表示构造时是否成功完成无主元交换的分解。
    bool valid;

    explicit LDLT(const vector<vector<long double>> &a, long double tol = 1E-12L)
        : l(a.size(), vector<long double>(a.size())), d(a.size()), eps(tol), valid(true)
    {
        // a 是方阵且应对称，tol 是非负的对称性与枢轴判定容差；保存 A 的 LDL^T 分解。
        assert(tol >= 0); // 调试检查，可删。
        int n = (int)a.size();
        for (int i = 0; i < n; i++)
        {
            assert((int)a[i].size() == n); // 调试检查，可删。
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                assert(fabsl(a[i][j] - a[j][i]) <= eps); // 调试检查，可删。
            }
            l[i][i] = 1;
        }
        for (int j = 0; j < n && valid; j++)
        {
            long double diag = a[j][j];
            for (int k = 0; k < j; k++)
            {
                diag -= l[j][k] * l[j][k] * d[k];
            }
            if (fabsl(diag) <= eps)
            {
                valid = false;
                break;
            }
            d[j] = diag;
            for (int i = j + 1; i < n; i++)
            {
                long double value = a[i][j];
                for (int k = 0; k < j; k++)
                {
                    value -= l[i][k] * d[k] * l[j][k];
                }
                l[i][j] = value / d[j];
            }
        }
    }

    optional<vector<long double>> solve(const vector<long double> &b) const
    {
        // b 是长度等于矩阵阶数的右端向量；返回 Ax=b 的解，无效分解时返回空。
        assert((int)b.size() == (int)d.size()); // 调试检查，可删。
        if (!valid)
        {
            return nullopt;
        }
        int n = (int)d.size();
        vector<long double> y = b;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < i; j++)
            {
                y[i] -= l[i][j] * y[j];
            }
        }
        vector<long double> z(n);
        for (int i = 0; i < n; i++)
        {
            z[i] = y[i] / d[i];
        }
        vector<long double> x = z;
        for (int i = n - 1; i >= 0; i--)
        {
            for (int j = i + 1; j < n; j++)
            {
                x[i] -= l[j][i] * x[j];
            }
        }
        return x;
    }
};
