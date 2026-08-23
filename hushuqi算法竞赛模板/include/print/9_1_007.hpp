#pragma once
#include <bits/stdc++.h>
#include "support/linear_algebra_field.hpp"
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
