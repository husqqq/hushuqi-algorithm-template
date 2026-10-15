#pragma once
#include "11_1_001.hpp"
#include "11_6_001.hpp"

Real tetraVolume(P3 a, P3 b, P3 c, P3 d)
{
    // a、b、c、d 是四面体顶点；返回无符号体积。
    return fabsl(dot(b - a, cross(c - a, d - a))) / 6;
}

pair<P3, Real> tetraSphere(P3 a, P3 b, P3 c, P3 d)
{
    // a、b、c、d 是非退化四面体顶点；返回内心和内切球半径。
    Real x = abs(cross(b - a, c - a));
    Real y = abs(cross(b - a, d - a));
    Real z = abs(cross(c - a, d - a));
    Real w = abs(cross(c - b, d - b));
    Real s = x + y + z + w;
    assert(sgn(s) != 0 && sgn(tetraVolume(a, b, c, d)) != 0); // 调试检查，可删。
    P3 o = (d * x + c * y + b * z + a * w) / s;
    return {o, 3 * tetraVolume(a, b, c, d) / (s / 2)};
}
