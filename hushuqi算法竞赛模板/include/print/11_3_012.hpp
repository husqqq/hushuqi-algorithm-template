#pragma once
#include "11_1_002.hpp"
#include "11_1_003.hpp"

P fermatPoint(P a, P b, P c)
{
    // a、b、c 表示非共线三角形顶点；返回到三顶点距离和最小的费马点。
    assert(sgn(cross(a, b, c)) != 0); // 调试检查，可删。
    const Real pi = acosl(-1);
    auto ang = [&](P x, P y, P z)
    {
        // x 表示角顶点，y、z 表示另外两点；返回夹角弧度。
        auto u = y - x;
        auto v = z - x;
        auto q = dot(u, v) / abs(u) / abs(v);
        return acosl(clamp(q, (Real)-1, (Real)1));
    };
    auto A = ang(a, b, c);
    auto B = ang(b, c, a);
    auto C = ang(c, a, b);
    if (sgn(A - 2 * pi / 3) >= 0)
    {
        return a;
    }
    if (sgn(B - 2 * pi / 3) >= 0)
    {
        return b;
    }
    if (sgn(C - 2 * pi / 3) >= 0)
    {
        return c;
    }
    auto x = abs(b - c) / sinl(A + pi / 3);
    auto y = abs(c - a) / sinl(B + pi / 3);
    auto z = abs(a - b) / sinl(C + pi / 3);
    return (a * x + b * y + c * z) / (x + y + z);
}
