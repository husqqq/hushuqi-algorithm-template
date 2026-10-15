#pragma once

#include "11_4_001.hpp"

int fixedCover(const vector<P> &a, Real r)
{
    assert(sgn(r) >= 0); // 调试检查，可删。
    int ans = a.empty() ? 0 : 1;
    const Real pi = acosl(-1);
    for (int i = 0; i < (int)a.size(); i++)
    {
        vector<pair<Real, int>> e;
        int same = 1;
        for (int j = 0; j < (int)a.size(); j++)
        {
            if (i == j)
            {
                continue;
            }
            Real d = abs(a[j] - a[i]);
            if (sgn(d) == 0)
            {
                same++;
                continue;
            }
            if (sgn(d - 2 * r) > 0)
            {
                continue;
            }
            Real x = atan2l(a[j].y - a[i].y, a[j].x - a[i].x);
            Real y = acosl(clamp(d / (2 * r), Real(-1), Real(1)));
            Real l = x - y;
            Real h = x + y;
            while (l < 0)
            {
                l += 2 * pi;
                h += 2 * pi;
            }
            while (l >= 2 * pi)
            {
                l -= 2 * pi;
                h -= 2 * pi;
            }
            if (h <= 2 * pi)
            {
                e.push_back({l, 1});
                e.push_back({h, -1});
            }
            else
            {
                e.push_back({l, 1});
                e.push_back({2 * pi, -1});
                e.push_back({0, 1});
                e.push_back({h - 2 * pi, -1});
            }
        }
        sort(e.begin(), e.end(), [](auto x, auto y)
             {
                 return x.first != y.first ? x.first < y.first : x.second > y.second;
             });
        ans = max(ans, same);
        int cur = same;
        for (auto [x, v] : e)
        {
            cur += v;
            ans = max(ans, cur);
        }
    }
    return ans;
}
