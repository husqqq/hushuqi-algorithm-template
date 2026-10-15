#pragma once
#include "11_1_002.hpp"
#include "11_1_005.hpp"

template <class T> bool onSegment(const Point<T> &p, const Line<T> &l)
{
    // p 表示查询点，l 表示闭线段；返回 p 是否在线段上。
    return sgn(cross(p - l.a, l.b - l.a)) == 0 && sgn(dot(p - l.a, p - l.b)) <= 0;
}

template <class T> bool segMeet(const Line<T> &a, const Line<T> &b)
{
    // a、b 表示两条闭线段；返回它们是否至少有一个公共点。
    int c1 = sgn(cross(a.b - a.a, b.a - a.a));
    int c2 = sgn(cross(a.b - a.a, b.b - a.a));
    int c3 = sgn(cross(b.b - b.a, a.a - b.a));
    int c4 = sgn(cross(b.b - b.a, a.b - b.a));
    if (c1 == 0 && onSegment(b.a, a))
    {
        return true;
    }
    if (c2 == 0 && onSegment(b.b, a))
    {
        return true;
    }
    if (c3 == 0 && onSegment(a.a, b))
    {
        return true;
    }
    if (c4 == 0 && onSegment(a.b, b))
    {
        return true;
    }
    return ((c1 > 0) != (c2 > 0)) && ((c3 > 0) != (c4 > 0));
}

template <class T> Real distanceSS(const Line<T> &a, const Line<T> &b)
{
    // a、b 表示两条闭线段；返回线段间最短距离。
    if (segMeet(a, b))
    {
        return 0;
    }
    return min({distancePS(a.a, b), distancePS(a.b, b), distancePS(b.a, a), distancePS(b.b, a)});
}

template <class T> bool onSeg(Point<T> p, Point<T> a, Point<T> b)
{
    // p 表示查询点，a、b 表示线段端点；返回 p 是否在线段上。
    return onSegment(p, Line<T>{a, b});
}

template <class T> bool segIsect(Point<T> a, Point<T> b, Point<T> c, Point<T> d)
{
    // a、b 与 c、d 分别表示两条闭线段；返回是否相交。
    return segMeet(Line<T>{a, b}, Line<T>{c, d});
}

template <class T> Real segDis(Point<T> a, Point<T> b, Point<T> c, Point<T> d)
{
    // a、b 与 c、d 分别表示两条闭线段；返回最短距离。
    return distanceSS(Line<T>{a, b}, Line<T>{c, d});
}
