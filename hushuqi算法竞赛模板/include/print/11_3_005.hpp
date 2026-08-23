#pragma once

#include "11_3_004.hpp"

template <class T> optional<array<Line<T>, 2>> hullTan(const vector<Point<T>> &a, Point<T> p)
{
    // a 是严格逆时针凸包，p 是查询点；外点返回两条切线，否则返回空。
    auto [x, y] = hullTangents(a, p);
    if (x < 0)
    {
        return nullopt;
    }
    return array<Line<T>, 2>{Line<T>{p, a[x]}, Line<T>{p, a[y]}};
}
