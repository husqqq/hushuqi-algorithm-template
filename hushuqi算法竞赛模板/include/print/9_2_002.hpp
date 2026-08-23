#pragma once
#include <bits/stdc++.h>
using namespace std;

inline optional<vector<long double>> gaussPivot(vector<vector<long double>> a, int n)
{
    // 调试检查，可删。
    assert(n >= 0);
    for (const auto &r : a)
    {
        // 调试检查，可删。
        assert((int)r.size() == n + 1);
    }
    constexpr long double eps = 1E-12L;
    int m = a.size();
    int rk = 0;
    vector<int> pivot(n, -1);
    for (int col = 0; col < n && rk < m; col++)
    {
        int row = rk;
        for (int i = rk + 1; i < m; i++)
        {
            if (fabsl(a[i][col]) > fabsl(a[row][col]))
            {
                row = i;
            }
        }
        if (fabsl(a[row][col]) <= eps)
        {
            continue;
        }
        swap(a[row], a[rk]);
        for (int j = n; j >= col; j--)
        {
            a[rk][j] /= a[rk][col];
        }
        for (int i = 0; i < m; i++)
        {
            if (i == rk)
            {
                continue;
            }
            long double q = a[i][col];
            for (int j = col; j <= n; j++)
            {
                a[i][j] -= q * a[rk][j];
            }
        }
        pivot[col] = rk++;
    }
    for (int i = rk; i < m; i++)
    {
        if (fabsl(a[i][n]) > eps)
        {
            return nullopt;
        }
    }
    if (rk < n)
    {
        return nullopt;
    }
    vector<long double> x(n);
    for (int col = 0; col < n; col++)
    {
        x[col] = a[pivot[col]][n];
    }
    return x;
}
