#pragma once

#include "11_1_001.hpp"

struct P3
{
    Real x; // x 保存横坐标。
    Real y; // y 保存纵坐标。
    Real z; // z 保存竖坐标。

    P3 operator+(P3 p) const
    {
        // p 是另一三维点或向量；返回逐分量之和。
        return {x + p.x, y + p.y, z + p.z};
    }
    P3 operator-(P3 p) const
    {
        // p 是另一三维点或向量；返回逐分量之差，即从 p 指向当前点的向量。
        return {x - p.x, y - p.y, z - p.z};
    }
    P3 operator*(Real k) const
    {
        // k 是实数标量；返回各分量乘以 k 的向量。
        return {x * k, y * k, z * k};
    }
    P3 operator/(Real k) const
    {
        // k 是非零实数标量；返回各分量除以 k 的向量。
        assert(sgn(k) != 0); // 调试检查，可删。
        return {x / k, y / k, z / k};
    }
};

Real dot(P3 a, P3 b)
{
    // a、b 是三维向量；返回点积 a·b。
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
P3 cross(P3 a, P3 b)
{
    // a、b 是三维向量；返回右手系叉积 a×b，与 a、b 都垂直。
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
Real norm(P3 a)
{
    // a 是三维向量；返回模长平方 |a|^2。
    return dot(a, a);
}
Real abs(P3 a)
{
    // a 是三维向量；返回模长 |a|。
    return sqrtl(norm(a));
}
P3 unit(P3 a)
{
    // a 是非零三维向量；返回同方向单位向量 a/|a|。
    return a / abs(a);
}

struct L3
{
    P3 p; // p 是直线上的一点。
    P3 v; // v 是非零方向向量。
};

struct Plane
{
    P3 p; // p 是平面上的一点。
    P3 n; // n 是非零法向量。
};
