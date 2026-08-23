#pragma once

#include "11_1_003.hpp"

template <class T> struct Line
{
    Point<T> a;
    Point<T> b;

    Line(Point<T> a = {}, Point<T> b = {}) : a(a), b(b)
    {
    }
};

struct DirLine
{
    P p;
    P v;

    P at(Real t) const
    {
        return p + v * t;
    }
};

using L = DirLine;

template <class T> DirLine toDir(const Line<T> &l)
{
    auto v = l.b - l.a;
    assert(l.a != l.b); // 调试检查，可删。
    return {(P)l.a, (P)v};
}
