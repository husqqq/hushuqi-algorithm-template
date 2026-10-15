#pragma once

#include "11_1_002.hpp"

template <class T> T manhattan(Point<T> a, Point<T> b)
{
    // a、b 是两个点；返回曼哈顿距离。
    T x = a.x - b.x, y = a.y - b.y;
    return (x < 0 ? -x : x) + (y < 0 ? -y : y);
}

template <class T> T chebyshev(Point<T> a, Point<T> b)
{
    // a、b 是两个点；返回切比雪夫距离。
    T x = a.x - b.x, y = a.y - b.y;
    x = x < 0 ? -x : x;
    y = y < 0 ? -y : y;
    return max(x, y);
}

// (x,y)->(x+y,x-y)：原坐标曼哈顿距离等于新坐标切比雪夫距离。
template <class T> Point<T> toCheb(Point<T> p)
{
    // p 是平面点；返回对应切比雪夫坐标。
    return {p.x + p.y, p.x - p.y};
}

// 逆变换；若使用整数坐标，u,v 必须同奇偶。
P fromCheb(P p)
{
    // p 是浮点切比雪夫坐标；返回原坐标。
    return {(p.x + p.y) / 2, (p.x - p.y) / 2};
}

Point<long long> fromCheb(Point<long long> p)
{
    // p 是同奇偶的整数切比雪夫坐标；返回精确原坐标。
    assert((p.x - p.y) % 2 == 0); // 调试检查，可删。
    return {(p.x + p.y) / 2, (p.x - p.y) / 2};
}
