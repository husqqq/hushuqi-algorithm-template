#pragma once

#include "11_6_011.hpp"

enum class TriPlaneType
{
    Empty,
    Point,
    Segment,
    Triangle
};

struct TriPlaneRes3
{
    TriPlaneType type; // type 是空、点、线段或整三角形四类交集。
    vector<P3> points;       // points 按 type 保存 0、1、2 或 3 点；整三角形保持输入序，线段点按三条边枚举序。
};

struct TriClose3
{
    P3 point;                   // point 是三角形闭集上的最近点。
    array<Real, 3> barycentric; // barycentric 依次是三个顶点的重心坐标。
    Real distance;              // distance 是查询点到三角形的距离。
};

bool inTri3(P3 p, P3 a, P3 b, P3 c)
{
    // p 是查询点，a、b、c 是非退化三角形；返回 p 是否在三角形闭集内。
    P3 n = cross(b - a, c - a);
    assert(sgn(abs(n)) != 0); // 调试检查，可删。
    if (!onPlane3(p, {a, n}))
    {
        return false;
    }
    Real x = dot(cross(b - a, p - a), n) / abs(b - a) / abs(n);
    Real y = dot(cross(c - b, p - b), n) / abs(c - b) / abs(n);
    Real z = dot(cross(a - c, p - c), n) / abs(a - c) / abs(n);
    return sgn(x) >= 0 && sgn(y) >= 0 && sgn(z) >= 0;
}

TriClose3 pointTri3(P3 p, P3 a, P3 b, P3 c)
{
    // p 是查询点，a、b、c 是非退化三角形；返回最近点、重心坐标和距离。
    P3 u = b - a;
    P3 v = c - a;
    P3 n = cross(u, v);
    assert(sgn(abs(n)) != 0); // 调试检查，可删。
    P3 q = projection(Plane{a, n}, p);
    if (inTri3(q, a, b, c))
    {
        P3 w = q - a;
        Real uu = norm(u);
        Real uv = dot(u, v);
        Real vv = norm(v);
        Real uw = dot(u, w);
        Real vw = dot(v, w);
        Real d = uu * vv - uv * uv;
        Real y = (vv * uw - uv * vw) / d;
        Real z = (uu * vw - uv * uw) / d;
        return {q, {1 - y - z, y, z}, abs(p - q)};
    }
    PointSeg3 ab = pointSeg3(p, a, b);
    PointSeg3 bc = pointSeg3(p, b, c);
    PointSeg3 ca = pointSeg3(p, c, a);
    TriClose3 ans{ab.point, {1 - ab.t, ab.t, 0}, ab.distance};
    if (bc.distance < ans.distance)
    {
        ans = {bc.point, {0, 1 - bc.t, bc.t}, bc.distance};
    }
    if (ca.distance < ans.distance)
    {
        ans = {ca.point, {ca.t, 0, 1 - ca.t}, ca.distance};
    }
    return ans;
}

TriPlaneRes3 triPlane3(P3 a, P3 b, P3 c, Plane s)
{
    // a、b、c 是非退化三角形，s 是非退化平面；返回完整交集分类与顶点。
    assert(sgn(abs(cross(b - a, c - a))) != 0 && sgn(abs(s.n)) != 0); // 调试检查，可删。
    array<P3, 3> p{a, b, c};
    array<Real, 3> d;
    array<int, 3> side;
    for (int i = 0; i < 3; i++)
    {
        d[i] = dot(p[i] - s.p, s.n) / abs(s.n);
        side[i] = sgn(d[i]);
    }
    if (side[0] == 0 && side[1] == 0 && side[2] == 0)
    {
        return {TriPlaneType::Triangle, {a, b, c}};
    }
    vector<P3> out;
    auto add = [&](P3 q)
    {
        for (P3 x : out)
        {
            if (sgn(abs(x - q)) == 0)
            {
                return;
            }
        }
        out.push_back(q);
    };
    for (int i = 0; i < 3; i++)
    {
        int j = (i + 1) % 3;
        if (side[i] == 0)
        {
            add(p[i]);
        }
        if (side[i] * side[j] < 0)
        {
            add(p[i] + (p[j] - p[i]) * (d[i] / (d[i] - d[j])));
        }
    }
    if (out.empty())
    {
        return {TriPlaneType::Empty, {}};
    }
    if (out.size() == 1)
    {
        return {TriPlaneType::Point, out};
    }
    assert(out.size() == 2); // 调试检查，可删。
    return {TriPlaneType::Segment, out};
}

vector<P3> clipPolygon3(const vector<P3> &poly, Plane s)
{
    // poly 是有序共面凸多边形，s 是非退化裁剪平面；返回法向正侧闭半空间内的顶点序列。
    assert(sgn(abs(s.n)) != 0); // 调试检查，可删。
    if (poly.empty())
    {
        return {};
    }
    vector<P3> out;
    int n = (int)poly.size();
    for (int i = 0; i < n; i++)
    {
        P3 a = poly[i];
        P3 b = poly[(i + 1) % n];
        Real x = dot(a - s.p, s.n) / abs(s.n);
        Real y = dot(b - s.p, s.n) / abs(s.n);
        int sx = sgn(x);
        int sy = sgn(y);
        if (sx >= 0)
        {
            out.push_back(a);
        }
        if (sx * sy < 0)
        {
            out.push_back(a + (b - a) * (x / (x - y)));
        }
    }
    vector<P3> clean;
    for (P3 p : out)
    {
        if (clean.empty() || sgn(abs(clean.back() - p)) != 0)
        {
            clean.push_back(p);
        }
    }
    if (clean.size() > 1 && sgn(abs(clean.front() - clean.back())) == 0)
    {
        clean.pop_back();
    }
    return clean;
}

Real triArea3(P3 a, P3 b, P3 c)
{
    // a、b、c 是三角形顶点；返回无符号面积，退化时返回 0。
    return abs(cross(b - a, c - a)) / 2;
}

Real polygonArea3(const vector<P3> &poly)
{
    // poly 是顶点有序的共面简单多边形；返回无符号面积，少于三个点时返回 0。
    if (poly.size() < 3)
    {
        return 0;
    }
    P3 sum{};
    for (int i = 1; i + 1 < (int)poly.size(); i++)
    {
        sum = sum + cross(poly[i] - poly[0], poly[i + 1] - poly[0]);
    }
    return abs(sum) / 2;
}

RayPtQuery3 rayTriangle3(Ray3 r, P3 a, P3 b, P3 c)
{
    // r 是可退化射线，a、b、c 是非退化三角形；返回三角形闭集的首次命中。
    P3 n = cross(b - a, c - a);
    assert(sgn(abs(n)) != 0); // 调试检查，可删。
    if (r.v.x == 0 && r.v.y == 0 && r.v.z == 0)
    {
        return {RayState3::Degenerate, {}};
    }
    Real d = dot(r.v, n);
    if (d != 0)
    {
        Real t = dot(a - r.p, n) / d;
        if (t < 0)
        {
            return {RayState3::None, {}};
        }
        P3 p = r.p + r.v * t;
        if (!inTri3(p, a, b, c))
        {
            return {RayState3::None, {}};
        }
        return {RayState3::Hit, {t, p}};
    }
    if (!onPlane3(r.p, {a, n}))
    {
        return {RayState3::None, {}};
    }
    array<P3, 3> p{a, b, c};
    Real enter = 0;
    Real exit = numeric_limits<Real>::infinity();
    for (int i = 0; i < 3; i++)
    {
        int j = (i + 1) % 3;
        P3 edge = p[j] - p[i];
        Real scale = abs(edge) * abs(n);
        Real x = dot(cross(edge, r.p - p[i]), n) / scale;
        Real y = dot(cross(edge, r.v), n) / scale;
        if (y == 0)
        {
            if (sgn(x) < 0)
            {
                return {RayState3::None, {}};
            }
            continue;
        }
        Real t = -x / y;
        if (y > 0)
        {
            enter = max(enter, t);
        }
        else
        {
            exit = min(exit, t);
        }
        if (enter > exit && !eq(enter, exit))
        {
            return {RayState3::None, {}};
        }
    }
    return {RayState3::Hit, {enter, r.p + r.v * enter}};
}
