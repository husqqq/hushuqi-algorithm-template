#pragma once

#include "13_1_001.hpp"

array<long double, 3> triangle(long double a, long double b, long double c)
{
    assert(a > 0 && b > 0 && c > 0 && a + b > c && a + c > b && b + c > a);
    long double s = (a + b + c) / 2;
    long double area = sqrtl(s * (s - a) * (s - b) * (s - c));
    long double angle = acosl(clamp((a * a + b * b - c * c) / (2 * a * b),
                                    -1.0L, 1.0L));
    return {area, angle, a * b * c / (4 * area)};
}
