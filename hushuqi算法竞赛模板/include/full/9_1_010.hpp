#pragma once
#include <bits/stdc++.h>
using namespace std;

namespace woodbury
{
using Mat = vector<vector<long double>>;

inline Mat mul(const Mat &a, const Mat &b)
{
    // a、b 是维数相容矩阵；返回矩阵乘积。
    if (a.empty())
    {
        return {};
    }
    if (b.empty())
    {
        for (const auto &r : a)
        {
            assert(r.empty()); // 调试检查，可删。
        }
        return Mat(a.size());
    }
    int n = a.size();
    int k = b.size();
    int m = b[0].size();
    // 调试检查，可删。
    for (const auto &r : b)
    {
        assert((int)r.size() == m);
    }
    Mat c(n, vector<long double>(m));
    for (int i = 0; i < n; i++)
    {
        // 调试检查，可删。
        assert((int)a[i].size() == k);
        for (int p = 0; p < k; p++)
        {
            for (int j = 0; j < m; j++)
            {
                c[i][j] += a[i][p] * b[p][j];
            }
        }
    }
    return c;
}

inline Mat inv(Mat a)
{
    // a 是可逆方阵；返回高斯消元求得的逆矩阵。
    int n = a.size();
    Mat b(n, vector<long double>(2 * n));
    for (int i = 0; i < n; i++)
    {
        // 调试检查，可删。
        assert((int)a[i].size() == n);
        copy(a[i].begin(), a[i].end(), b[i].begin());
        b[i][n + i] = 1;
    }
    for (int col = 0; col < n; col++)
    {
        int row = col;
        for (int i = col + 1; i < n; i++)
        {
            if (fabsl(b[i][col]) > fabsl(b[row][col]))
            {
                row = i;
            }
        }
        // 调试检查，可删。
        assert(fabsl(b[row][col]) > 1E-18L);
        swap(b[row], b[col]);
        long double x = b[col][col];
        for (auto &v : b[col])
        {
            v /= x;
        }
        for (int i = 0; i < n; i++)
        {
            if (i == col)
            {
                continue;
            }
            x = b[i][col];
            for (int j = 0; j < 2 * n; j++)
            {
                b[i][j] -= x * b[col][j];
            }
        }
    }
    Mat ans(n, vector<long double>(n));
    for (int i = 0; i < n; i++)
    {
        copy(b[i].begin() + n, b[i].end(), ans[i].begin());
    }
    return ans;
}

inline Mat woodbury(const Mat &ai, const Mat &u, const Mat &ci, const Mat &v)
{
    // ai、ci 分别是 A 与 C 的逆，u、v 是低秩因子；返回 (A+UCV) 的逆。
    int n = ai.size();
    int k = ci.size();
    // 调试检查，可删。
    assert((int)u.size() == n && (int)v.size() == k);
    for (const auto &r : ai)
    {
        // 调试检查，可删。
        assert((int)r.size() == n);
    }
    for (const auto &r : u)
    {
        // 调试检查，可删。
        assert((int)r.size() == k);
    }
    for (const auto &r : ci)
    {
        // 调试检查，可删。
        assert((int)r.size() == k);
    }
    for (const auto &r : v)
    {
        // 调试检查，可删。
        assert((int)r.size() == n);
    }
    if (n == 0)
    {
        return {};
    }
    if (k == 0)
    {
        return ai;
    }
    Mat left = mul(ai, u);
    Mat right = mul(v, ai);
    Mat mid = ci;
    Mat vaiu = mul(right, u);
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            mid[i][j] += vaiu[i][j];
        }
    }
    Mat sub = mul(mul(left, inv(mid)), right);
    Mat ans = ai;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            ans[i][j] -= sub[i][j];
        }
    }
    return ans;
}
}
