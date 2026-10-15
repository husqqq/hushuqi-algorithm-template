#pragma once

#include "11_1_003.hpp"

template <class T> T area2(const vector<Point<T>> &a)
{
    T ans = 0;
    int n = (int)a.size();
    for (int i = 0; i < n; i++)
    {
        ans += cross(a[i], a[(i + 1) % n]);
    }
    return ans;
}

template <class T> Real area(const vector<Point<T>> &a)
{
    return (Real)area2(a) / 2;
}

template <class T> Real perimeter(const vector<Point<T>> &a)
{
    Real ans = 0;
    int n = (int)a.size();
    for (int i = 0; i < n; i++)
    {
        ans += abs(a[(i + 1) % n] - a[i]);
    }
    return ans;
}

template <class T> Point<Real> cen(const vector<Point<T>> &a)
{
    P ans{};
    Real s = 0;
    int n = (int)a.size();
    for (int i = 0; i < n; i++)
    {
        Real c = (Real)cross(a[i], a[(i + 1) % n]);
        ans += ((P)a[i] + (P)a[(i + 1) % n]) * c;
        s += c;
    }
    assert(sgn(s) != 0); // 调试检查，可删。
    return ans / (3 * s);
}
