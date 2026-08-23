#pragma once

#include "11_2_001.hpp"

template <class T> vector<array<int, 3>> triangulate(const vector<Point<T>> &a)
{
    int n = (int)a.size();
    if (n < 3)
    {
        return {};
    }
    T s = area2(a);
    assert(sgn(s) != 0); // 调试检查，可删。
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    if (s < 0)
    {
        reverse(p.begin(), p.end());
    }
    bool changed = true;
    while (changed && p.size() > 3)
    {
        changed = false;
        int m = (int)p.size();
        for (int i = 0; i < m; i++)
        {
            int x = p[(i + m - 1) % m];
            int y = p[i];
            int z = p[(i + 1) % m];
            if (sgn(cross(a[x], a[y], a[z])) == 0)
            {
                p.erase(p.begin() + i);
                changed = true;
                break;
            }
        }
    }
    vector<array<int, 3>> ans;
    auto inside = [&](Point<T> q, Point<T> x, Point<T> y, Point<T> z)
    {
        return sgn(cross(x, y, q)) >= 0 && sgn(cross(y, z, q)) >= 0
            && sgn(cross(z, x, q)) >= 0;
    };
    while (p.size() > 3)
    {
        bool ok = false;
        int m = (int)p.size();
        for (int i = 0; i < m; i++)
        {
            int x = p[(i + m - 1) % m];
            int y = p[i];
            int z = p[(i + 1) % m];
            if (sgn(cross(a[x], a[y], a[z])) <= 0)
            {
                continue;
            }
            bool bad = false;
            for (int w : p)
            {
                if (w != x && w != y && w != z
                    && inside(a[w], a[x], a[y], a[z]))
                {
                    bad = true;
                    break;
                }
            }
            if (bad)
            {
                continue;
            }
            ans.push_back({x, y, z});
            p.erase(p.begin() + i);
            ok = true;
            break;
        }
        if (!ok)
        {
            return {};
        }
    }
    if (p.size() == 3)
    {
        ans.push_back({p[0], p[1], p[2]});
    }
    return ans;
}
