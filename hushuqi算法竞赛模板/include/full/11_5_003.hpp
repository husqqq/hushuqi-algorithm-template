#pragma once

#include "11_1_002.hpp"

template <class T> T manhattan(Point<T> a, Point<T> b)
{
    T x = a.x - b.x, y = a.y - b.y;
    return (x < 0 ? -x : x) + (y < 0 ? -y : y);
}

template <class T> T chebyshev(Point<T> a, Point<T> b)
{
    T x = a.x - b.x, y = a.y - b.y;
    x = x < 0 ? -x : x;
    y = y < 0 ? -y : y;
    return max(x, y);
}

template <class T> Point<T> toCheb(Point<T> p)
{
    return {p.x + p.y, p.x - p.y};
}

P fromCheb(P p)
{
    return {(p.x + p.y) / 2, (p.x - p.y) / 2};
}

Point<long long> fromCheb(Point<long long> p)
{
    assert((p.x - p.y) % 2 == 0);
    return {(p.x + p.y) / 2, (p.x - p.y) / 2};
}
