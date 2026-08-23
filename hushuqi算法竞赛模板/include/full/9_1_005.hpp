#pragma once
#include <bits/stdc++.h>
#include "../linear_algebra_mod.hpp"
#include "../linear_algebra_field.hpp"
using namespace std;

inline long long invInt(long long a, long long mod)
{
    // 调试检查，可删。
    assert(mod > 1);
    a %= mod;
    if (a < 0)
    {
        a += mod;
    }
    long long b = mod;
    __int128 x = 1;
    __int128 y = 0;
    while (b)
    {
        long long q = a / b;
        a -= q * b;
        swap(a, b);
        x -= (__int128)q * y;
        swap(x, y);
    }
    // 调试检查，可删。
    assert(a == 1);
    x %= mod;
    if (x < 0)
    {
        x += mod;
    }
    return (long long)x;
}

inline optional<vector<vector<long long>>> matInvPP(vector<vector<long long>> a,
                                                             long long p, long long pk)
{
    int n = a.size();
    // 调试检查，可删。
    assert(p >= 2 && pk >= p);
    for (auto &r : a)
    {
        // 调试检查，可删。
        assert((int)r.size() == n);
        for (auto &x : r)
        {
            x %= pk;
            if (x < 0)
            {
                x += pk;
            }
        }
    }
    vector b(n, vector<long long>(n));
    for (int i = 0; i < n; i++)
    {
        b[i][i] = 1;
    }
    for (int col = 0; col < n; col++)
    {
        int row = col;
        while (row < n && a[row][col] % p == 0)
        {
            row++;
        }
        if (row == n)
        {
            return nullopt;
        }
        swap(a[row], a[col]);
        swap(b[row], b[col]);
        long long z = invInt(a[col][col], pk);
        for (int j = 0; j < n; j++)
        {
            a[col][j] = linearMulMod(a[col][j], z, pk);
            b[col][j] = linearMulMod(b[col][j], z, pk);
        }
        for (int i = 0; i < n; i++)
        {
            if (i == col)
            {
                continue;
            }
            long long q = a[i][col];
            for (int j = 0; j < n; j++)
            {
                a[i][j] = (a[i][j] - linearMulMod(q, a[col][j], pk)) % pk;
                b[i][j] = (b[i][j] - linearMulMod(q, b[col][j], pk)) % pk;
                if (a[i][j] < 0)
                {
                    a[i][j] += pk;
                }
                if (b[i][j] < 0)
                {
                    b[i][j] += pk;
                }
            }
        }
    }
    return b;
}

inline optional<vector<vector<long long>>> matInvAny(const vector<vector<long long>> &a,
                                                      long long mod)
{
    // 调试检查，可删。
    assert(mod > 1);
    vector<pair<long long, long long>> fac;
    long long x = mod;
    for (long long p = 2; p <= x / p; p++)
    {
        if (x % p)
        {
            continue;
        }
        long long pk = 1;
        while (x % p == 0)
        {
            x /= p;
            pk *= p;
        }
        fac.push_back({p, pk});
    }
    if (x > 1)
    {
        fac.push_back({x, x});
    }
    int n = a.size();
    long long cur = 1;
    vector ans(n, vector<long long>(n));
    for (auto [p, pk] : fac)
    {
        auto b = matInvPP(a, p, pk);
        if (!b)
        {
            return nullopt;
        }
        long long z = invInt(cur % pk, pk);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                long long t = ((*b)[i][j] - ans[i][j]) % pk;
                if (t < 0)
                {
                    t += pk;
                }
                t = linearMulMod(t, z, pk);
                // 已处理模数 cur 与当前 pk 的乘积不超过总模数，故 cur*t 不溢出。
                ans[i][j] += cur * t;
            }
        }
        cur *= pk;
    }
    return ans;
}
