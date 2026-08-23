#pragma once
#include <bits/stdc++.h>
using namespace std;

inline optional<vector<long double>> minNormSolve(const vector<vector<long double>> &a,
                                                   const vector<long double> &b, int n)
{
    int m = a.size();
    int s = n + m;
    // 调试检查，可删。
    assert(n >= 0 && (int)b.size() == m);
    // 调试检查，可删。
    for (const auto &r : a)
    {
        assert((int)r.size() == n);
    }
    vector mat(s, vector<long double>(s + 1));
    for (int j = 0; j < n; j++)
    {
        mat[j][j] = 1;
        for (int i = 0; i < m; i++)
        {
            mat[j][n + i] = a[i][j];
        }
    }
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mat[n + i][j] = a[i][j];
        }
        mat[n + i][s] = b[i];
    }
    int rk = 0;
    vector<int> pivot(s, -1);
    for (int col = 0; col < s && rk < s; col++)
    {
        int row = rk;
        for (int i = rk + 1; i < s; i++)
        {
            if (fabsl(mat[i][col]) > fabsl(mat[row][col]))
            {
                row = i;
            }
        }
        if (fabsl(mat[row][col]) <= 1E-12L)
        {
            continue;
        }
        swap(mat[row], mat[rk]);
        for (int j = s; j >= col; j--)
        {
            mat[rk][j] /= mat[rk][col];
        }
        for (int i = 0; i < s; i++)
        {
            if (i == rk)
            {
                continue;
            }
            long double q = mat[i][col];
            for (int j = col; j <= s; j++)
            {
                mat[i][j] -= q * mat[rk][j];
            }
        }
        pivot[col] = rk++;
    }
    for (int i = rk; i < s; i++)
    {
        if (fabsl(mat[i][s]) > 1E-10L)
        {
            return nullopt;
        }
    }
    vector<long double> z(s);
    for (int col = 0; col < s; col++)
    {
        if (pivot[col] != -1)
        {
            z[col] = mat[pivot[col]][s];
        }
    }
    z.resize(n);
    return z;
}
