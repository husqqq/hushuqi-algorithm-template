#pragma once

#include "14_1_001.hpp"

struct Frac
{
    int x, y; // x、y 是既约分子和正分母。

    Frac(int x = 0, int y = 1) : x(x), y(y)
    {
        // x、y 是分子和非零分母；构造既约分数 x/y。
        assert(y); // 调试检查，可删。
        if (y < 0)
        {
            x = -x;
            y = -y;
        }
        int g = gcd(abs(x), y);
        this->x = x / g;
        this->y = y / g;
    }

    friend Frac operator+(const Frac &a, const Frac &b)
    {
        // a、b 是两个分数；返回 a+b。
        int g = gcd(a.y, b.y);
        return {a.x * (b.y / g) + b.x * (a.y / g), a.y / g * b.y};
    }
    friend Frac operator-(const Frac &a, const Frac &b)
    {
        // a 是被减数，b 是减数；返回 a-b。
        int g = gcd(a.y, b.y);
        return {a.x * (b.y / g) - b.x * (a.y / g), a.y / g * b.y};
    }
    friend Frac operator*(const Frac &a, const Frac &b)
    {
        // a、b 是两个分数；返回 a*b。
        int g = gcd(abs(a.x), b.y);
        int h = gcd(abs(b.x), a.y);
        return {a.x / g * (b.x / h), a.y / h * (b.y / g)};
    }
    friend Frac operator/(const Frac &a, const Frac &b)
    {
        // a 是被除数，b 是非零除数；返回 a/b。
        assert(b.x); // 调试检查，可删。
        int g = gcd(abs(a.x), abs(b.x));
        int h = gcd(a.y, b.y);
        return {a.x / g * (b.y / h), a.y / h * (b.x / g)};
    }
    friend bool operator==(const Frac &a, const Frac &b)
    {
        // a、b 是两个分数；返回二者是否相等。
        return a.x == b.x && a.y == b.y;
    }
    friend bool operator<(const Frac &a, const Frac &b)
    {
        // a、b 是两个分数；返回 a 是否小于 b。
        return (__int128)a.x * b.y < (__int128)b.x * a.y;
    }
};

optional<Frac> ratRecover(int a, int m, int b)
{
    // 0<=a<m，b>=1，2b^2<m；返回满足 p=a*q (mod m)、|p|<=b、1<=q<=b
    // 且 gcd(q,m)=1 的唯一既约分数 p/q；不存在时返回空。
    assert(0 <= a && a < m && b >= 1 && 2 * (__int128)b * b < m);
    int r0 = m, r1 = a, t0 = 0, t1 = 1; // 始终有 ri=a*ti (mod m)。
    while (r1 > b)
    {
        int k = r0 / r1;
        int r = r0 - k * r1;
        // 系数幅值不超过 m；乘减局部加宽，避免中间乘积溢出。
        int t = (int)((__int128)t0 - (__int128)k * t1);
        r0 = r1;
        r1 = r;
        t0 = t1;
        t1 = t;
    }
    if (t1 < 0)
    {
        r1 = -r1;
        t1 = -t1;
    }
    if (t1 > b || gcd(abs(r1), t1) != 1 || gcd(t1, m) != 1) return nullopt;
    return Frac(r1, t1);
}
