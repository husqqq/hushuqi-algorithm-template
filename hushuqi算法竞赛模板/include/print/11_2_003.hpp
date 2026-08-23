#pragma once

#include "11_2_002.hpp"

bool segInPoly(const vector<P> &a, P s, P t)
{
    if (!inPoly(a, s) || !inPoly(a, t))
    {
        return false;
    }
    vector<Real> q{0, 1};
    P v = t - s;
    Real z = norm(v);
    if (sgn(z) == 0)
    {
        return true;
    }
    int n = (int)a.size();
    for (int i = 0; i < n; i++)
    {
        P x = a[i];
        P y = a[(i + 1) % n];
        P w = y - x;
        Real d = cross(v, w);
        if (sgn(d) != 0)
        {
            Real u = cross(x - s, w) / d;
            Real k = cross(x - s, v) / d;
            if (sgn(u) >= 0 && sgn(u - 1) <= 0
                && sgn(k) >= 0 && sgn(k - 1) <= 0)
            {
                q.push_back(u);
            }
        }
        else if (sgn(cross(v, x - s)) == 0)
        {
            q.push_back(clamp(dot(x - s, v) / z, Real(0), Real(1)));
            q.push_back(clamp(dot(y - s, v) / z, Real(0), Real(1)));
        }
    }
    sort(q.begin(), q.end());
    q.erase(unique(q.begin(), q.end(), [](Real x, Real y)
                   {
                       return sgn(x - y) == 0;
                   }),
            q.end());
    for (int i = 0; i + 1 < (int)q.size(); i++)
    {
        P p = s + v * ((q[i] + q[i + 1]) / 2);
        if (!inPoly(a, p))
        {
            return false;
        }
    }
    return true;
}
