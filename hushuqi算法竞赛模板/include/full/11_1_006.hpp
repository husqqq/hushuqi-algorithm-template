#pragma once
#include "11_1_002.hpp"
#include "11_1_003.hpp"
#include "11_1_005.hpp"

template <class T> bool parallel(const Line<T> &a, const Line<T> &b)
{
    // a、b 表示两条端点式直线；返回方向是否平行。
    return sgn(cross(a.b - a.a, b.b - b.a)) == 0;
}

template <class T> bool perp(const Line<T> &a, const Line<T> &b)
{
    // a、b 表示两条端点式直线；返回方向是否垂直。
    return sgn(dot(a.b - a.a, b.b - b.a)) == 0;
}

template <class T> int side(const Line<T> &l, const Point<T> &p)
{
    // l 表示有向直线，p 表示查询点；返回叉积符号，正数在左侧。
    return sgn(cross(l.b - l.a, p - l.a));
}

bool parallel(L a, L b)
{
    // a、b 表示两条点方向式直线；返回方向是否平行。
    return sgn(cross(a.v, b.v)) == 0;
}
