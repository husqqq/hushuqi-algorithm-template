#pragma once

#include "11_1_005.hpp"
#include "11_4_001.hpp"

using InvertObj = variant<Line<Real>, C>;

optional<P> invert(P p, C c)
{
    // p 是待反演点，c 是正半径反演圆；返回像点，圆心没有有限像。
    assert(sgn(c.r) > 0); // 调试检查，可删。
    P v = p - c.o;
    if (sgn(norm(v)) == 0)
    {
        return nullopt;
    }
    return c.o + v * (c.r * c.r / norm(v));
}

InvertObj invertLine(Line<Real> l, C c)
{
    // l 是待反演直线，c 是正半径反演圆；返回像直线或像圆。
    assert(!eq(l.a, l.b)); // 调试检查，可删。
    assert(sgn(c.r) > 0);  // 调试检查，可删。
    P v = l.b - l.a;
    P h = l.a + v * (dot(c.o - l.a, v) / norm(v));
    P d = h - c.o;
    if (sgn(norm(d)) == 0)
    {
        return Line<Real>{c.o, c.o + v};
    }
    P q = c.o + d * (c.r * c.r / norm(d));
    return C{(c.o + q) / 2, abs(q - c.o) / 2};
}

optional<InvertObj> invertCircle(C x, C c)
{
    // x 是待反演圆，c 是正半径反演圆；返回像直线或像圆，仅反演中心这个点圆返回空。
    assert(sgn(x.r) >= 0); // 调试检查，可删。
    assert(sgn(c.r) > 0);  // 调试检查，可删。
    P v = x.o - c.o;
    Real d2 = norm(v);
    if (sgn(x.r) == 0 && sgn(d2) == 0)
    {
        return nullopt;
    }
    Real k = c.r * c.r;
    Real z = d2 - x.r * x.r;
    if (sgn(z) == 0)
    {
        P p = c.o + v * (k / (2 * d2));
        return InvertObj{Line<Real>{p, p + rot(v)}};
    }
    return InvertObj{C{c.o + v * (k / z), k * x.r / fabsl(z)}};
}

optional<C> apollonius(P a, P b, Real k)
{
    // a、b 是不同定点，k 是到 a 的距离除以到 b 的距离；返回轨迹圆，k=1 的直线轨迹返回空。
    assert(!eq(a, b) && sgn(k) >= 0); // 调试检查，可删。
    if (sgn(k - 1) == 0)
    {
        return nullopt;
    }
    Real z = 1 - k * k;
    P o = (a - b * (k * k)) / z;
    return C{o, abs(a - b) * fabsl(k / z)};
}
