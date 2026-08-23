#pragma once
#include "7_1_001.hpp"
#include "7_2_004.hpp"

long long primRoot(long long p)
{
    // p 是素数；返回模 p 的最小正原根，p=2 时返回 1。
    assert(p >= 2); // 调试检查，可删
    if (p == 2)
    {
        return 1;
    }
    long long n = p - 1;
    auto raw = factor64(n);
    raw.erase(unique(raw.begin(), raw.end()), raw.end());
    for (long long g = 2;; g++)
    {
        bool ok = true;
        for (auto d : raw)
        {
            if (powerMod(g, n / d, p) == 1)
            {
                ok = false;
            }
        }
        if (ok)
        {
            return g;
        }
    }
}

vector<long long> primRoots(long long p)
{
    // p 是素数；返回模 p 的全部原根并按数值升序排列。
    long long g = primRoot(p);
    long long n = p - 1;
    vector<long long> a;
    for (long long k = 1; k <= n; k++)
    {
        if (gcd(k, n) == 1)
        {
            a.push_back(powerMod(g, k, p));
        }
    }
    sort(a.begin(), a.end());
    return a;
}
