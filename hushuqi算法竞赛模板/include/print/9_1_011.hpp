#pragma once
#include "9_1_005.hpp"
#include "9_1_006.hpp"

inline vector<vector<long long>> adjugateP(vector<vector<long long>> a, long long mod)
{
    // a 是方阵，mod 是素数；返回 A 的伴随矩阵。
    // 调试检查，可删。
    assert(mod > 1);
    int n = a.size();
    for (auto &r : a)
    {
        // 调试检查，可删。
        assert((int)r.size() == n);
        for (auto &x : r)
        {
            x %= mod;
            if (x < 0)
            {
                x += mod;
            }
        }
    }
    if (n == 0)
    {
        return {};
    }
    auto power = [&](long long x, long long b)
    {
        // x 是非零剩余，b 是非负指数；返回 x 的 b 次幂对 mod 的余数。
        long long ans = 1;
        while (b)
        {
            if (b & 1)
            {
                ans = linearMulMod(ans, x, mod);
            }
            x = linearMulMod(x, x, mod);
            b >>= 1;
        }
        return ans;
    };
    auto reduce = [&](vector<vector<long long>> m)
    {
        // m 是 n 阶方阵；返回秩、行最简形和每列对应的主元行，非主元列记为 -1。
        vector<int> pivot(n, -1);
        int rk = 0;
        for (int col = 0; col < n && rk < n; col++)
        {
            int row = rk;
            while (row < n && m[row][col] == 0)
            {
                row++;
            }
            if (row == n)
            {
                continue;
            }
            swap(m[row], m[rk]);
            long long z = power(m[rk][col], mod - 2);
            for (int j = 0; j < n; j++)
            {
                m[rk][j] = linearMulMod(m[rk][j], z, mod);
            }
            for (int i = 0; i < n; i++)
            {
                if (i == rk)
                {
                    continue;
                }
                long long q = m[i][col];
                for (int j = 0; j < n; j++)
                {
                    m[i][j] = (m[i][j] - linearMulMod(q, m[rk][j], mod)) % mod;
                    if (m[i][j] < 0)
                    {
                        m[i][j] += mod;
                    }
                }
            }
            pivot[col] = rk++;
        }
        return tuple<int, vector<vector<long long>>, vector<int>>(rk, move(m), move(pivot));
    };
    auto [rk, r, pivot] = reduce(a);
    vector ans(n, vector<long long>(n));
    if (rk == n)
    {
        auto inv = matInvPP(a, mod, mod);
        long long det = detPrime(a, mod);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                ans[i][j] = linearMulMod(det, (*inv)[i][j], mod);
            }
        }
        return ans;
    }
    if (rk <= n - 2)
    {
        return ans;
    }
    int fx = find(pivot.begin(), pivot.end(), -1) - pivot.begin();
    vector<long long> x(n);
    x[fx] = 1;
    for (int col = 0; col < n; col++)
    {
        if (pivot[col] != -1)
        {
            x[col] = (mod - r[pivot[col]][fx]) % mod;
        }
    }
    vector at(n, vector<long long>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            at[i][j] = a[j][i];
        }
    }
    auto [rk2, rt, pivot2] = reduce(at);
    (void)rk2;
    int fy = find(pivot2.begin(), pivot2.end(), -1) - pivot2.begin();
    vector<long long> y(n);
    y[fy] = 1;
    for (int col = 0; col < n; col++)
    {
        if (pivot2[col] != -1)
        {
            y[col] = (mod - rt[pivot2[col]][fy]) % mod;
        }
    }
    int i0 = 0, j0 = 0;
    while (x[i0] == 0)
    {
        i0++;
    }
    while (y[j0] == 0)
    {
        j0++;
    }
    vector<vector<long long>> minor;
    for (int i = 0; i < n; i++)
    {
        if (i == j0)
        {
            continue;
        }
        vector<long long> row;
        for (int j = 0; j < n; j++)
        {
            if (j != i0)
            {
                row.push_back(a[i][j]);
            }
        }
        minor.push_back(move(row));
    }
    long long cof = detPrime(minor, mod);
    if ((i0 + j0) & 1 && cof)
    {
        cof = mod - cof;
    }
    long long scale = linearMulMod(cof, power(x[i0], mod - 2), mod);
    scale = linearMulMod(scale, power(y[j0], mod - 2), mod);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            ans[i][j] = linearMulMod(linearMulMod(scale, x[i], mod), y[j], mod);
        }
    }
    return ans;
}
