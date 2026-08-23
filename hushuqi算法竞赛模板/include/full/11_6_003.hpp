#pragma once

#include "11_6_001.hpp"

optional<P3> linePlane(L3 l, Plane s)
{
    assert(sgn(abs(l.v)) != 0 && sgn(abs(s.n)) != 0); // 调试检查，可删。
    Real d = dot(l.v, s.n);
    if (sgn(d / abs(l.v) / abs(s.n)) == 0)
    {
        return nullopt;
    }
    return l.p + l.v * (dot(s.p - l.p, s.n) / d);
}

optional<L3> planePlane(Plane a, Plane b)
{
    assert(sgn(abs(a.n)) != 0 && sgn(abs(b.n)) != 0); // 调试检查，可删。
    P3 v = cross(a.n, b.n);
    Real d = norm(v);
    if (sgn(d / norm(a.n) / norm(b.n)) == 0)
    {
        return nullopt;
    }
    Real x = dot(a.n, a.p);
    Real y = dot(b.n, b.p);
    P3 p = cross(b.n * x - a.n * y, v) / d;
    return L3{p, v};
}
