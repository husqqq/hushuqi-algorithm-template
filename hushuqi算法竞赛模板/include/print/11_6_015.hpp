#pragma once

#include "11_6_007.hpp"
#include "11_6_010.hpp"

P3 reflection(Plane s, P3 p)
{
    // s 是非退化平面，p 是查询点；返回 p 关于 s 的镜像点。
    P3 q = projection(s, p);
    return q * 2 - p;
}

Real signedDih3(P3 edge, P3 na, P3 nb)
{
    // edge 是非零有向边，两法向的正交投影非零；返回绕 edge 按右手方向从 A 到 B 的 [-pi,pi] 角。
    assert(sgn(abs(edge)) != 0 && sgn(abs(na)) != 0 && sgn(abs(nb)) != 0); // 调试检查，可删。
    P3 e = unit(edge);
    P3 a = na - e * dot(na, e);
    P3 b = nb - e * dot(nb, e);
    assert(sgn(abs(a)) != 0 && sgn(abs(b)) != 0); // 调试检查，可删。
    return atan2l(dot(e, cross(a, b)), dot(a, b));
}

Real dihedral3(P3 edge, P3 na, P3 nb)
{
    // edge 是非零公共边，na、nb 是两面非零法向；返回范围 [0,pi] 的无向二面角。
    return fabsl(signedDih3(edge, na, nb));
}

P3 fromLatLon3(Real lat, Real lon, Real r = 1)
{
    // lat、lon 是纬度和经度弧度，r 是非负半径；返回球心在原点的球面点。
    const Real pi = acosl(-1);
    assert((lat > -pi / 2 || eq(lat, -pi / 2)) && (lat < pi / 2 || eq(lat, pi / 2)) &&
           r >= 0); // 调试检查，可删。
    Real z = cosl(lat);
    return {r * z * cosl(lon), r * z * sinl(lon), r * sinl(lat)};
}

pair<Real, Real> toLatLon3(P3 p)
{
    // p 是以原点为球心的非零球面点；返回纬度与范围 [-pi,pi] 的经度弧度。
    assert(p.x != 0 || p.y != 0 || p.z != 0); // 调试检查，可删。
    Real xy = hypotl(p.x, p.y);
    Real lon = xy == 0 ? 0 : atan2l(p.y, p.x);
    return {atan2l(p.z, xy), lon};
}

Real solidAngle3(P3 a, P3 b, P3 c)
{
    // a、b、c 是非零方向且两两不反向；返回三条短大圆弧围成的主值有向立体角。
    assert((a.x != 0 || a.y != 0 || a.z != 0) && (b.x != 0 || b.y != 0 || b.z != 0) &&
           (c.x != 0 || c.y != 0 || c.z != 0)); // 调试检查，可删。
    auto normalize = [&](P3 p)
    {
        Real len = abs(p);
        return P3{p.x / len, p.y / len, p.z / len};
    };
    a = normalize(a);
    b = normalize(b);
    c = normalize(c);
    assert(norm(a + b) != 0 && norm(b + c) != 0 && norm(c + a) != 0); // 调试检查，可删。
    Real y = dot(a, cross(b, c));
    Real x = 1 + dot(a, b) + dot(b, c) + dot(c, a);
    return 2 * atan2l(y, x);
}

Real spherePoly3(const vector<P3> &poly, Sphere s)
{
    // poly 是球面简单多边形顶点，s 是正半径球；返回 (-2pi*r^2,2pi*r^2] 内的主值有向面积。
    assert(s.r > 0); // 调试检查，可删。
    auto normalize = [&](P3 p)
    {
        Real len = abs(p);
        return P3{p.x / len, p.y / len, p.z / len};
    };
    vector<P3> dir;
    for (P3 p : poly)
    {
        P3 q = p - s.o;
        Real len = abs(q);
        assert(fabsl(len - s.r) <= eps * max(len, s.r)); // 调试检查，可删。
        P3 u = normalize(q);
        if (dir.empty() || norm(dir.back() - u) != 0)
        {
            dir.push_back(u);
        }
    }
    if (dir.size() > 1 && norm(dir.front() - dir.back()) == 0)
    {
        dir.pop_back();
    }
    if (dir.size() < 3)
    {
        return 0;
    }
    for (int i = 0; i < (int)dir.size(); i++)
    {
        int j = (i + 1) % dir.size();
        assert(norm(dir[i] + dir[j]) != 0); // 调试检查，可删。
    }
    Real turn = 0;
    int n = (int)dir.size();
    for (int i = 0; i < n; i++)
    {
        P3 pre = dir[(i + n - 1) % n];
        P3 cur = dir[i];
        P3 next = dir[(i + 1) % n];
        P3 ni = normalize(cross(pre, cur));
        P3 no = normalize(cross(cur, next));
        P3 vin = cross(ni, cur);
        P3 vout = cross(no, cur);
        turn += atan2l(dot(cur, cross(vin, vout)), dot(vin, vout));
    }
    const Real pi = acosl(-1);
    Real ans = turn > 0 ? 2 * pi - turn : -2 * pi - turn;
    if (turn == 0)
    {
        ans = 2 * pi;
    }
    while (ans <= -2 * pi)
    {
        ans += 4 * pi;
    }
    while (ans > 2 * pi)
    {
        ans -= 4 * pi;
    }
    return ans * s.r * s.r;
}
