#pragma once
#include "7_1_001.hpp"
#include "7_1_014.hpp"

optional<long long> bsgs(long long a, long long b, long long m,
                         long long ord = -1)
{
    // 在 [0,ord) 内求最小解；ord=-1 时使用通用上界 m，无解返回空。
    assert(m > 0); // 调试检查，可删
    if (m == 1)
    {
        return 0;
    }
    a %= m;
    b %= m;
    if (a < 0)
    {
        a += m;
    }
    if (b < 0)
    {
        b += m;
    }
    assert(binaryGcd(a, m) == 1); // 调试检查，可删
    if (ord == -1)
    {
        ord = m;
    }
    assert(ord > 0); // 调试检查，可删
    long long n = kthRoot((unsigned long long)ord, 2);
    if (n <= (ord - 1) / n && n * n < ord)
    {
        n++;
    }
    unordered_map<long long, long long> baby;
    long long x = 1;
    for (long long j = 0; j < n; j++)
    {
        if (!baby.contains(x))
        {
            baby[x] = j;
        }
        x = mulMod(x, a, m);
    }
    long long step = powerMod(*invMod(a, m), n, m);
    x = b;
    long long rounds = ord / n + (ord % n != 0);
    for (long long i = 0; i < rounds; i++)
    {
        if (auto it = baby.find(x); it != baby.end())
        {
            long long j = it->second;
            if (j < ord && i <= (ord - 1 - j) / n)
            {
                return i * n + j;
            }
        }
        x = mulMod(x, step, m);
    }
    return nullopt;
}

optional<long long> exbsgs(long long a, long long b, long long m)
{
    // 求 a^x=b (mod m) 的最小非负解，不要求 a 与 m 互素。
    assert(m > 0); // 调试检查，可删
    a %= m;
    b %= m;
    if (a < 0)
    {
        a += m;
    }
    if (b < 0)
    {
        b += m;
    }
    if (m == 1 || b == 1 % m)
    {
        return 0;
    }
    long long k = 1;
    long long z = 0;
    while (true)
    {
        long long g = (long long)binaryGcd(a, m);
        if (g == 1)
        {
            break;
        }
        if (b == k)
        {
            return z;
        }
        if (b % g)
        {
            return nullopt;
        }
        b /= g;
        m /= g;
        z++;
        k = mulMod(k, a / g, m);
    }
    if (m == 1)
    {
        return z;
    }
    auto x = bsgs(a, mulMod(b, *invMod(k, m), m), m);
    if (!x)
    {
        return nullopt;
    }
    return *x + z;
}
