#pragma once
#include "11_1_001.hpp"
#include "11_1_002.hpp"
#include "11_1_005.hpp"

template <class T> Real distancePL(const Point<T> &p, const Line<T> &l)
{
    // p 表示点，l 表示非退化直线；返回点到无限直线的距离。
    assert(l.a != l.b); // 调试检查，可删。
    return fabsl((Real)cross(l.b - l.a, p - l.a)) / abs(l.b - l.a);
}

template <class T> Real distancePS(const Point<T> &p, const Line<T> &l)
{
    // p 表示点，l 表示线段；返回点到闭线段的最短距离。
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
