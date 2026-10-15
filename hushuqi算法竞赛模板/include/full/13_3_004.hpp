#pragma once

#include "13_1_001.hpp"

array<long double, 4> roundBodies(long double r, long double h, long double r2)
{
    // r 是圆/球半径及圆台第一底半径，h 是圆台高，r2 是另一底半径；返回 {圆面积, 球面积, 球体积, 圆台体积}。
    assert(r >= 0 && h >= 0 && r2 >= 0);
    long double pi = acosl(-1.0L);
    return {pi * r * r, 4 * pi * r * r,
            4 * pi * r * r * r / 3,
            pi * h * (r * r + r * r2 + r2 * r2) / 3};
}
