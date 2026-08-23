#pragma once

#include "11_4_001.hpp"

vector<P> tangents(C c, P p)
{
    assert(sgn(c.r) >= 0); // 调试检查，可删。
    P v = p - c.o;
    Real d = norm(v);
    Real z = d - c.r * c.r;
    if (sgn(d) == 0)
    {
        return {};
    }
    if (sgn(c.r) == 0)
    {
        return {c.o};
    }
    if (sgn(z) < 0)
    {
        return {};
    }
    P q = c.o + v * (c.r * c.r / d);
    if (sgn(z) == 0)
    {
        return {q};
    }
    P w = rot(v) * (c.r * sqrtl(z) / d);
    return {q + w, q - w};
}

vector<pair<P, P>> tangents(C a, C b)
{
    assert(sgn(a.r) >= 0 && sgn(b.r) >= 0); // 调试检查，可删。
    if (sgn(a.r) == 0)
    {
        vector<pair<P, P>> ans;
        for (P q : tangents(b, a.o))
        {
            ans.push_back({a.o, q});
        }
        return ans;
    }
    if (sgn(b.r) == 0)
    {
        vector<pair<P, P>> ans;
        for (P q : tangents(a, b.o))
        {
            ans.push_back({q, b.o});
        }
        return ans;
    }
    P d = b.o - a.o;
    Real z = norm(d);
    if (sgn(z) == 0)
    {
        return {};
    }
    vector<pair<P, P>> ans;
    for (int s : {-1, 1})
    {
        Real r = a.r - s * b.r;
        Real h2 = z - r * r;
        if (sgn(h2) < 0)
        {
            continue;
        }
        for (int t : {-1, 1})
        {
            if (sgn(h2) == 0 && t == 1)
            {
                continue;
            }
            P v = (d * r + rot(d) * (sqrtl(max(Real(0), h2)) * t)) / z;
            ans.push_back({a.o + v * a.r, b.o + v * (s * b.r)});
        }
    }
    return ans;
}
