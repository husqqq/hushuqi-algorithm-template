#pragma once

#include "11_1_002.hpp"

template <class T> T farManhattan(const vector<Point<T>> &a)
{
    if (a.empty())
    {
        return 0;
    }
    T lo1 = a[0].x + a[0].y, hi1 = lo1;
    T lo2 = a[0].x - a[0].y, hi2 = lo2;
    for (auto p : a)
    {
        T u = p.x + p.y, v = p.x - p.y;
        lo1 = min(lo1, u);
        hi1 = max(hi1, u);
        lo2 = min(lo2, v);
        hi2 = max(hi2, v);
    }
    return max(hi1 - lo1, hi2 - lo2);
}
