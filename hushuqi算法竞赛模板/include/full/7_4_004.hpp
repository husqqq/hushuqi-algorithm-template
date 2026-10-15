#pragma once
#include "7_4_001.hpp"

optional<long long> sqrtMod(long long n, long long p)
{
    // n 是被开方数，p 是素数模；返回较小平方根，非二次剩余返回空。
    assert(p >= 2); // 调试检查，可删
    n %= p;
    if (n < 0)
    {
        n += p;
    }
    if (n == 0)
    {
        return 0;
    }
    if (p == 2)
    {
        return n;
    }
    if (powerMod(n, (p - 1) / 2, p) != 1)
    {
        return nullopt;
    }
    if (p % 4 == 3)
    {
        long long x = powerMod(n, (p + 1) / 4, p);
        return min(x, p - x);
    }
    long long q = p - 1;
    long long s = 0;
    while (!(q & 1))
    {
        q >>= 1;
        s++;
    }
    long long z = 2;
    while (powerMod(z, (p - 1) / 2, p) != p - 1)
    {
        z++;
    }
    long long c = powerMod(z, q, p);
    long long x = powerMod(n, (q + 1) / 2, p);
    long long t = powerMod(n, q, p);
    long long m = s;
    while (t != 1)
    {
        long long i = 1;
        long long y = mulMod(t, t, p);
        while (y != 1)
        {
            y = mulMod(y, y, p);
            i++;
        }
        long long b = powerMod(c, 1ULL << (m - i - 1), p);
        x = mulMod(x, b, p);
        c = mulMod(b, b, p);
        t = mulMod(t, c, p);
        m = i;
    }
    return min(x, p - x);
}
