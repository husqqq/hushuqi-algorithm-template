#pragma once
#include <bits/stdc++.h>
#include "../linear_algebra_mod.hpp"
#include "../linear_algebra_field.hpp"
using namespace std;

inline long long detPrime(vector<vector<long long>> a, long long mod)
{
    // 调试检查，可删。
    assert(mod > 1);
    int n = a.size();
    long long ans = 1;
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
    for (int col = 0; col < n; col++)
    {
        int row = col;
        while (row < n && a[row][col] == 0)
        {
            row++;
        }
        if (row == n)
        {
            return 0;
        }
        if (row != col)
        {
            swap(a[row], a[col]);
            ans = (mod - ans) % mod;
        }
        ans = linearMulMod(ans, a[col][col], mod);
        long long z = 1;
        long long b = mod - 2;
        long long x = a[col][col];
        while (b)
        {
            if (b & 1)
            {
                z = linearMulMod(z, x, mod);
            }
            x = linearMulMod(x, x, mod);
            b >>= 1;
        }
        for (int i = col + 1; i < n; i++)
        {
            long long q = linearMulMod(a[i][col], z, mod);
            for (int j = col; j < n; j++)
            {
                a[i][j] = (a[i][j] - linearMulMod(q, a[col][j], mod)) % mod;
                if (a[i][j] < 0)
                {
                    a[i][j] += mod;
                }
            }
        }
    }
    return ans;
}

inline long long detAny(vector<vector<long long>> a, long long mod)
{
    // 调试检查，可删。
    assert(mod > 1);
    int n = a.size();
    long long ans = 1;
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
    for (int col = 0; col < n; col++)
    {
        for (int i = col + 1; i < n; i++)
        {
            while (a[i][col])
            {
                long long q = a[col][col] / a[i][col];
                for (int j = col; j < n; j++)
                {
                    a[col][j] = (a[col][j] - linearMulMod(q, a[i][j], mod)) % mod;
                    if (a[col][j] < 0)
                    {
                        a[col][j] += mod;
                    }
                }
                swap(a[col], a[i]);
                ans = (mod - ans) % mod;
            }
        }
        if (a[col][col] == 0)
        {
            return 0;
        }
        ans = linearMulMod(ans, a[col][col], mod);
    }
    return ans;
}

template <uint32_t P> uint32_t detPrimeFast(const vector<vector<long long>> &src)
{
    static_assert(1 < P && P < (1U << 30));
    uint32_t n = src.size();
    vector<unsigned long long> a((size_t)n * n);
    vector<uint32_t> cnt(n);
    for (uint32_t i = 0; i < n; i++)
    {
        assert(src[i].size() == n); // 调试检查，可删
        for (uint32_t j = 0; j < n; j++)
        {
            long long x = src[i][j] % (long long)P;
            if (x < 0)
            {
                x += P;
            }
            a[(size_t)i * n + j] = x;
        }
    }
    auto power = [](unsigned long long x, uint32_t b)
    {
        unsigned long long ans = 1;
        for (; b; b >>= 1, x = x * x % P)
        {
            if (b & 1)
            {
                ans = ans * x % P;
            }
        }
        return (uint32_t)ans;
    };
    bool neg = false;
    for (uint32_t i = 0; i < n; i++)
    {
        for (uint32_t j = i; j < n; j++)
        {
            a[(size_t)j * n + i] %= P;
        }
        uint32_t p = i;
        while (p < n && !a[(size_t)p * n + i])
        {
            p++;
        }
        if (p == n)
        {
            return 0;
        }
        if (p != i)
        {
            for (uint32_t j = i; j < n; j++)
            {
                swap(a[(size_t)i * n + j], a[(size_t)p * n + j]);
            }
            swap(cnt[i], cnt[p]);
            neg = !neg;
        }
        auto row = a.data() + (size_t)i * n;
        for (uint32_t j = i; j < n; j++)
        {
            row[j] %= P;
        }
        uint32_t iv = power(row[i], P - 2);
        for (uint32_t k = i + 1; k < n; k++)
        {
            auto cur = a.data() + (size_t)k * n;
            uint32_t q = cur[i] ? P - (unsigned long long)iv * cur[i] % P : 0;
            uint32_t j = i;
            for (; j + 4 <= n; j += 4)
            {
                cur[j] += (unsigned long long)q * row[j];
                cur[j + 1] += (unsigned long long)q * row[j + 1];
                cur[j + 2] += (unsigned long long)q * row[j + 2];
                cur[j + 3] += (unsigned long long)q * row[j + 3];
            }
            for (; j < n; j++)
            {
                cur[j] += (unsigned long long)q * row[j];
            }
            if (++cnt[k] == 8)
            {
                cnt[k] = 0;
                for (uint32_t j = i; j < n; j++)
                {
                    cur[j] %= P;
                }
            }
        }
    }
    unsigned long long ans = neg ? P - 1 : 1;
    for (uint32_t i = 0; i < n; i++)
    {
        ans = ans * (a[(size_t)i * n + i] % P) % P;
    }
    return ans;
}
