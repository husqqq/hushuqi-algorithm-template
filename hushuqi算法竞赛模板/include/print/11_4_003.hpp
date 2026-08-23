#pragma once

#include "11_4_001.hpp"

Real circleArea(C a, C b)
{
    assert(sgn(a.r) >= 0 && sgn(b.r) >= 0); // 调试检查，可删。
    Real d = abs(a.o - b.o);
    if (sgn(d - a.r - b.r) >= 0)
    {
        return 0;
    }
    if (sgn(d - fabsl(a.r - b.r)) <= 0)
    {
        Real r = min(a.r, b.r);
        return acosl(-1) * r * r;
    }
    Real x = acosl(clamp((a.r * a.r + d * d - b.r * b.r)
                         / (2 * a.r * d), Real(-1), Real(1)));
    Real y = acosl(clamp((b.r * b.r + d * d - a.r * a.r)
                         / (2 * b.r * d), Real(-1), Real(1)));
    return a.r * a.r * x + b.r * b.r * y - a.r * d * sinl(x);
}
