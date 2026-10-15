#pragma once

#include "11_6_001.hpp"

Real sphereVolume(Real r, int n = 3)
{
    assert(sgn(r) >= 0 && n >= 0); // 调试检查，可删。
    if (n == 0)
    {
        return 1;
    }
    if (sgn(r) == 0)
    {
        return 0;
    }
    Real x = Real(n) / 2 * logl(acosl(-1)) + n * logl(r)
        - lgammal(Real(n) / 2 + 1);
    return expl(x);
}
