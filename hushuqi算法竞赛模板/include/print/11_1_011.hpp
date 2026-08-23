#pragma once

#include "11_1_003.hpp"

template <class T> int rectKind(const array<Point<T>, 4> &p)
{
    vector<T> d;
    for (int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            d.push_back(square(p[i] - p[j]));
        }
    }
    sort(d.begin(), d.end());
    if (sgn(d[0]) == 0 || !eq(d[0], d[1]) || !eq(d[2], d[3]) ||
        !eq(d[4], d[5]) || !eq(d[0] + d[2], d[4]))
    {
        return 0;
    }
    return eq(d[0], d[2]) ? 2 : 1;
}
