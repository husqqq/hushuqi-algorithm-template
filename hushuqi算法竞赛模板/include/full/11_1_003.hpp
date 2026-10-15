#pragma once

#include "11_1_002.hpp"

template <class T> T dot(const Point<T> &a, const Point<T> &b)
{
    // a、b 表示两个向量；返回点积。
    return a.x * b.x + a.y * b.y;
}

template <class T> T cross(const Point<T> &a, const Point<T> &b)
{
    // a、b 表示两个向量；返回有向叉积。
    return a.x * b.y - a.y * b.x;
}

template <class T> T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c)
{
    // a、b、c 表示三个点；返回三角形 abc 的有向面积两倍。
    return cross(b - a, c - a);
}

template <class T> T square(const Point<T> &a)
{
    // a 表示向量；返回模长平方。
    return dot(a, a);
}

template <class T> Real abs(const Point<T> &a)
{
    // a 表示向量；返回其欧氏模长。
    return sqrtl((Real)square(a));
}

template <class T> T norm(const Point<T> &a)
{
    // a 表示向量；返回模长平方。
    return square(a);
}

template <class T> Real angle(const Point<T> &a, const Point<T> &b)
{
    // a、b 是非零向量；返回 a 到 b 的有向夹角，范围为 (-pi,pi]。
    assert(a.x != T{} || a.y != T{}); // 调试检查，可删。
    assert(b.x != T{} || b.y != T{}); // 调试检查，可删。
    return atan2l(cross(a, b), dot(a, b));
}

template <class T> Point<Real> rotate(const Point<T> &a, Real t)
{
    // a 表示向量，t 表示逆时针旋转弧度；返回旋转后的浮点向量。
    return {a.x * cosl(t) - a.y * sinl(t), a.x * sinl(t) + a.y * cosl(t)};
}

template <class T> Point<T> rot(const Point<T> &a)
{
    // a 表示向量；返回逆时针旋转 90 度后的向量。
    return {-a.y, a.x};
}
