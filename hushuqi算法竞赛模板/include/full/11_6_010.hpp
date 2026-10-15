#pragma once

#include "11_6_002.hpp"
#include "11_6_003.hpp"
#include "11_6_004.hpp"

struct PointSeg3
{
    P3 point;      // point 是线段上的最近点。
    Real t;        // t 满足 point=a+(b-a)*t，范围为 [0,1]。
    Real distance; // distance 是查询点到线段的距离。
};

struct ClosestPair3
{
    P3 onA;        // onA 是第一个对象上的最近点。
    P3 onB;        // onB 是第二个对象上的最近点。
    Real tA;       // tA 是 onA 在第一个对象参数式中的参数。
    Real tB;       // tB 是 onB 在第二个对象参数式中的参数。
    Real distance; // distance 是两个对象之间的距离。
};

enum class LineRel3
{
    Intersecting,
    Skew,
    Parallel,
    Coincident,
    Degenerate
};

struct LineClosest3
{
    LineRel3 relation;         // relation 是两条直线的空间关系。
    Real distance;                  // distance 是两条直线的最短距离，所有关系下都有效。
    optional<ClosestPair3> closest; // closest 在相交、异面或退化点线关系下保存唯一最近点对。
};

P3 projection(L3 l, P3 p)
{
    // l 是非退化直线，p 是查询点；返回 p 在 l 上的正交投影。
    assert(norm(l.v) != 0); // 调试检查，可删。
    return l.p + l.v * (dot(p - l.p, l.v) / norm(l.v));
}

P3 projection(Plane s, P3 p)
{
    // s 是非退化平面，p 是查询点；返回 p 在 s 上的正交投影。
    assert(norm(s.n) != 0); // 调试检查，可删。
    return p - s.n * (dot(p - s.p, s.n) / norm(s.n));
}

PointSeg3 pointSeg3(P3 p, P3 a, P3 b)
{
    // p 是查询点，a、b 是闭线段端点；返回最近点、线段参数和距离。
    P3 v = b - a;
    Real t = 0;
    if (norm(v) != 0)
    {
        t = clamp(dot(p - a, v) / norm(v), Real(0), Real(1));
    }
    P3 q = a + v * t;
    return {q, t, abs(p - q)};
}

LineClosest3 lineLine3(L3 a, L3 b)
{
    // a、b 是两条可退化直线；返回关系、距离以及存在唯一解时的最近点对。
    Real aa = norm(a.v);
    Real bb = norm(b.v);
    if (aa == 0 && bb == 0)
    {
        Real distance = abs(a.p - b.p);
        return {LineRel3::Degenerate, distance, ClosestPair3{a.p, b.p, 0, 0, distance}};
    }
    if (aa == 0)
    {
        Real t = dot(a.p - b.p, b.v) / bb;
        P3 q = b.p + b.v * t;
        Real distance = abs(a.p - q);
        return {LineRel3::Degenerate, distance, ClosestPair3{a.p, q, 0, t, distance}};
    }
    if (bb == 0)
    {
        Real s = dot(b.p - a.p, a.v) / aa;
        P3 q = a.p + a.v * s;
        Real distance = abs(q - b.p);
        return {LineRel3::Degenerate, distance, ClosestPair3{q, b.p, s, 0, distance}};
    }
    P3 n = cross(a.v, b.v);
    Real d = norm(n);
    if (d == 0)
    {
        LineRel3 relation = onLine3(a.p, b) ? LineRel3::Coincident : LineRel3::Parallel;
        Real distance = relation == LineRel3::Coincident ? 0 : dis(a.p, b);
        return {relation, distance, nullopt};
    }
    P3 w = b.p - a.p;
    Real s = dot(cross(w, b.v), n) / d;
    Real t = dot(cross(w, a.v), n) / d;
    P3 x = a.p + a.v * s;
    P3 y = b.p + b.v * t;
    Real distance = abs(x - y);
    LineRel3 relation = sgn(distance) == 0 ? LineRel3::Intersecting : LineRel3::Skew;
    return {relation, distance, ClosestPair3{x, y, s, t, distance}};
}

ClosestPair3 segSeg3(P3 a, P3 b, P3 c, P3 d)
{
    // a、b 与 c、d 是两条闭线段端点；返回一组最近点、两段参数和距离。
    P3 u = b - a;
    P3 v = d - c;
    P3 w = a - c;
    Real aa = norm(u);
    Real ab = dot(u, v);
    Real bb = norm(v);
    Real aw = dot(u, w);
    Real bw = dot(v, w);
    Real s = 0;
    Real t = 0;
    if (aa == 0 && bb == 0)
    {
        s = t = 0;
    }
    else if (aa == 0)
    {
        t = clamp(bw / bb, Real(0), Real(1));
    }
    else if (bb == 0)
    {
        s = clamp(-aw / aa, Real(0), Real(1));
    }
    else
    {
        P3 n = cross(u, v);
        Real det = norm(n);
        if (det != 0)
        {
            s = clamp(dot(cross(c - a, v), n) / det, Real(0), Real(1));
        }
        t = (ab * s + bw) / bb;
        if (t < 0)
        {
            t = 0;
            s = clamp(-aw / aa, Real(0), Real(1));
        }
        else if (t > 1)
        {
            t = 1;
            s = clamp((ab - aw) / aa, Real(0), Real(1));
        }
    }
    P3 x = a + u * s;
    P3 y = c + v * t;
    ClosestPair3 ans{x, y, s, t, abs(x - y)};
    auto take = [&](P3 p, P3 q, Real ta, Real tb)
    {
        Real distance = abs(p - q);
        if (distance < ans.distance)
        {
            ans = {p, q, ta, tb, distance};
        }
    };
    PointSeg3 acd = pointSeg3(a, c, d);
    PointSeg3 bcd = pointSeg3(b, c, d);
    PointSeg3 cab = pointSeg3(c, a, b);
    PointSeg3 dab = pointSeg3(d, a, b);
    take(a, acd.point, 0, acd.t);
    take(b, bcd.point, 1, bcd.t);
    take(cab.point, c, cab.t, 0);
    take(dab.point, d, dab.t, 1);
    return ans;
}

Real angle(L3 a, L3 b)
{
    // a、b 是两条非退化无向直线；返回范围 [0,pi/2] 的较小夹角弧度。
    assert(norm(a.v) != 0 && norm(b.v) != 0); // 调试检查，可删。
    Real x = fabsl(dot(a.v, b.v) / abs(a.v) / abs(b.v));
    return acosl(clamp(x, Real(0), Real(1)));
}

Real angle(L3 a, Plane b)
{
    // a 是非退化直线，b 是非退化平面；返回范围 [0,pi/2] 的夹角弧度。
    assert(norm(a.v) != 0 && norm(b.n) != 0); // 调试检查，可删。
    Real x = fabsl(dot(a.v, b.n) / abs(a.v) / abs(b.n));
    return asinl(clamp(x, Real(0), Real(1)));
}

Real angle(Plane a, Plane b)
{
    // a、b 是两个非退化无向平面；返回范围 [0,pi/2] 的较小夹角弧度。
    assert(norm(a.n) != 0 && norm(b.n) != 0); // 调试检查，可删。
    Real x = fabsl(dot(a.n, b.n) / abs(a.n) / abs(b.n));
    return acosl(clamp(x, Real(0), Real(1)));
}

optional<P3> planesMeet3(Plane a, Plane b, Plane c)
{
    // a、b、c 是三个非退化平面；返回唯一公共点，不唯一或不存在时返回空。
    assert(norm(a.n) != 0 && norm(b.n) != 0 && norm(c.n) != 0); // 调试检查，可删。
    Real det = dot(a.n, cross(b.n, c.n));
    if (det == 0)
    {
        return nullopt;
    }
    Real x = dot(a.n, a.p);
    Real y = dot(b.n, b.p);
    Real z = dot(c.n, c.p);
    P3 num = cross(b.n, c.n) * x + cross(c.n, a.n) * y + cross(a.n, b.n) * z;
    return num * (1 / det);
}
