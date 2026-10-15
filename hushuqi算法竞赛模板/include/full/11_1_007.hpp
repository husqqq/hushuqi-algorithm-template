#pragma once

#include "11_1_006.hpp"

template <class T> Point<Real> lineMeet(const Line<T> &a, const Line<T> &b)
{
    auto u = (P)a.a;
    auto v = (P)(a.b - a.a);
    auto p = (P)b.a;
    auto q = (P)(b.b - b.a);
    assert(sgn(cross(v, q)) != 0); // 调试检查，可删。
    return u + v * (cross(p - u, q) / cross(v, q));
}

template <class T> Point<Real> projection(const Line<T> &l, const Point<T> &p)
{
    auto a = (P)l.a;
    auto v = (P)(l.b - l.a);
    auto q = (P)p;
    assert(sgn(square(v)) != 0); // 调试检查，可删。
    return a + v * (dot(q - a, v) / square(v));
}

template <class T> Point<Real> reflection(const Line<T> &l, const Point<T> &p)
{
    return projection(l, p) * 2 - (P)p;
}

P isect(L a, L b)
{
    assert(sgn(cross(a.v, b.v)) != 0); // 调试检查，可删。
    return a.at(cross(b.p - a.p, b.v) / cross(a.v, b.v));
}

P proj(L l, P p)
{
    assert(sgn(square(l.v)) != 0); // 调试检查，可删。
    return l.at(dot(p - l.p, l.v) / square(l.v));
}

P refl(L l, P p)
{
    return proj(l, p) * 2 - p;
}

Real dis(L l, P p)
{
    assert(sgn(square(l.v)) != 0); // 调试检查，可删。
    return fabsl(cross(l.v, p - l.p)) / abs(l.v);
}
