#pragma once

#include "11_4_001.hpp"

P arcCentroid(C c, Real l, Real r)
{
    assert(sgn(c.r) >= 0); // 调试检查，可删。
    Real t = r - l;
    if (sgn(t) == 0)
    {
        return c.o + P{cosl(l), sinl(l)} * c.r;
    }
    Real z = 2 * c.r * sinl(t / 2) / t;
    return c.o + P{cosl((l + r) / 2), sinl((l + r) / 2)} * z;
}
