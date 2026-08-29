#pragma once
#include <bits/stdc++.h>
#include "7_1_001.hpp"
#include "support/linear_algebra_field.hpp"
using namespace std;

struct ModLinearSol
{
    vector<long long> part;
    vector<vector<long long>> basis;
};

inline int rankMod(vector<vector<long long>> a, int m, long long mod)
{
    // a 是行宽为 m 的矩阵，mod 是素数；返回有限域上的矩阵秩。
    assert(m >= 0 && mod > 1);
    int n = a.size(), rk = 0;
    for (auto &r : a)
    {
        assert((int)r.size() == m);
        for (auto &x : r) x = (x % mod + mod) % mod;
    }
    auto power = [&](long long x, long long b)
    {
        long long ans = 1;
        while (b)
        {
            if (b & 1) ans = mulMod(ans, x, mod);
            x = mulMod(x, x, mod);
            b >>= 1;
        }
        return ans;
    };
    for (int col = 0; col < m && rk < n; col++)
    {
        int p = rk;
        while (p < n && a[p][col] == 0) p++;
        if (p == n) continue;
        swap(a[p], a[rk]);
        long long inv = power(a[rk][col], mod - 2);
        for (int i = rk + 1; i < n; i++) if (a[i][col])
        {
            long long q = mulMod(a[i][col], inv, mod);
            for (int j = col; j < m; j++)
            {
                a[i][j] -= mulMod(q, a[rk][j], mod);
                if (a[i][j] < 0) a[i][j] += mod;
            }
        }
        rk++;
    }
    return rk;
}

inline optional<ModLinearSol> gaussMod(vector<vector<long long>> a, int n, long long mod)
{
    // 调试检查，可删。
    assert(n >= 0 && mod > 1);
    for (auto &r : a)
    {
        // 调试检查，可删。
        assert((int)r.size() == n + 1);
        for (auto &x : r)
        {
            x %= mod;
            if (x < 0)
            {
                x += mod;
            }
        }
    }
    int m = a.size();
    int rk = 0;
    vector<int> pivot(n, -1);
    auto power = [&](long long x, long long b)
    {
        long long ans = 1;
        while (b)
        {
            if (b & 1)
            {
                ans = mulMod(ans, x, mod);
            }
            x = mulMod(x, x, mod);
            b >>= 1;
        }
        return ans;
    };
    for (int col = 0; col < n && rk < m; col++)
    {
        int row = rk;
        while (row < m && a[row][col] == 0)
        {
            row++;
        }
        if (row == m)
        {
            continue;
        }
        swap(a[row], a[rk]);
        long long inv = power(a[rk][col], mod - 2);
        for (int j = col; j <= n; j++)
        {
            a[rk][j] = mulMod(a[rk][j], inv, mod);
        }
        for (int i = 0; i < m; i++)
        {
            if (i == rk)
            {
                continue;
            }
            long long q = a[i][col];
            for (int j = col; j <= n; j++)
            {
                a[i][j] -= mulMod(q, a[rk][j], mod);
                if (a[i][j] < 0)
                {
                    a[i][j] += mod;
                }
            }
        }
        pivot[col] = rk++;
    }
    for (int i = rk; i < m; i++)
    {
        if (a[i][n])
        {
            return nullopt;
        }
    }
    ModLinearSol sol{vector<long long>(n), {}};
    for (int col = 0; col < n; col++)
    {
        if (pivot[col] != -1)
        {
            sol.part[col] = a[pivot[col]][n];
        }
    }
    for (int free = 0; free < n; free++)
    {
        if (pivot[free] != -1)
        {
            continue;
        }
        vector<long long> v(n);
        v[free] = 1;
        for (int col = 0; col < n; col++)
        {
            if (pivot[col] != -1)
            {
                v[col] = (mod - a[pivot[col]][free]) % mod;
            }
        }
        sol.basis.push_back(move(v));
    }
    return sol;
}
