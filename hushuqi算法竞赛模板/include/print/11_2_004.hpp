#pragma once

#include "11_1_007.hpp"

vector<P> cutPoly(const vector<P> &a, L l)
{
    vector<P> b;
    int n = (int)a.size();
    for (int i = 0; i < n; i++)
    {
        P x = a[i];
        P y = a[(i + 1) % n];
        int sx = sgn(cross(l.v, x - l.p));
        int sy = sgn(cross(l.v, y - l.p));
        if (sx >= 0)
        {
            b.push_back(x);
        }
        if (sx * sy < 0)
        {
            b.push_back(isect({x, y - x}, l));
        }
    }
    return b;
}
