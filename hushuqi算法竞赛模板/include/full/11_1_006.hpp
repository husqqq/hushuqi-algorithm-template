#pragma once

#include "11_1_005.hpp"

template <class T> bool parallel(const Line<T> &a, const Line<T> &b)
{
    return sgn(cross(a.b - a.a, b.b - b.a)) == 0;
}

template <class T> bool perp(const Line<T> &a, const Line<T> &b)
{
    return sgn(dot(a.b - a.a, b.b - b.a)) == 0;
}

template <class T> int side(const Line<T> &l, const Point<T> &p)
{
    return sgn(cross(l.b - l.a, p - l.a));
}

bool parallel(L a, L b)
{
    return sgn(cross(a.v, b.v)) == 0;
}
