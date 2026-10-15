#pragma once

#include "11_2_001.hpp"
#include "11_2_004.hpp"

vector<P> polyKernel(const vector<P> &a)
{
    if (a.size() < 3)
    {
        return {};
    }
    vector<P> b = a;
    Real s = area(a);
    assert(sgn(s) != 0); // 调试检查，可删。
    int n = (int)a.size();
    for (int i = 0; i < n && !b.empty(); i++)
    {
        P x = a[i];
        P y = a[(i + 1) % n];
        if (s < 0)
        {
            swap(x, y);
        }
        b = cutPoly(b, {x, y - x});
    }
    return b;
}
