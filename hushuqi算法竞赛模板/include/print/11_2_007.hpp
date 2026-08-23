#pragma once

#include "11_2_001.hpp"
#include "11_2_002.hpp"

vector<P> convexIsect(vector<P> a, const vector<P> &b)
{
    auto in = [&](P p, const vector<P> &q)
    {
        if (q.empty())
        {
            return false;
        }
        if (q.size() == 1)
        {
            return eq(p, q[0]);
        }
        if (q.size() == 2)
        {
            return onSeg(p, q[0], q[1]);
        }
        for (int i = 0; i < (int)q.size(); i++)
        {
            if (sgn(cross(q[i], q[(i + 1) % q.size()], p)) < 0)
            {
                return false;
            }
        }
        return true;
    };
    auto add = [&](vector<P> &q, P p)
    {
        for (P x : q)
        {
            if (eq(x, p))
            {
                return;
            }
        }
        q.push_back(p);
    };
    auto segSeg = [&](P x, P y, P u, P v)
    {
        vector<P> q;
        if (onSeg(x, u, v))
        {
            add(q, x);
        }
        if (onSeg(y, u, v))
        {
            add(q, y);
        }
        if (onSeg(u, x, y))
        {
            add(q, u);
        }
        if (onSeg(v, x, y))
        {
            add(q, v);
        }
        if (q.empty() && segIsect(x, y, u, v))
        {
            add(q, isect({x, y - x}, {u, v - u}));
        }
        return q;
    };
    auto segPoly = [&](P x, P y, const vector<P> &q)
    {
        if (q.empty())
        {
            return vector<P>{};
        }
        if (q.size() == 1)
        {
            return onSeg(q[0], x, y) ? vector<P>{q[0]} : vector<P>{};
        }
        if (q.size() == 2)
        {
            return segSeg(x, y, q[0], q[1]);
        }
        P d = y - x;
        Real lo = 0;
        Real hi = 1;
        for (int i = 0; i < (int)q.size(); i++)
        {
            P e = q[(i + 1) % q.size()] - q[i];
            Real c = cross(e, x - q[i]);
            Real k = cross(e, d);
            if (sgn(k) == 0)
            {
                if (sgn(c) < 0)
                {
                    return vector<P>{};
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
            return vector<P>{};
        }
        vector<P> r{x + d * lo};
        if (!eq(lo, hi))
        {
            r.push_back(x + d * hi);
        }
        return r;
    };
    auto clean = [&](vector<P> q)
    {
        vector<P> r;
        for (P p : q)
        {
            if (r.empty() || !eq(r.back(), p))
            {
                r.push_back(p);
            }
        }
        if (r.size() > 1 && eq(r.front(), r.back()))
        {
            r.pop_back();
        }
        if (r.size() > 2 && sgn(area(r)) == 0)
        {
            P x = *min_element(r.begin(), r.end());
            P y = *max_element(r.begin(), r.end());
            return eq(x, y) ? vector<P>{x} : vector<P>{x, y};
        }
        return r;
    };
    if (a.empty() || b.empty())
    {
        return {};
    }
    if (a.size() == 1)
    {
        return in(a[0], b) ? a : vector<P>{};
    }
    if (b.size() == 1)
    {
        return in(b[0], a) ? b : vector<P>{};
    }
    if (a.size() == 2)
    {
        return segPoly(a[0], a[1], b);
    }
    if (b.size() == 2)
    {
        return segPoly(b[0], b[1], a);
    }
    for (int i = 0; i < (int)b.size() && !a.empty(); i++)
    {
        P p = b[i];
        P q = b[(i + 1) % b.size()];
        vector<P> c;
        for (int j = 0; j < (int)a.size(); j++)
        {
            P x = a[j];
            P y = a[(j + 1) % a.size()];
            int sx = sgn(cross(p, q, x));
            int sy = sgn(cross(p, q, y));
            if (sx >= 0)
            {
                c.push_back(x);
            }
            if (sx * sy < 0)
            {
                c.push_back(isect({x, y - x}, {p, q - p}));
            }
        }
        a.swap(c);
    }
    return clean(a);
}

Real convexUnion(const vector<P> &a, const vector<P> &b)
{
    return area(a) + area(b) - area(convexIsect(a, b));
}
