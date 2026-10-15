#pragma once
#include <bits/stdc++.h>
#include "../linear_algebra_field.hpp"
using namespace std;

template <class T> vector<T> charPoly(const vector<vector<T>> &a)
{
    // a 是域上的方阵；返回 det(xI-a) 从常数项到最高次项的系数。
    int n = a.size();
    // 调试检查，可删。
    for (const auto &r : a)
    {
        assert((int)r.size() == n);
    }
    vector h = a;
    for (int col = 0; col + 2 < n; col++)
    {
        int p = col + 1;
        while (p < n && h[p][col] == T(0)) p++;
        if (p == n) continue;
        if (p != col + 1)
        {
            swap(h[p], h[col + 1]);
            for (int i = 0; i < n; i++) swap(h[i][p], h[i][col + 1]);
        }
        T inv = T(1) / h[col + 1][col];
        for (int row = col + 2; row < n; row++)
        {
            if (h[row][col] == T(0)) continue;
            T q = h[row][col] * inv;
            for (int j = col; j < n; j++) h[row][j] -= q * h[col + 1][j];
            for (int i = 0; i < n; i++) h[i][col + 1] += q * h[i][row];
        }
    }
    vector<vector<T>> p(n + 1);
    p[0] = {T(1)};
    for (int i = 1; i <= n; i++)
    {
        p[i].assign(i + 1, T(0));
        for (int j = 0; j < i; j++)
        {
            p[i][j] -= p[i - 1][j] * h[i - 1][i - 1];
            p[i][j + 1] += p[i - 1][j];
        }
        T prod = 1;
        for (int len = 1; len < i; len++)
        {
            prod *= h[i - len][i - len - 1];
            T q = prod * h[i - len - 1][i - 1];
            for (int j = 0; j <= i - len - 1; j++)
            {
                p[i][j] -= q * p[i - len - 1][j];
            }
        }
    }
    return p[n];
}

template <class T> vector<T> detAPlusBz(vector<vector<T>> a,
                                        vector<vector<T>> b)
{
    // a、b 是域上的同阶方阵；返回 det(a+bz) 从常数项到最高次项的系数，允许 b 奇异。
    int n = a.size(), shift = 0;
    assert((int)b.size() == n); // 调试检查，可删
    for (int i = 0; i < n; i++)
        assert((int)a[i].size() == n && (int)b[i].size() == n); // 调试检查，可删
    T scale = T(1);
    for (int col = 0; col < n; col++)
    {
        while (true)
        {
            for (int p = 0; p < col; p++)
            {
                T w = b[p][col];
                for (int i = 0; i < n; i++) a[i][col] -= w * a[i][p];
                b[p][col] = T(0);
            }
            int p = col;
            while (p < n && b[p][col] == T(0)) p++;
            if (p < n) break;
            if (++shift > n) return vector<T>(n + 1);
            for (int i = 0; i < n; i++) b[i][col] = a[i][col], a[i][col] = T(0);
        }
        int p = col;
        while (b[p][col] == T(0)) p++;
        if (p != col) scale = -scale, swap(a[p], a[col]), swap(b[p], b[col]);
        scale *= b[col][col];
        T inv = T(1) / b[col][col];
        for (int j = 0; j < n; j++) a[col][j] *= inv, b[col][j] *= inv;
        for (int i = col + 1; i < n; i++)
        {
            T w = b[i][col];
            for (int j = 0; j < n; j++) a[i][j] -= w * a[col][j], b[i][j] -= w * b[col][j];
        }
    }
    for (auto &row : a) for (T &x : row) x = -x;
    for (int col = 0; col + 1 < n; col++)
    {
        int p = col + 1;
        while (p < n && a[p][col] == T(0)) p++;
        if (p == n) continue;
        swap(a[p], a[col + 1]);
        for (int i = 0; i < n; i++) swap(a[i][p], a[i][col + 1]);
        T inv = T(1) / a[col + 1][col];
        for (int i = col + 2; i < n; i++)
        {
            T w = a[i][col] * inv;
            for (int j = 0; j < n; j++)
                a[i][j] -= w * a[col + 1][j], a[j][col + 1] += w * a[j][i];
        }
    }
    vector<vector<T>> f(n + 1);
    f[0] = {T(1)};
    for (int m = 1; m <= n; m++)
    {
        f[m].assign(m + 1, T(0));
        for (int i = 0; i < m; i++)
        {
            T w = a[i][m - 1];
            for (int k = i + 1; k < m; k++) w *= a[k][k - 1];
            for (int d = 0; d <= i; d++) f[m][d] -= w * f[i][d];
        }
        for (int d = 0; d < m; d++) f[m][d + 1] += f[m - 1][d];
    }
    vector<T> ans(n + 1);
    for (int d = 0; d + shift <= n; d++) ans[d] = f[n][d + shift] * scale;
    return ans;
}
