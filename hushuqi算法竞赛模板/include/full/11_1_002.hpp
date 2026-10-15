#pragma once

#include "11_1_001.hpp"

template <class T> struct Point
{
    T x; // x 保存横坐标。
    T y; // y 保存纵坐标。

    Point(const T &x = 0, const T &y = 0) : x(x), y(y)
    {
        // x、y 表示初始坐标；构造点或向量。
    }

    template <class U> explicit operator Point<U>() const
    {
        // U 表示目标坐标类型；返回逐坐标 C 风格转换后的点。
        return Point<U>((U)x, (U)y);
    }

    Point &operator+=(const Point &p)
    {
        // p 表示另一个向量；把 p 加到当前向量并返回自身。
        x += p.x;
        y += p.y;
        return *this;
    }

    Point &operator-=(const Point &p)
    {
        // p 表示另一个向量；从当前向量减去 p 并返回自身。
        x -= p.x;
        y -= p.y;
        return *this;
    }

    Point &operator*=(const T &k)
    {
        // k 表示缩放倍数；原地数乘并返回自身。
        x *= k;
        y *= k;
        return *this;
    }

    Point &operator/=(const T &k)
    {
        // k 表示非零除数；原地除法并返回自身。
        assert(k != T(0));
        x /= k;
        y /= k;
        return *this;
    }

    Point operator-() const
    {
        // 无参数；返回反向向量。
        return {-x, -y};
    }

    friend Point operator+(Point a, const Point &b)
    {
        // a、b 表示两个向量；返回逐坐标之和。
        return a += b;
    }

    friend Point operator-(Point a, const Point &b)
    {
        // a、b 表示两个点或向量；返回 a-b。
        return a -= b;
    }

    friend Point operator*(Point a, const T &k)
    {
        // a 表示向量，k 表示倍数；返回缩放后的向量。
        return a *= k;
    }

    friend Point operator*(const T &k, Point a)
    {
        // k 表示倍数，a 表示向量；返回缩放后的向量。
        return a *= k;
    }

    friend Point operator/(Point a, const T &k)
    {
        // a 表示向量，k 表示非零除数；返回缩放后的向量。
        return a /= k;
    }

    friend bool operator<(const Point &a, const Point &b)
    {
        // a、b 表示两个点；返回按 x、y 的字典序比较结果。
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    }

    friend bool operator==(const Point &a, const Point &b)
    {
        // a、b 表示两个点；返回坐标是否精确相等。
        return a.x == b.x && a.y == b.y;
    }
};

using P = Point<Real>;

template <class T> bool eq(Point<T> a, Point<T> b)
{
    // a、b 表示两个点；整数坐标精确比较，浮点坐标按统一误差比较。
    return eq(a.x, b.x) && eq(a.y, b.y);
}
