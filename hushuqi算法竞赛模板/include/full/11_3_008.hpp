#pragma once

#include "11_3_001.hpp"

template <class T> vector<Point<T>> minkowski(vector<Point<T>> a, vector<Point<T>> b)
{
    a = hull(a);
    b = hull(b);
    if (a.empty() || b.empty())
    {
        return {};
    }
    if (a.size() <= 2 || b.size() <= 2)
    {
        vector<Point<T>> c;
        for (Point<T> x : a)
        {
            for (Point<T> y : b)
            {
                c.push_back(x + y);
            }
        }
        return hull(c);
    }
    auto edge = [](const vector<Point<T>> &p)
    {
        vector<Point<T>> e(p.size());
        for (int i = 0; i < (int)p.size(); i++)
        {
            e[i] = p[(i + 1) % p.size()] - p[i];
        }
        return e;
    };
    auto x = edge(a);
    auto y = edge(b);
    vector<Point<T>> c = {a[0] + b[0]};
    int i = 0;
    int j = 0;
    while (i < (int)x.size() || j < (int)y.size())
    {
        if (j == (int)y.size()
            || (i < (int)x.size() && sgn(cross(x[i], y[j])) > 0))
        {
            c.push_back(c.back() + x[i++]);
        }
        else if (i == (int)x.size() || sgn(cross(x[i], y[j])) < 0)
        {
            c.push_back(c.back() + y[j++]);
        }
        else
        {
            c.push_back(c.back() + x[i++] + y[j++]);
        }
    }
    c.pop_back();
    return hull(c);
}
