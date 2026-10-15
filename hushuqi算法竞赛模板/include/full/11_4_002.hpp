#pragma once

#include "11_1_008.hpp"
#include "11_4_001.hpp"

vector<P> lineCircle(L l, C c)
{
    assert(sgn(norm(l.v)) != 0 && sgn(c.r) >= 0); // 调试检查，可删。
    P h = proj(l, c.o);
    Real d = norm(h - c.o);
    if (sgn(d - c.r * c.r) > 0)
    {
        return {};
    }
    Real t = sqrtl(max(Real(0), c.r * c.r - d)) / abs(l.v);
    if (sgn(t) == 0)
    {
        return {h};
    }
    return {h - l.v * t, h + l.v * t};
}

vector<P> circleIsect(C a, C b)
{
    assert(sgn(a.r) >= 0 && sgn(b.r) >= 0); // 调试检查，可删。
    Real d = abs(b.o - a.o);
    if (sgn(d) == 0)
    {
        if (sgn(a.r) == 0 && sgn(b.r) == 0)
        {
            return {a.o};
        }
        return {};
    }
    if (sgn(d - a.r - b.r) > 0 || sgn(d - fabsl(a.r - b.r)) < 0)
    {
        return {};
    }
    Real x = (a.r * a.r - b.r * b.r + d * d) / (2 * d);
    Real h = sqrtl(max(Real(0), a.r * a.r - x * x));
    P v = (b.o - a.o) / d;
    P p = a.o + v * x;
    if (sgn(h) == 0)
    {
        return {p};
    }
    return {p + rot(v) * h, p - rot(v) * h};
}
