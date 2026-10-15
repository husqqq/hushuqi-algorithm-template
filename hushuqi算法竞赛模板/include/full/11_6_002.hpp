#pragma once

#include "11_6_001.hpp"

struct IP3
{
    long long x; // x 是整数点的第一维坐标。
    long long y; // y 是整数点的第二维坐标。
    long long z; // z 是整数点的第三维坐标。
};

int mixedSign(IP3 a, IP3 b, IP3 c, IP3 d)
{
    // a、b、c、d 是整数空间点；返回 (b-a)·((c-a)×(d-a)) 的精确符号。
    __int128 ux = (__int128)b.x - a.x;
    __int128 uy = (__int128)b.y - a.y;
    __int128 uz = (__int128)b.z - a.z;
    __int128 vx = (__int128)c.x - a.x;
    __int128 vy = (__int128)c.y - a.y;
    __int128 vz = (__int128)c.z - a.z;
    __int128 wx = (__int128)d.x - a.x;
    __int128 wy = (__int128)d.y - a.y;
    __int128 wz = (__int128)d.z - a.z;
    __int128 det = ux * (vy * wz - vz * wy) - uy * (vx * wz - vz * wx) + uz * (vx * wy - vy * wx);
    return (det > 0) - (det < 0);
}

bool collinearEx(IP3 a, IP3 b, IP3 c)
{
    // a、b、c 是整数空间点；返回三点是否共线，重合点视为共线。
    __int128 ux = (__int128)b.x - a.x;
    __int128 uy = (__int128)b.y - a.y;
    __int128 uz = (__int128)b.z - a.z;
    __int128 vx = (__int128)c.x - a.x;
    __int128 vy = (__int128)c.y - a.y;
    __int128 vz = (__int128)c.z - a.z;
    return uy * vz - uz * vy == 0 && uz * vx - ux * vz == 0 && ux * vy - uy * vx == 0;
}

bool coplanarEx(IP3 a, IP3 b, IP3 c, IP3 d)
{
    // a、b、c、d 是整数空间点；返回四点是否共面，退化点集视为共面。
    return mixedSign(a, b, c, d) == 0;
}

bool collinear(P3 a, P3 b, P3 c)
{
    // a、b、c 是三个空间点；返回三点是否共线，重合点视为共线。
    P3 u = b - a;
    P3 v = c - a;
    if (sgn(abs(u)) == 0 || sgn(abs(v)) == 0)
    {
        return true;
    }
    return sgn(abs(cross(u, v)) / abs(u) / abs(v)) == 0;
}

bool coplanar(P3 a, P3 b, P3 c, P3 d)
{
    // a、b、c、d 是四个空间点；返回四点是否共面，重合或共线退化视为共面。
    P3 u = b - a;
    P3 v = c - a;
    P3 w = d - a;
    if (sgn(abs(u)) == 0 || sgn(abs(v)) == 0 || sgn(abs(w)) == 0)
    {
        return true;
    }
    return sgn(dot(u, cross(v, w)) / abs(u) / abs(v) / abs(w)) == 0;
}

bool onLine3(P3 p, L3 l)
{
    // p 是查询点，l 是非退化直线；返回 p 是否在 l 上。
    assert(sgn(abs(l.v)) != 0); // 调试检查，可删。
    P3 u = p - l.p;
    if (sgn(abs(u)) == 0)
    {
        return true;
    }
    return sgn(abs(cross(u, l.v)) / abs(u) / abs(l.v)) == 0;
}

bool onSegment3(P3 p, P3 a, P3 b)
{
    // p 是查询点，a、b 是闭线段端点；返回 p 是否在线段上，a=b 时线段是单点。
    P3 v = b - a;
    if (sgn(abs(v)) == 0)
    {
        return sgn(abs(p - a)) == 0;
    }
    if (!onLine3(p, {a, v}))
    {
        return false;
    }
    Real t = dot(p - a, v) / norm(v);
    return t >= -eps && t <= 1 + eps;
}

bool onPlane3(P3 p, Plane s)
{
    // p 是查询点，s 是非退化平面；返回 p 是否在 s 上。
    assert(sgn(abs(s.n)) != 0); // 调试检查，可删。
    return sgn(dot(p - s.p, s.n) / abs(s.n)) == 0;
}

bool parallel(L3 a, L3 b)
{
    // a、b 是两条非退化直线；返回二者方向是否平行。
    assert(sgn(abs(a.v)) != 0 && sgn(abs(b.v)) != 0); // 调试检查，可删。
    return sgn(abs(cross(a.v, b.v)) / abs(a.v) / abs(b.v)) == 0;
}

bool parallel(Plane a, Plane b)
{
    // a、b 是两个非退化平面；返回二者是否平行。
    assert(sgn(abs(a.n)) != 0 && sgn(abs(b.n)) != 0); // 调试检查，可删。
    return sgn(abs(cross(a.n, b.n)) / abs(a.n) / abs(b.n)) == 0;
}

bool parallel(L3 a, Plane b)
{
    // a 是非退化直线，b 是非退化平面；返回直线方向是否平行于平面。
    assert(sgn(abs(a.v)) != 0 && sgn(abs(b.n)) != 0); // 调试检查，可删。
    return sgn(dot(a.v, b.n) / abs(a.v) / abs(b.n)) == 0;
}

bool perp(L3 a, L3 b)
{
    // a、b 是两条非退化直线；返回二者方向是否垂直。
    assert(sgn(abs(a.v)) != 0 && sgn(abs(b.v)) != 0); // 调试检查，可删。
    return sgn(dot(a.v, b.v) / abs(a.v) / abs(b.v)) == 0;
}

bool perp(Plane a, Plane b)
{
    // a、b 是两个非退化平面；返回二者法向量是否垂直。
    assert(sgn(abs(a.n)) != 0 && sgn(abs(b.n)) != 0); // 调试检查，可删。
    return sgn(dot(a.n, b.n) / abs(a.n) / abs(b.n)) == 0;
}
