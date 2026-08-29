#pragma once
#include "7_2_003.hpp"

unsigned long long pollard(unsigned long long n)
{
    // n 是大于 1 的合数；用 Brent 分块乘积返回一个非平凡因子。
    if (n % 2 == 0)
    {
        return 2;
    }
    static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    while (true)
    {
        auto c = rng() % (n - 1) + 1;
        auto y = rng() % (n - 1) + 1;
        auto f = [&](unsigned long long z)
        {
            return (unsigned long long)(((unsigned __int128)mul64(z, z, n) + c) % n);
        };
        unsigned long long x = 0, ys = 0, g = 1, q = 1, r = 1;
        constexpr unsigned long long m = 128;
        while (g == 1)
        {
            x = y;
            q = 1;
            for (unsigned long long i = 0; i < r; i++)
            {
                y = f(y);
            }
            for (unsigned long long k = 0; k < r && g == 1; k += m)
            {
                ys = y;
                auto lim = min(m, r - k);
                for (unsigned long long i = 0; i < lim; i++)
                {
                    y = f(y);
                    auto d = x > y ? x - y : y - x;
                    q = mul64(q, d, n);
                }
                g = gcd(q, n);
            }
            r <<= 1;
        }
        if (g == n)
        {
            do
            {
                ys = f(ys);
                auto d = x > ys ? x - ys : ys - x;
                g = gcd(d, n);
            } while (g == 1);
        }
        if (g != n)
        {
            return g;
        }
    }
}

vector<unsigned long long> factor64(unsigned long long n)
{
    // n 是正整数；返回升序质因子表并保留重数，n=1 返回空表。
    assert(n > 0); // 调试检查，可删
    vector<unsigned long long> ans;
    auto dfs = [&](auto &&self, unsigned long long x) -> void
    {
        if (x == 1)
        {
            return;
        }
        if (isPrime(x))
        {
            ans.push_back(x);
            return;
        }
        auto d = pollard(x);
        self(self, d);
        self(self, x / d);
    };
    dfs(dfs, n);
    sort(ans.begin(), ans.end());
    return ans;
}
