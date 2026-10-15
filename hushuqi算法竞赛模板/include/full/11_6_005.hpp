#pragma once

#include "11_1_009.hpp"
#include "11_6_001.hpp"

bool segTri(P3 p, P3 q, P3 a, P3 b, P3 c)
{
    P3 v = q - p;
    P3 e1 = b - a;
    P3 e2 = c - a;
    P3 n = cross(e1, e2);
    assert(sgn(abs(n)) != 0); // 调试检查，可删。
    P3 h = cross(v, e2);
    Real d = dot(e1, h);
    Real scale = abs(v) * abs(e1) * abs(e2);
    if (fabsl(d) <= eps * max(Real(1), scale))
    {
        if (sgn(dot(n, p - a) / abs(n)) != 0
            || sgn(dot(n, q - a) / abs(n)) != 0)
        {
            return false;
        }
        int k = 0;
        if (fabsl(n.y) > fabsl(n.x))
        {
            k = 1;
        }
        if (fabsl(n.z) > (k == 0 ? fabsl(n.x) : fabsl(n.y)))
        {
            k = 2;
        }
        auto proj = [&](P3 x)
        {
            if (k == 0)
            {
                return P{x.y, x.z};
            }
            if (k == 1)
            {
                return P{x.x, x.z};
            }
            return P{x.x, x.y};
        };
        P x = proj(p);
        P y = proj(q);
        P A = proj(a);
        P B = proj(b);
        P C = proj(c);
        auto inside = [&](P z)
        {
            int x1 = sgn(cross(A, B, z));
            int x2 = sgn(cross(B, C, z));
            int x3 = sgn(cross(C, A, z));
            return !(min({x1, x2, x3}) < 0 && max({x1, x2, x3}) > 0);
        };
        return inside(x) || inside(y) || segIsect(x, y, A, B)
            || segIsect(x, y, B, C) || segIsect(x, y, C, A);
    }
    Real z = 1 / d;
    P3 s = p - a;
    Real u = dot(s, h) * z;
    if (u < -eps || u > 1 + eps)
    {
        return false;
    }
    Real w = dot(v, cross(s, e1)) * z;
    if (w < -eps || u + w > 1 + eps)
    {
        return false;
    }
    Real t = dot(e2, cross(s, e1)) * z;
    return t >= -eps && t <= 1 + eps;
}
