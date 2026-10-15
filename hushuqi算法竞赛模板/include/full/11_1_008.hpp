#pragma once

#include "11_1_007.hpp"

template <class T> Real distancePL(const Point<T> &p, const Line<T> &l)
{
    assert(l.a != l.b); // 调试检查，可删。
    return fabsl((Real)cross(l.b - l.a, p - l.a)) / abs(l.b - l.a);
}

template <class T> Real distancePS(const Point<T> &p, const Line<T> &l)
{
    if (l.a == l.b)
    {
        return abs(p - l.a);
    }
    if (dot(p - l.a, l.b - l.a) <= 0)
    {
        return abs(p - l.a);
    }
    if (dot(p - l.b, l.a - l.b) <= 0)
    {
        return abs(p - l.b);
    }
    return distancePL(p, l);
}
