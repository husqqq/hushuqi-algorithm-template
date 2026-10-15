#pragma once
#include <bits/stdc++.h>
using namespace std;

namespace strassen_mul
{
template <class T> using Matrix = vector<vector<T>>;

template <class T> Matrix<T> add(const Matrix<T> &a, const Matrix<T> &b, int sign = 1)
{
    int n = a.size();
    Matrix<T> c(n, vector<T>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            c[i][j] = a[i][j] + (sign == 1 ? b[i][j] : -b[i][j]);
        }
    }
    return c;
}

template <class T> Matrix<T> rec(const Matrix<T> &a, const Matrix<T> &b)
{
    int n = a.size();
    if (n <= 32)
    {
        Matrix<T> c(n, vector<T>(n));
        for (int i = 0; i < n; i++)
        {
            for (int k = 0; k < n; k++)
            {
                for (int j = 0; j < n; j++)
                {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        return c;
    }
    int m = n / 2;
    Matrix<T> x[2][2], y[2][2];
    for (int p = 0; p < 2; p++)
    {
        for (int q = 0; q < 2; q++)
        {
            x[p][q].assign(m, vector<T>(m));
            y[p][q].assign(m, vector<T>(m));
            for (int i = 0; i < m; i++)
            {
                copy_n(a[i + p * m].begin() + q * m, m, x[p][q][i].begin());
                copy_n(b[i + p * m].begin() + q * m, m, y[p][q][i].begin());
            }
        }
    }
    auto p1 = rec(add(x[0][0], x[1][1]), add(y[0][0], y[1][1]));
    auto p2 = rec(add(x[1][0], x[1][1]), y[0][0]);
    auto p3 = rec(x[0][0], add(y[0][1], y[1][1], -1));
    auto p4 = rec(x[1][1], add(y[1][0], y[0][0], -1));
    auto p5 = rec(add(x[0][0], x[0][1]), y[1][1]);
    auto p6 = rec(add(x[1][0], x[0][0], -1), add(y[0][0], y[0][1]));
    auto p7 = rec(add(x[0][1], x[1][1], -1), add(y[1][0], y[1][1]));
    Matrix<T> c(n, vector<T>(n));
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            c[i][j] = p1[i][j] + p4[i][j] - p5[i][j] + p7[i][j];
            c[i][j + m] = p3[i][j] + p5[i][j];
            c[i + m][j] = p2[i][j] + p4[i][j];
            c[i + m][j + m] = p1[i][j] - p2[i][j] + p3[i][j] + p6[i][j];
        }
    }
    return c;
}

template <class T> Matrix<T> strassen(const Matrix<T> &a, const Matrix<T> &b)
{
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
    if (!n || !m || !k)
    {
        return Matrix<T>(n, vector<T>(k));
    }
    int z = (int)bit_ceil((unsigned long long)max({n, m, k}));
    Matrix<T> x(z, vector<T>(z)), y(z, vector<T>(z));
    for (int i = 0; i < n; i++)
    {
        copy(a[i].begin(), a[i].end(), x[i].begin());
    }
    for (int i = 0; i < m; i++)
    {
        copy(b[i].begin(), b[i].end(), y[i].begin());
    }
    x = rec(x, y);
    x.resize(n);
    for (auto &r : x)
    {
        r.resize(k);
    }
    return x;
}
}
