#pragma once

#include "11_1_008.hpp"

template <class T> bool onSegment(const Point<T> &p, const Line<T> &l)
{
    return sgn(cross(p - l.a, l.b - l.a)) == 0 && sgn(dot(p - l.a, p - l.b)) <= 0;
}

template <class T> bool segMeet(const Line<T> &a, const Line<T> &b)
{
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
    if (segMeet(a, b))
    {
        return 0;
    }
    return min({distancePS(a.a, b), distancePS(a.b, b),
                distancePS(b.a, a), distancePS(b.b, a)});
}

template <class T> bool onSeg(Point<T> p, Point<T> a, Point<T> b)
{
    return onSegment(p, Line<T>{a, b});
}

template <class T> bool segIsect(Point<T> a, Point<T> b, Point<T> c, Point<T> d)
{
    return segMeet(Line<T>{a, b}, Line<T>{c, d});
}

template <class T> Real segDis(Point<T> a, Point<T> b, Point<T> c, Point<T> d)
{
    return distanceSS(Line<T>{a, b}, Line<T>{c, d});
}
