#pragma once

#include "11_1_003.hpp"

Real circlePoly(const vector<P> &a, P o, Real r)
{
    assert(sgn(r) >= 0);
    auto edge = [&](P a, P b)
    {
        P v = b - a;
        array<Real, 4> q{0, 1};
        int m = 2;
        Real aa = norm(v);
        Real bb = 2 * dot(a, v);
        Real cc = norm(a) - r * r;
        Real d = bb * bb - 4 * aa * cc;
        if (sgn(d) >= 0 && sgn(aa) != 0)
        {
            d = sqrtl(max(Real(0), d));
            for (Real t : {(-bb - d) / (2 * aa), (-bb + d) / (2 * aa)})
            {
                if (sgn(t) > 0 && sgn(t - 1) < 0)
                {
                    q[m++] = t;
                }
            }
        }
        for (int i = 1; i < m; i++)
        {
            for (int j = i; j > 0 && q[j] < q[j - 1]; j--)
            {
                swap(q[j], q[j - 1]);
            }
        }
        Real ans = 0;
        for (int i = 0; i + 1 < m; i++)
        {
            P x = a + v * q[i];
            P y = a + v * q[i + 1];
            P z = (x + y) / 2;
            if (sgn(norm(z) - r * r) <= 0)
            {
                ans += cross(x, y) / 2;
            }
            else
            {
                ans += atan2l(cross(x, y), dot(x, y)) * r * r / 2;
            }
        }
        return ans;
    };
    Real ans = 0;
    int n = (int)a.size();
    for (int i = 0; i < n; i++)
    {
        ans += edge(a[i] - o, a[(i + 1) % n] - o);
    }
    return fabsl(ans);
}
