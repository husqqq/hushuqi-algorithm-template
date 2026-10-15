#pragma once

#include "11_1_001.hpp"

struct P3
{
    Real x;
    Real y;
    Real z;

    P3 operator+(P3 p) const
    {
        return {x + p.x, y + p.y, z + p.z};
    }

    P3 operator-(P3 p) const
    {
        return {x - p.x, y - p.y, z - p.z};
    }

    P3 operator*(Real k) const
    {
        return {x * k, y * k, z * k};
    }

    P3 operator/(Real k) const
    {
        assert(sgn(k) != 0); // 调试检查，可删。
        return {x / k, y / k, z / k};
    }
};

Real dot(P3 a, P3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

P3 cross(P3 a, P3 b)
{
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

Real norm(P3 a)
{
    return dot(a, a);
}

Real abs(P3 a)
{
    return sqrtl(norm(a));
}

P3 unit(P3 a)
{
    return a / abs(a);
}

struct L3
{
    P3 p;
    P3 v;
};

struct Plane
{
    P3 p;
    P3 n;
};
