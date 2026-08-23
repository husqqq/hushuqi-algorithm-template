#pragma once

#include "11_1_003.hpp"

template <class T> vector<Point<T>> hull(vector<Point<T>> a)
{
    sort(a.begin(), a.end());
    a.erase(unique(a.begin(), a.end()), a.end());
    if (a.size() <= 1)
    {
        return a;
    }
    vector<Point<T>> h;
    for (int t = 0; t < 2; t++)
    {
        int z = (int)h.size();
        for (Point<T> p : a)
        {
            while ((int)h.size() >= z + 2
                   && sgn(cross(h.back() - h[h.size() - 2], p - h.back())) <= 0)
            {
                h.pop_back();
            }
            h.push_back(p);
        }
        h.pop_back();
        reverse(a.begin(), a.end());
    }
    return h;
}
