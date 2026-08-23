#pragma once

#include "11_1_009.hpp"

template <class T> int inPoly(const vector<Point<T>> &a, Point<T> p)
{
    // a 是简单多边形，p 是查询点；返回 0、1、2 表示外部、边界、内部。
    bool in = false;
    int n = (int)a.size();
    for (int i = 0; i < n; i++)
    {
        Point<T> x = a[i];
        Point<T> y = a[(i + 1) % n];
        if (onSeg(p, x, y))
        {
            return 1;
        }
        if (x.y > y.y)
        {
            swap(x, y);
        }
        if (sgn(x.y - p.y) <= 0 && sgn(p.y - y.y) < 0 && sgn(cross(x, y, p)) > 0)
        {
            in = !in;
        }
    }
    return in ? 2 : 0;
}

template <class T> pair<bool, int> windingNum(const vector<Point<T>> &a, Point<T> p)
{
    // a 是按顺序连接并首尾闭合的轮廓，p 是查询点；返回是否在边界上及有符号绕数，边界上时绕数置零。
    int ans = 0;
    int n = (int)a.size();
    for (int i = 0; i < n; i++)
    {
        Point<T> x = a[i];
        Point<T> y = a[(i + 1) % n];
        if (onSeg(p, x, y))
        {
            return {true, 0};
        }
        int sx = sgn(x.y - p.y);
        int sy = sgn(y.y - p.y);
        int c = sgn(cross(x, y, p));
        if (sx <= 0 && sy > 0 && c > 0)
        {
            ans++;
        }
        if (sy <= 0 && sx > 0 && c < 0)
        {
            ans--;
        }
    }
    return {false, ans};
}
