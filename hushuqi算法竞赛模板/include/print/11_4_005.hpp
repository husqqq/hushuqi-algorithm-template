#pragma once

#include "11_4_001.hpp"

C circle(P a, P b)
{
    // a、b 是直径端点；返回对应的圆。
    P o = (a + b) / 2;
    return {o, abs(a - o)};
}

C circle(P a, P b, P c)
{
    // a、b、c 是三个点；共线时返回覆盖三点的最小圆。
    P u = b - a;
    P v = c - a;
    Real d = 2 * cross(u, v);
    if (sgn(d) == 0)
    {
        if (norm(a - b) >= norm(a - c) && norm(a - b) >= norm(b - c))
        {
            return circle(a, b);
        }
        if (norm(a - c) >= norm(b - c))
        {
            return circle(a, c);
        }
        return circle(b, c);
    }
    P o = a + P{norm(u) * v.y - norm(v) * u.y, u.x * norm(v) - v.x * norm(u)} / d;
    return {o, abs(a - o)};
}

optional<C> incircle(P a, P b, P c)
{
    // a、b、c 是三角形顶点；返回内切圆，三点共线或有重复点时返回空。
    Real s = fabsl(cross(b - a, c - a));
    if (sgn(s) == 0)
    {
        return nullopt;
    }
    Real x = abs(b - c);
    Real y = abs(c - a);
    Real z = abs(a - b);
    Real sum = x + y + z;
    P o = (a * x + b * y + c * z) / sum;
    return C{o, s / sum};
}
