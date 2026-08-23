#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Simplex
{
    // m 是约束数，n 是变量数。
    int m, n;
    // b、z 分别记录各行的基变量和各列的非基变量编号。
    vector<int> b, z;
    // a 是单纯形表。
    vector<vector<long double>> a;
    // e 是浮点比较容差，linf 标记无界或不可行。
    static constexpr long double e = 1E-12L;
    static constexpr long double linf = numeric_limits<long double>::infinity();

    Simplex(const vector<vector<long double>> &A, const vector<long double> &B,
            const vector<long double> &C)
        : m(B.size()), n(C.size()), b(m), z(n + 1), a(m + 2, vector<long double>(n + 2))
    {
        // A、B 给出 Ax<=B，C 是最大化目标系数；隐含 x>=0。
        // 调试检查，可删。
        assert((int)A.size() == m);
        for (int i = 0; i < m; i++)
        {
            // 调试检查，可删。
            assert((int)A[i].size() == n);
            for (int j = 0; j < n; j++)
            {
                a[i][j] = A[i][j];
            }
            b[i] = n + i;
            a[i][n] = -1;
            a[i][n + 1] = B[i];
        }
        for (int j = 0; j < n; j++)
        {
            z[j] = j;
            a[m][j] = -C[j];
        }
        z[n] = -1;
        a[m + 1][n] = 1;
    }

private:
    void pivot(int r, int s)
    {
        // r 是离基变量所在行，s 是入基变量所在列；用 a[r][s] 换基并更新整张表。
        long double v = 1 / a[r][s];
        for (int i = 0; i < m + 2; i++)
        {
            if (i != r)
            {
                for (int j = 0; j < n + 2; j++)
                {
                    if (j != s)
                    {
                        a[i][j] -= a[r][j] * a[i][s] * v;
                    }
                }
            }
        }
        for (int j = 0; j < n + 2; j++)
        {
            if (j != s)
            {
                a[r][j] *= v;
            }
        }
        for (int i = 0; i < m + 2; i++)
        {
            if (i != r)
            {
                a[i][s] *= -v;
            }
        }
        a[r][s] = v;
        swap(b[r], z[s]);
    }

    bool phase(int p)
    {
        // p=1 时找可行基，p=2 时优化原目标；返回 false 表示存在无界改进方向。
        int x = p == 1 ? m + 1 : m;
        while (true)
        {
            int s = -1;
            for (int j = 0; j <= n; j++)
            {
                if (z[j] == -1 || a[x][j] >= -e)
                {
                    continue;
                }
                if (s < 0 || z[j] < z[s])
                {
                    s = j;
                }
            }
            if (s < 0)
            {
                return true;
            }
            int r = -1;
            for (int i = 0; i < m; i++)
            {
                if (a[i][s] <= e)
                {
                    continue;
                }
                if (r < 0 || a[i][n + 1] / a[i][s] < a[r][n + 1] / a[r][s] - e ||
                    (fabsl(a[i][n + 1] / a[i][s] - a[r][n + 1] / a[r][s]) <= e && b[i] < b[r]))
                {
                    r = i;
                }
            }
            if (r < 0)
            {
                return false;
            }
            pivot(r, s);
        }
    }

public:
    pair<long double, vector<long double>> solve()
    {
        // 返回最优值与变量；不可行返回 -linf，无界返回 linf，二者的解向量均为空。
        if (m == 0)
        {
            for (int j = 0; j < n; j++)
            {
                if (a[m][j] < -e)
                {
                    return {linf, {}};
                }
            }
            return {0, vector<long double>(n)};
        }
        int r = min_element(a.begin(), a.begin() + m,
                            [&](const auto &x, const auto &y)
                            {
                                return x[n + 1] < y[n + 1];
                            }) -
                a.begin();
        if (a[r][n + 1] < -e)
        {
            pivot(r, n);
            if (!phase(1) || a[m + 1][n + 1] < -e || fabsl(a[m + 1][n + 1]) > e)
            {
                return {-linf, {}};
            }
            auto it = find(b.begin(), b.end(), -1);
            if (it != b.end())
            {
                r = it - b.begin();
                int s = 0;
                for (int j = 1; j <= n; j++)
                {
                    if (fabsl(a[r][j]) > fabsl(a[r][s]))
                    {
                        s = j;
                    }
                }
                if (fabsl(a[r][s]) > e)
                {
                    pivot(r, s);
                }
            }
        }
        if (!phase(2))
        {
            return {linf, {}};
        }
        vector<long double> x(n);
        for (int i = 0; i < m; i++)
        {
            if (b[i] < n)
            {
                x[b[i]] = a[i][n + 1];
            }
        }
        return {a[m][n + 1], x};
    }
};
