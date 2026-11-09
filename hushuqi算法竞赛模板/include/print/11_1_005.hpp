#pragma once
#include "11_1_002.hpp"

template <class T> struct Line
{
    Point<T> a; // a 保存直线、射线或线段的起点。
    Point<T> b; // b 保存直线上的第二点；作为射线时方向为 b-a。

    Line(Point<T> a = {}, Point<T> b = {}) : a(a), b(b)
    {
        // a、b 表示两个端点；构造端点式直线。
    }
};

template <class T> array<T, 3> lineABC(const Line<T> &l)
{
    // l 是由两个不同整数点确定的直线；返回规范化后的 {A,B,C}，满足 Ax+By+C=0。
    static_assert(is_integral_v<T>, "lineABC requires integral coordinates");
    assert(l.a != l.b); // 调试检查，可删。
    T A = l.a.y - l.b.y;
    T B = l.b.x - l.a.x;
    T C = l.a.x * l.b.y - l.b.x * l.a.y;
    T g = gcd(gcd(A, B), C);
    if (g < 0) g = -g;
    assert(g != 0); // 非退化直线的三个系数不全为零。
    A /= g;
    B /= g;
    C /= g;
    if (A < 0 || (A == 0 && B < 0) || (A == 0 && B == 0 && C < 0))
        A = -A, B = -B, C = -C;
    return {A, B, C};
}

struct DirLine
{
    P p; // p 保存直线上一点。
    P v; // v 保存非零方向向量。

    P at(Real t) const
    {
        // t 表示方向参数；返回 p+v*t。
        return p + v * t;
    }
};

using L = DirLine;

template <class T> DirLine toDir(const Line<T> &l)
{
    // l 表示端点式直线；返回供浮点算法使用的点方向式直线。
    auto v = l.b - l.a;
    assert(l.a != l.b); // 调试检查，可删。
    return {(P)l.a, (P)v};
}
