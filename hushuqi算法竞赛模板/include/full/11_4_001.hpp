#pragma once

#include "11_1_003.hpp"

struct C
{
    P o;
    Real r;
};

bool in(C c, P p)
{
    assert(sgn(c.r) >= 0); // 调试检查，可删。
    return sgn(abs(p - c.o) - c.r) <= 0;
}

int circleRel(C a, C b)
{
    assert(sgn(a.r) >= 0 && sgn(b.r) >= 0); // 调试检查，可删。
    Real d = abs(a.o - b.o);
    if (sgn(d) == 0 && sgn(a.r - b.r) == 0)
    {
        return 5;
    }
    int x = sgn(d - a.r - b.r);
    int y = sgn(d - fabsl(a.r - b.r));
    if (x > 0)
    {
        return 0;
    }
    if (x == 0)
    {
        return 1;
    }
    if (y > 0)
    {
        return 2;
    }
    if (y == 0)
    {
        return 3;
    }
    return 4;
}
