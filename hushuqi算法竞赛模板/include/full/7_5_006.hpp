#pragma once
#include "7_1_014.hpp"

optional<pair<long long, long long>> pell(long long d)
{
    // d 是正整数；返回 x^2-d*y^2=1 的基本解，d 为平方数或解越过 long long 时返回空。
    assert(d > 0); // 调试检查，可删
    long long a0 = kthRoot((unsigned long long)d, 2);
    if ((__int128)a0 * a0 == d)
    {
        return nullopt;
    }
    __int128 D = d;
    __int128 m = 0;
    __int128 q = 1;
    __int128 a = a0;
    __int128 p0 = 1;
    __int128 p1 = a;
    __int128 q0 = 0;
    __int128 q1 = 1;
    constexpr unsigned __int128 IMAX = ((unsigned __int128)1 << 127) - 1;
    while (true)
    {
        auto uq1 = (unsigned __int128)q1;
        if (q1 && uq1 > IMAX / (unsigned __int128)D / uq1)
        {
            return nullopt;
        }
        if (p1 * p1 - D * q1 * q1 == 1)
        {
            break;
        }
        m = q * a - m;
        q = (D - m * m) / q;
        a = (a0 + m) / q;
        __int128 p2 = a * p1 + p0;
        __int128 q2 = a * q1 + q0;
        if (p2 > numeric_limits<long long>::max() ||
            q2 > numeric_limits<long long>::max())
        {
            return nullopt;
        }
        p0 = p1;
        p1 = p2;
        q0 = q1;
        q1 = q2;
    }
    return pair{(long long)p1, (long long)q1};
}
