#pragma once

#include "11_1_003.hpp"

template <class T> int half(Point<T> p)
{
    // p 是向量；返回其所在的极角半平面编号。
    return p.y < 0 || (p.y == 0 && p.x < 0);
}

template <class T> bool polarCmp(Point<T> a, Point<T> b)
{
    // a、b 是向量；返回 a 的极角是否排在 b 前面。
    int x = half(a);
    int y = half(b);
    if (x != y)
    {
        return x < y;
    }
    auto c = cross(a, b);
    return c != 0 ? c > 0 : norm(a) < norm(b);
}

template <class T> void polarSort(vector<Point<T>> &a, Point<T> o = {0, 0})
{
    // a 是点列，o 是极点；原地按极角排序。
    sort(a.begin(),
         a.end(),
         [&](Point<T> x, Point<T> y)
         {
             return polarCmp(x - o, y - o);
         });
}
