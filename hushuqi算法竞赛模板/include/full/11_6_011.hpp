#pragma once

#include "11_6_007.hpp"
#include "11_6_010.hpp"

struct Ray3
{
    P3 p; // p 是射线起点。
    P3 v; // v 是射线方向，查询函数允许零方向并返回 Degenerate。
};

struct AABB3
{
    P3 lo; // lo 是三个坐标的闭区间下界。
    P3 hi; // hi 是三个坐标的闭区间上界。
};

struct ParamPoint3
{
    Real t;   // t 是参数式 p+v*t 中的参数。
    P3 point; // point 是参数 t 对应的空间点。
};

struct ParamRange3
{
    Real enter; // enter 是命中闭集的最小参数。
    Real exit;  // exit 是离开闭集的最大参数。
    P3 first;   // first 是 enter 对应的点。
    P3 last;    // last 是 exit 对应的点。
};

enum class RayState3
{
    None,
    Hit,
    Degenerate
};

struct RayPtQuery3
{
    RayState3 state; // state 表示未命中、命中或零方向退化。
    ParamPoint3 hit;      // hit 只在 state=Hit 时有效。
};

struct RayIntQuery3
{
    RayState3 state; // state 表示未命中、命中或零方向退化。
    ParamRange3 hit;   // hit 只在 state=Hit 时有效。
};

bool in(AABB3 b, P3 p)
{
    // b 是合法闭包围盒，p 是查询点；返回 p 是否在盒内或边界上。
    assert(b.lo.x <= b.hi.x && b.lo.y <= b.hi.y && b.lo.z <= b.hi.z); // 调试检查，可删。
    auto inside = [&](Real x, Real l, Real r)
    {
        return (x > l || eq(x, l)) && (x < r || eq(x, r));
    };
    return inside(p.x, b.lo.x, b.hi.x) && inside(p.y, b.lo.y, b.hi.y) && inside(p.z, b.lo.z, b.hi.z);
}

optional<ParamRange3> aabbRange3(P3 p, P3 v, AABB3 b, Real lower, Real upper)
{
    // p+v*t 是参数直线，b 是合法闭包围盒，[lower,upper] 是参数域；返回命中参数区间，未命中返回空。
    assert(lower <= upper);                                           // 调试检查，可删。
    assert(b.lo.x <= b.hi.x && b.lo.y <= b.hi.y && b.lo.z <= b.hi.z); // 调试检查，可删。
    array<Real, 3> origin{p.x, p.y, p.z};
    array<Real, 3> dir{v.x, v.y, v.z};
    array<Real, 3> lo{b.lo.x, b.lo.y, b.lo.z};
    array<Real, 3> hi{b.hi.x, b.hi.y, b.hi.z};
    if (v.x == 0 && v.y == 0 && v.z == 0)
    {
        if (!in(b, p))
        {
            return nullopt;
        }
        return ParamRange3{lower, upper, p, p};
    }
    Real enter = lower;
    Real exit = upper;
    for (int i = 0; i < 3; i++)
    {
        if (dir[i] == 0)
        {
            bool outside = (origin[i] < lo[i] && !eq(origin[i], lo[i])) || (origin[i] > hi[i] && !eq(origin[i], hi[i]));
            if (outside)
            {
                return nullopt;
            }
            continue;
        }
        Real x = (lo[i] - origin[i]) / dir[i];
        Real y = (hi[i] - origin[i]) / dir[i];
        if (x > y)
        {
            swap(x, y);
        }
        enter = max(enter, x);
        exit = min(exit, y);
        if (enter > exit && !eq(enter, exit))
        {
            return nullopt;
        }
    }
    if (enter > exit)
    {
        Real t = (enter + exit) / 2;
        enter = exit = t;
    }
    return ParamRange3{enter, exit, p + v * enter, p + v * exit};
}

RayPtQuery3 rayPlane3(Ray3 r, Plane s)
{
    // r 是可退化射线，s 是非退化平面；返回闭平面的首次非负命中。
    assert(sgn(abs(s.n)) != 0); // 调试检查，可删。
    if (r.v.x == 0 && r.v.y == 0 && r.v.z == 0)
    {
        return {RayState3::Degenerate, {}};
    }
    Real d = dot(r.v, s.n);
    if (d == 0)
    {
        if (onPlane3(r.p, s))
        {
            return {RayState3::Hit, {0, r.p}};
        }
        return {RayState3::None, {}};
    }
    Real t = dot(s.p - r.p, s.n) / d;
    if (t < 0)
    {
        return {RayState3::None, {}};
    }
    return {RayState3::Hit, {t, r.p + r.v * t}};
}

RayIntQuery3 rayAABB3(Ray3 r, AABB3 b)
{
    // r 是可退化射线，b 是合法闭包围盒；返回盒内的非负参数区间。
    assert(b.lo.x <= b.hi.x && b.lo.y <= b.hi.y && b.lo.z <= b.hi.z); // 调试检查，可删。
    if (r.v.x == 0 && r.v.y == 0 && r.v.z == 0)
    {
        return {RayState3::Degenerate, {}};
    }
    auto hit = aabbRange3(r.p, r.v, b, 0, numeric_limits<Real>::infinity());
    if (!hit)
    {
        return {RayState3::None, {}};
    }
    return {RayState3::Hit, *hit};
}

optional<ParamRange3> segmentAABB3(P3 a, P3 b, AABB3 box)
{
    // a、b 是闭线段端点，box 是合法闭包围盒；返回参数域 [0,1] 内的命中区间，未命中返回空。
    P3 v = b - a;
    if (v.x == 0 && v.y == 0 && v.z == 0)
    {
        if (!in(box, a))
        {
            return nullopt;
        }
        return ParamRange3{0, 1, a, a};
    }
    return aabbRange3(a, v, box, 0, 1);
}

bool sphereAABB3(Sphere s, AABB3 b)
{
    // s 是非负半径闭球，b 是合法闭包围盒；返回两个闭集是否相交。
    assert(sgn(s.r) >= 0);                                            // 调试检查，可删。
    assert(b.lo.x <= b.hi.x && b.lo.y <= b.hi.y && b.lo.z <= b.hi.z); // 调试检查，可删。
    P3 q{clamp(s.o.x, b.lo.x, b.hi.x), clamp(s.o.y, b.lo.y, b.hi.y), clamp(s.o.z, b.lo.z, b.hi.z)};
    return in(b, q) && in(s, q);
}
