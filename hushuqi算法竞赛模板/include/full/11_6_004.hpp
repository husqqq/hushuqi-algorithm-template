#pragma once

#include "11_6_001.hpp"

Real dis(P3 p, L3 l)
{
    return abs(cross(p - l.p, l.v)) / abs(l.v);
}

Real dis(P3 p, Plane s)
{
    return fabsl(dot(p - s.p, s.n)) / abs(s.n);
}

Real dis(L3 a, L3 b)
{
    P3 n = cross(a.v, b.v);
    if (sgn(abs(n) / abs(a.v) / abs(b.v)) == 0)
    {
        return dis(a.p, b);
    }
    return fabsl(dot(b.p - a.p, n)) / abs(n);
}

Real angle(P3 a, P3 b)
{
    return acosl(clamp(dot(a, b) / abs(a) / abs(b), Real(-1), Real(1)));
}
