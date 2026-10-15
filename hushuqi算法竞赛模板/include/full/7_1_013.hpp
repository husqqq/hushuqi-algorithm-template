#pragma once
#include "7_1_001.hpp"

optional<pair<long long, long long>> ratRebuild(long long x, long long m,
                                                         long long a, long long b)
{
    // x 是模 m 的剩余，a、b 是分子绝对值与分母上界；在 2ab<m 时返回唯一既约真分数。
    assert(m > 0 && a >= 0 && b > 0); // 调试检查，可删
    assert((__int128)2 * a * b < m); // 调试检查，可删
    x %= m;
    if (x < 0)
    {
        x += m;
    }
    __int128 r0 = m;
    __int128 r1 = x;
    __int128 t0 = 0;
    __int128 t1 = 1;
    auto mag = [](__int128 v)
    {
        return v < 0 ? -v : v;
    };
    while (mag(r1) > a)
    {
        if (!r1)
        {
            return nullopt;
        }
        __int128 q = r0 / r1;
        __int128 nr = r0 - q * r1;
        __int128 nt = t0 - q * t1;
        r0 = r1;
        r1 = nr;
        t0 = t1;
        t1 = nt;
    }
    if (!t1 || mag(t1) > b)
    {
        return nullopt;
    }
    if (t1 < 0)
    {
        r1 = -r1;
        t1 = -t1;
    }
    if (mag(r1) >= t1 || gcd((long long)mag(r1), (long long)t1) != 1)
    {
        return nullopt;
    }
    if (((__int128)x * t1 - r1) % m)
    {
        return nullopt;
    }
    return pair{(long long)r1, (long long)t1};
}
