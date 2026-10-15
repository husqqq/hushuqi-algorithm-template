#pragma once

#include "11_1_002.hpp"

struct Hex
{
    int q; // q 是第一轴坐标。
    int r; // r 是第二轴坐标。

    Hex operator+(Hex b) const
    {
        // b 是另一组轴坐标；返回逐分量相加的轴坐标（平移 b）。
        return {q + b.q, r + b.r};
    }
    Hex operator-(Hex b) const
    {
        // b 是另一组轴坐标；返回逐分量相减的轴坐标，即从 b 到当前格的位移。
        return {q - b.q, r - b.r};
    }
};

int hexDist(Hex a, Hex b = {})
{
    // a、b 是轴坐标；返回六边形网格距离。
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
    // a 是轴坐标，size 是单元边长；返回平面显示坐标。
    return {size * sqrtl(3) * (a.q + a.r / 2.L), size * 1.5L * a.r};
}
