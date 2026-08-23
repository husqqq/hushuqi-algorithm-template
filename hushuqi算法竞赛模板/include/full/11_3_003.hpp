#pragma once

#include "11_1_009.hpp"

template <class T> bool inConvex(const vector<Point<T>> &a, Point<T> p)
{
    int n = (int)a.size();
    if (n == 0)
    {
        return false;
    }
    if (n == 1)
    {
        return eq(a[0], p);
    }
    if (n == 2)
    {
        return onSeg(p, a[0], a[1]);
    }
    if (sgn(cross(a[0], a[1], p)) < 0
        || sgn(cross(a[0], a.back(), p)) > 0)
    {
        return false;
    }
    int l = 1;
    int r = n - 1;
    while (r - l > 1)
    {
        int m = (l + r) / 2;
        if (sgn(cross(a[m] - a[0], p - a[0])) >= 0)
        {
            l = m;
        }
        else
        {
            r = m;
        }
    }
    return sgn(cross(a[l], a[(l + 1) % n], p)) >= 0;
}
