#pragma once

#include "11_6_009.hpp"
#include "11_6_011.hpp"

struct Circle3
{
    P3 o;   // o 是空间圆圆心。
    P3 n;   // n 是圆所在平面的非零法向量。
    Real r; // r 是非负半径，0 表示单点圆。
};

enum class SphereRel3
{
    Separate,
    ExtTangent,
    Intersecting,
    IntTangent,
    Contained,
    Coincident
};

struct SpherePair3
{
    SphereRel3 relation; // relation 是两个球面的相对关系。
    optional<Circle3> circle; // circle 在相切或相交时保存交圆，相切时半径为 0。
};

struct RayPtsQuery3
{
    RayState3 state;     // state 表示未命中、命中或零方向退化。
    vector<ParamPoint3> hits; // hits 在命中时按非负参数递增保存球面交点。
};

vector<ParamPoint3> lineSphere3(L3 l, Sphere s)
{
    // l 是非退化直线，s 是非负半径球；返回按参数递增的 0 至 2 个球面交点。
    assert((l.v.x != 0 || l.v.y != 0 || l.v.z != 0) && s.r >= 0); // 调试检查，可删。
    Real vv = norm(l.v);
    Real middle = dot(s.o - l.p, l.v) / vv;
    P3 foot = l.p + l.v * middle;
    Real d2 = norm(foot - s.o);
    Real radius2 = s.r * s.r;
    Real tol = eps * max(d2, radius2);
    if (d2 > radius2 + tol)
    {
        return {};
    }
    Real chord2 = radius2 - d2;
    if (fabsl(chord2) <= tol)
    {
        return {{middle, foot}};
    }
    Real half = sqrtl(max(Real(0), chord2) / vv);
    Real x = middle - half;
    Real y = middle + half;
    return {{x, l.p + l.v * x}, {y, l.p + l.v * y}};
}

RayPtsQuery3 raySphere3(Ray3 r, Sphere s)
{
    // r 是可退化射线，s 是非负半径球；返回所有非负参数的球面交点。
    assert(s.r >= 0); // 调试检查，可删。
    if (r.v.x == 0 && r.v.y == 0 && r.v.z == 0)
    {
        return {RayState3::Degenerate, {}};
    }
    vector<ParamPoint3> out;
    for (ParamPoint3 hit : lineSphere3({r.p, r.v}, s))
    {
        if (hit.t < 0)
        {
            continue;
        }
        out.push_back(hit);
    }
    if (out.empty())
    {
        return {RayState3::None, {}};
    }
    return {RayState3::Hit, out};
}

optional<Circle3> planeSphere3(Plane p, Sphere s)
{
    // p 是非退化平面，s 是非负半径球；返回交圆，相离时返回空，相切时半径为 0。
    assert(sgn(abs(p.n)) != 0 && sgn(s.r) >= 0); // 调试检查，可删。
    Real h = dot(s.o - p.p, p.n) / abs(p.n);
    if (fabsl(h) > s.r && !eq(fabsl(h), s.r))
    {
        return nullopt;
    }
    P3 o = projection(p, s.o);
    Real r2 = max(Real(0), s.r * s.r - h * h);
    return Circle3{o, unit(p.n), sqrtl(r2)};
}

SpherePair3 spherePair3(Sphere a, Sphere b)
{
    // a、b 是两个非负半径球；返回球面关系及相切或相交时的交圆。
    assert(sgn(a.r) >= 0 && sgn(b.r) >= 0); // 调试检查，可删。
    Real d = abs(b.o - a.o);
    Real sum = a.r + b.r;
    Real diff = fabsl(a.r - b.r);
    if (eq(d, Real(0)) && eq(a.r, b.r))
    {
        return {SphereRel3::Coincident, nullopt};
    }
    if (d < diff && !eq(d, diff))
    {
        return {SphereRel3::Contained, nullopt};
    }
    SphereRel3 relation;
    if (eq(d, diff))
    {
        relation = SphereRel3::IntTangent;
    }
    else if (d > sum && !eq(d, sum))
    {
        return {SphereRel3::Separate, nullopt};
    }
    else if (eq(d, sum))
    {
        relation = SphereRel3::ExtTangent;
    }
    else
    {
        relation = SphereRel3::Intersecting;
    }
    P3 u = (b.o - a.o) / d;
    Real x = (d * d + a.r * a.r - b.r * b.r) / (2 * d);
    Real r2 = max(Real(0), a.r * a.r - x * x);
    return {relation, Circle3{a.o + u * x, u, sqrtl(r2)}};
}

vector<ParamPoint3> lineCircle3(L3 l, Circle3 c)
{
    // l 是非退化直线，c 是法向非零的空间圆；返回按直线参数递增的交点。
    assert((l.v.x != 0 || l.v.y != 0 || l.v.z != 0) && sgn(abs(c.n)) != 0 && c.r >= 0); // 调试检查，可删。
    Real d = dot(l.v, c.n);
    if (d != 0)
    {
        Real t = dot(c.o - l.p, c.n) / d;
        P3 p = l.p + l.v * t;
        Real d2 = norm(p - c.o);
        Real radius2 = c.r * c.r;
        if (fabsl(d2 - radius2) > eps * max(d2, radius2))
        {
            return {};
        }
        return {{t, p}};
    }
    if (sgn(dot(l.p - c.o, c.n) / abs(c.n)) != 0)
    {
        return {};
    }
    return lineSphere3(l, {c.o, c.r});
}

Real sphereLens3(Sphere a, Sphere b)
{
    // a、b 是两个非负半径闭球；返回二者公共部分体积。
    assert(sgn(a.r) >= 0 && sgn(b.r) >= 0); // 调试检查，可删。
    Real d = abs(b.o - a.o);
    Real sum = a.r + b.r;
    Real diff = fabsl(a.r - b.r);
    if (eq(d, Real(0)) || d < diff || eq(d, diff))
    {
        return sphereVolume(min(a.r, b.r), 3);
    }
    if (d > sum || eq(d, sum))
    {
        return 0;
    }
    const Real pi = acosl(-1);
    Real h = sum - d;
    Real q = d * d + 2 * d * sum - 3 * (a.r - b.r) * (a.r - b.r);
    return pi * h * h * q / (12 * d);
}

Real sphereArea(Real r)
{
    // r 是非负球半径；返回三维球面面积。
    assert(r >= 0); // 调试检查，可删。
    return 4 * acosl(-1) * r * r;
}

Real capArea(Real r, Real h)
{
    // r 是非负球半径，h 是范围 [0,2r] 的球冠高；返回球冠曲面面积。
    assert(r >= 0 && (h > 0 || eq(h, Real(0))) && (h < 2 * r || eq(h, 2 * r))); // 调试检查，可删。
    h = clamp(h, Real(0), 2 * r);
    return max(Real(0), 2 * acosl(-1) * r * h);
}

Real capVolume(Real r, Real h)
{
    // r 是非负球半径，h 是范围 [0,2r] 的球冠高；返回球冠体积。
    assert(r >= 0 && (h > 0 || eq(h, Real(0))) && (h < 2 * r || eq(h, 2 * r))); // 调试检查，可删。
    h = clamp(h, Real(0), 2 * r);
    return max(Real(0), acosl(-1) * h * h * (r - h / 3));
}

Real zoneArea(Real r, Real low, Real high)
{
    // r 是非负球半径，[low,high] 是相对球心轴坐标区间；返回球带曲面面积。
    assert(r >= 0 && (low > -r || eq(low, -r)) && (low < high || eq(low, high)) &&
           (high < r || eq(high, r))); // 调试检查，可删。
    low = clamp(low, -r, r);
    high = clamp(high, -r, r);
    if (low > high)
    {
        low = high = (low + high) / 2;
    }
    return max(Real(0), 2 * acosl(-1) * r * (high - low));
}

Real zoneVol3(Real r, Real low, Real high)
{
    // r 是非负球半径，[low,high] 是相对球心轴坐标区间；返回两截面间体积。
    assert(r >= 0 && (low > -r || eq(low, -r)) && (low < high || eq(low, high)) &&
           (high < r || eq(high, r))); // 调试检查，可删。
    low = clamp(low, -r, r);
    high = clamp(high, -r, r);
    if (low > high)
    {
        low = high = (low + high) / 2;
    }
    auto primitive = [&](Real z)
    {
        return r * r * z - z * z * z / 3;
    };
    return max(Real(0), acosl(-1) * (primitive(high) - primitive(low)));
}
