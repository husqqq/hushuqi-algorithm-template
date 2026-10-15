#pragma once

#include "11_4_001.hpp"

Real circleGap(C a, C b)
{
    assert(sgn(a.r) >= 0 && sgn(b.r) >= 0); // 调试检查，可删。
    Real d = abs(a.o - b.o);
    return max({Real(0), d - a.r - b.r, fabsl(a.r - b.r) - d});
}

optional<tuple<Real, int, int>> nearCircles(const vector<C> &a)
{
    if (a.size() < 2)
    {
        return nullopt;
    }
    tuple<Real, int, int> ans{numeric_limits<Real>::infinity(), -1, -1};
    for (int i = 0; i < (int)a.size(); i++)
    {
        for (int j = i + 1; j < (int)a.size(); j++)
        {
            Real d = circleGap(a[i], a[j]);
            if (d < get<0>(ans))
            {
                ans = {d, i, j};
            }
        }
    }
    return ans;
}
