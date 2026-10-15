#pragma once

#include "11_1_003.hpp"

array<P, 6> triCenters(P a, P b, P c)
{
    auto x = abs(b - c);
    auto y = abs(c - a);
    auto z = abs(a - b);
    auto in = (a * x + b * y + c * z) / (x + y + z);
    auto d = 2 * cross(b - a, c - a);
    assert(sgn(d) != 0); // 调试检查，可删。
    auto bb = square(b - a);
    auto cc = square(c - a);
    P out{a.x + ((c.y - a.y) * bb - (b.y - a.y) * cc) / d,
          a.y + ((b.x - a.x) * cc - (c.x - a.x) * bb) / d};
    auto orth = a + b + c - out * 2;
    auto exa = (-a * x + b * y + c * z) / (-x + y + z);
    auto exb = (a * x - b * y + c * z) / (x - y + z);
    auto exc = (a * x + b * y - c * z) / (x + y - z);
    return {in, out, orth, exa, exb, exc};
}
