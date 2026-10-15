#pragma once

#include "11_1_007.hpp"
#include "11_2_001.hpp"

struct HP
{
    P p;
    P v;
    Real a;

    HP(P p, P q) : p(p), v(q - p), a(atan2l(v.y, v.x))
    {
        assert(!eq(p, q)); // 调试检查，可删。
        if (a < 0)
        {
            a += 2 * acosl(-1);
        }
    }

    bool out(P q) const
    {
        return sgn(cross(v, q - p)) < 0;
    }
};

optional<vector<P>> halfPlane(vector<HP> a)
{
    auto feasible = [&](vector<HP> q) -> optional<P>
    {
        mt19937 rng(0);
        shuffle(q.begin(), q.end(), rng);
        P x{};
        const Real inf = numeric_limits<Real>::infinity();
        for (int i = 0; i < (int)q.size(); i++)
        {
            if (!q[i].out(x))
            {
                continue;
            }
            Real lo = -inf;
            Real hi = inf;
            for (int j = 0; j < i; j++)
            {
                Real c = cross(q[j].v, q[i].p - q[j].p);
                Real k = cross(q[j].v, q[i].v);
                if (sgn(k) == 0)
                {
                    if (sgn(c) < 0)
                    {
                        return nullopt;
                    }
                    continue;
                }
                Real t = -c / k;
                if (k > 0)
                {
                    lo = max(lo, t);
                }
                else
                {
                    hi = min(hi, t);
                }
            }
            if (sgn(lo - hi) > 0)
            {
                return nullopt;
            }
            Real t = 0;
            if (isfinite(lo))
            {
                t = lo;
            }
            else if (isfinite(hi))
            {
                t = hi;
            }
            x = q[i].p + q[i].v * t;
        }
        return x;
    };
    if (!feasible(a))
    {
        return vector<P>{};
    }
    if (a.empty())
    {
        return nullopt;
    }
    vector<Real> ang;
    for (const HP &h : a)
    {
        ang.push_back(h.a);
    }
    sort(ang.begin(), ang.end());
    Real pi = acosl(-1);
    for (int i = 0; i < (int)ang.size(); i++)
    {
        Real x = ang[i];
        Real y = i + 1 < (int)ang.size() ? ang[i + 1] : ang[0] + 2 * pi;
        if (sgn(y - x - pi) >= 0)
        {
            return nullopt;
        }
    }
    sort(a.begin(), a.end(), [](const HP &x, const HP &y)
         {
             if (x.a != y.a)
             {
                 return x.a < y.a;
             }
             return cross(x.v, y.p - x.p) < 0;
         });
    vector<HP> b;
    for (HP h : a)
    {
        if (!b.empty() && parallel({b.back().p, b.back().v}, {h.p, h.v})
            && sgn(dot(b.back().v, h.v)) > 0)
        {
            if (h.out(b.back().p))
            {
                b.back() = h;
            }
            continue;
        }
        b.push_back(h);
    }
    if (b.size() > 1
        && parallel({b.front().p, b.front().v}, {b.back().p, b.back().v})
        && sgn(dot(b.front().v, b.back().v)) > 0)
    {
        if (b.front().out(b.back().p))
        {
            b.back() = b.front();
        }
        b.erase(b.begin());
    }
    deque<HP> q;
    for (HP h : b)
    {
        while (q.size() > 1
               && h.out(isect({q[q.size() - 2].p, q[q.size() - 2].v},
                              {q.back().p, q.back().v})))
        {
            q.pop_back();
        }
        while (q.size() > 1
               && h.out(isect({q[0].p, q[0].v}, {q[1].p, q[1].v})))
        {
            q.pop_front();
        }
        q.push_back(h);
    }
    while (q.size() > 2
           && q.front().out(isect({q[q.size() - 2].p, q[q.size() - 2].v},
                                  {q.back().p, q.back().v})))
    {
        q.pop_back();
    }
    while (q.size() > 2
           && q.back().out(isect({q[0].p, q[0].v}, {q[1].p, q[1].v})))
    {
        q.pop_front();
    }
    if (q.size() < 3)
    {
        return vector<P>{};
    }
    vector<P> p;
    for (int i = 0; i < (int)q.size(); i++)
    {
        const HP &x = q[i];
        const HP &y = q[(i + 1) % q.size()];
        assert(!parallel({x.p, x.v}, {y.p, y.v})); // 调试检查，可删。
        P z = isect({x.p, x.v}, {y.p, y.v});
        if (p.empty() || !eq(p.back(), z))
        {
            p.push_back(z);
        }
    }
    if (p.size() > 1 && eq(p.front(), p.back()))
    {
        p.pop_back();
    }
    if (p.size() > 2 && sgn(area(p)) == 0)
    {
        P x = *min_element(p.begin(), p.end());
        P y = *max_element(p.begin(), p.end());
        return eq(x, y) ? vector<P>{x} : vector<P>{x, y};
    }
    return p;
}
