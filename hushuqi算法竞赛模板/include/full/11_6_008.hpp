#pragma once

#include "11_6_001.hpp"

Real tetraVolume(P3 a, P3 b, P3 c, P3 d)
{
    return fabsl(dot(b - a, cross(c - a, d - a))) / 6;
}

pair<P3, Real> tetraSphere(P3 a, P3 b, P3 c, P3 d)
{
    Real x = abs(cross(b - a, c - a));
    Real y = abs(cross(b - a, d - a));
    Real z = abs(cross(c - a, d - a));
    Real w = abs(cross(c - b, d - b));
    Real s = x + y + z + w;
    P3 o = (d * x + c * y + b * z + a * w) / s;
    return {o, 3 * tetraVolume(a, b, c, d) / (s / 2)};
}
