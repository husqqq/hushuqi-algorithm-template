#pragma once

#include "14_1_001.hpp"

struct Frac
{
    long long x, y;

    Frac(long long x = 0, long long y = 1) : x(x), y(y)
    {
        assert(y);
        if (y < 0)
        {
            x = -x;
            y = -y;
        }
        long long g = gcd(llabs(x), y);
        this->x = x / g;
        this->y = y / g;
    }

    friend Frac operator+(const Frac &a, const Frac &b)
    {
        // a、b 是两个分数；返回 a+b。
        long long g = gcd(a.y, b.y);
        return {a.x * (b.y / g) + b.x * (a.y / g), a.y / g * b.y};
    }

    friend Frac operator-(const Frac &a, const Frac &b)
    {
        // a 是被减数，b 是减数；返回 a-b。
        long long g = gcd(a.y, b.y);
        return {a.x * (b.y / g) - b.x * (a.y / g), a.y / g * b.y};
    }

    friend Frac operator*(const Frac &a, const Frac &b)
    {
        // a、b 是两个分数；返回 a*b。
        long long g = gcd(llabs(a.x), b.y);
        long long h = gcd(llabs(b.x), a.y);
        return {a.x / g * (b.x / h), a.y / h * (b.y / g)};
    }

    friend Frac operator/(const Frac &a, const Frac &b)
    {
        // a 是被除数，b 是非零除数；返回 a/b。
        assert(b.x);
        long long g = gcd(llabs(a.x), llabs(b.x));
        long long h = gcd(a.y, b.y);
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
