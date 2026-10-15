#pragma once

#include "11_1_002.hpp"

struct Hex
{
    int q;
    int r;

    Hex operator+(Hex b) const
    {
        return {q + b.q, r + b.r};
    }

    Hex operator-(Hex b) const
    {
        return {q - b.q, r - b.r};
    }
};

int hexDist(Hex a, Hex b = {})
{
    __int128 q = (__int128)a.q - b.q;
    __int128 r = (__int128)a.r - b.r;
    auto ab = [](__int128 x)
    {
        return x < 0 ? -x : x;
    };
    __int128 d = (ab(q) + ab(r) + ab(q + r)) / 2;
    assert(d <= LLONG_MAX); // 调试检查，可删。
    return (int)d;
}

P hexPoint(Hex a, Real size = 1)
{
    return {size * sqrtl(3) * (a.q + a.r / 2.L), size * 1.5L * a.r};
}
