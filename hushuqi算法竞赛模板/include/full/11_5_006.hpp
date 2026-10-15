#pragma once

#include "11_1_001.hpp"

Real sphereDist(Real lat1, Real lon1, Real lat2, Real lon2, Real r = 1)
{
    assert(sgn(r) >= 0); // 调试检查，可删。
    Real x = sinl((lat2 - lat1) / 2);
    Real y = sinl((lon2 - lon1) / 2);
    Real h = x * x + cosl(lat1) * cosl(lat2) * y * y;
    return 2 * r * asinl(sqrtl(clamp(h, Real(0), Real(1))));
}
