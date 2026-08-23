#pragma once
#include "7_2_003.hpp"

unsigned long long pollard(unsigned long long n)
{
    // n 是大于 1 的合数；返回 n 的一个非平凡因子。
    if (n % 2 == 0)
    {
        return 2;
    }
    static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    while (true)
    {
        auto c = rng() % (n - 1) + 1;
        auto x = rng() % n;
        auto y = x;
        auto d = 1ULL;
        auto f = [&](unsigned long long z)
        {
            return (unsigned long long)(((unsigned __int128)mul64(z, z, n) + c) % n);
        };
        while (d == 1)
        {
            x = f(x);
            y = f(f(y));
            d = gcd(x > y ? x - y : y - x, n);
        }
        if (d != n)
        {
            return d;
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
