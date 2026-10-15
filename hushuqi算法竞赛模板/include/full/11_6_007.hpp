#pragma once

#include "11_6_001.hpp"

struct Sphere
{
    P3 o;
    Real r;
};

bool in(Sphere s, P3 p)
{
    assert(sgn(s.r) >= 0); // 调试检查，可删。
    return sgn(abs(p - s.o) - s.r) <= 0;
}

optional<Sphere> sphere4(P3 a, P3 b, P3 c, P3 d)
{
    P3 u = b - a;
    P3 v = c - a;
    P3 w = d - a;
    Real det = dot(u, cross(v, w));
    if (fabsl(det) <= eps * max(Real(1), abs(u) * abs(v) * abs(w)))
    {
        return nullopt;
    }
    P3 o = a + (cross(v, w) * norm(u) + cross(w, u) * norm(v)
                + cross(u, v) * norm(w)) / (2 * det);
    return Sphere{o, abs(o - a)};
}

optional<Sphere> sphere3(P3 a, P3 b, P3 c)
{
    P3 u = b - a;
    P3 v = c - a;
    P3 w = cross(u, v);
    Real z = 2 * norm(w);
    if (sgn(abs(w)) == 0)
    {
        return nullopt;
    }
    P3 o = a + (cross(w, u) * norm(v) + cross(v, w) * norm(u)) / z;
    return Sphere{o, abs(o - a)};
}

Sphere boundSphere(const vector<P3> &p)
{
    if (p.empty())
    {
        return {{}, 0};
    }
    Sphere ans{{}, numeric_limits<Real>::infinity()};
    int n = (int)p.size();
    auto add = [&](optional<Sphere> s)
    {
        if (!s || s->r >= ans.r)
        {
            return;
        }
        for (P3 x : p)
        {
            if (!in(*s, x))
            {
                return;
            }
        }
        ans = *s;
    };
    for (int i = 0; i < n; i++)
    {
        add(Sphere{p[i], 0});
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < i; j++)
        {
            add(Sphere{(p[i] + p[j]) / 2, abs(p[i] - p[j]) / 2});
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < i; j++)
        {
            for (int k = 0; k < j; k++)
            {
                add(sphere3(p[i], p[j], p[k]));
            }
        }
    }
    if (n == 4)
    {
        add(sphere4(p[0], p[1], p[2], p[3]));
    }
    return ans;
}

Sphere minSphere(vector<P3> p, unsigned seed = 712367)
{
    mt19937 rng(seed);
    shuffle(p.begin(), p.end(), rng);
    Sphere s{{}, -1};
    for (int i = 0; i < (int)p.size(); i++)
    {
        if (s.r >= 0 && in(s, p[i]))
        {
            continue;
        }
        s = {p[i], 0};
        for (int j = 0; j < i; j++)
        {
            if (in(s, p[j]))
            {
                continue;
            }
            s = boundSphere({p[i], p[j]});
            for (int k = 0; k < j; k++)
            {
                if (in(s, p[k]))
                {
                    continue;
                }
                s = boundSphere({p[i], p[j], p[k]});
                for (int l = 0; l < k; l++)
                {
                    if (!in(s, p[l]))
                    {
                        s = boundSphere({p[i], p[j], p[k], p[l]});
                    }
                }
            }
        }
    }
    if (p.empty())
    {
        s = {{}, 0};
    }
    return s;
}
