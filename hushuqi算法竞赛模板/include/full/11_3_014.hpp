#pragma once

#include "11_1_003.hpp"

optional<pair<__int128, __int128>> triAreaRng2(const vector<Point<long long>> &a)
{
    // a 是整数点集；返回不同下标三点的最小、最大双倍面积，少于三个点时返回空。
    // 坐标绝对值不超过 1E9，以保证方向排序和面积乘积可由 __int128 表示。
    // 坐标差之间的叉积必须能由 __int128 表示。
    int n = (int)a.size();
    if (n < 3)
    {
        return nullopt;
    }
    vector<Point<long long>> p = a;
    sort(p.begin(), p.end());
    auto last = unique(p.begin(), p.end());
    bool dup = last != p.end();
    p.erase(last, p.end());
    if (p.size() < 3)
    {
        return pair<__int128, __int128>{0, 0};
    }
    int m = (int)p.size();
    struct Event
    {
        int u;      // 有向差向量的起点下标。
        int v;      // 有向差向量的终点下标。
        __int128 x; // 规范到上半平面后的横坐标差。
        __int128 y; // 规范到上半平面后的纵坐标差。
    };
    vector<Event> e;
    for (int i = 0; i < m; i++)
    {
        for (int j = i + 1; j < m; j++)
        {
            __int128 x = (__int128)p[j].x - p[i].x;
            __int128 y = (__int128)p[j].y - p[i].y;
            if (y < 0 || (y == 0 && x < 0))
            {
                x = -x;
                y = -y;
            }
            e.push_back({i, j, x, y});
        }
    }
    sort(e.begin(),
         e.end(),
         [](const Event &u, const Event &v)
         {
             __int128 c = u.x * v.y - u.y * v.x;
             if (c != 0)
             {
                 return c > 0;
             }
             return pair(u.u, u.v) < pair(v.u, v.v);
         });
    vector<int> ord(m), pos(m);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(),
         ord.end(),
         [&](int x, int y)
         {
             return tuple(p[x].y, p[x].x, x) < tuple(p[y].y, p[y].x, y);
         });
    for (int i = 0; i < m; i++)
    {
        pos[ord[i]] = i;
    }
    auto triArea = [&](int i, int j, int k)
    {
        __int128 x1 = (__int128)p[j].x - p[i].x;
        __int128 y1 = (__int128)p[j].y - p[i].y;
        __int128 x2 = (__int128)p[k].x - p[i].x;
        __int128 y2 = (__int128)p[k].y - p[i].y;
        __int128 s = x1 * y2 - y1 * x2;
        return s < 0 ? -s : s;
    };
    __int128 mn = 0;
    __int128 mx = 0;
    bool hasMin = dup;
    vector<int> fa(m, -1);
    auto find = [&](auto &&self, int x) -> int
    {
        return fa[x] == x ? x : fa[x] = self(self, fa[x]);
    };
    for (int l = 0; l < (int)e.size();)
    {
        int r = l + 1;
        while (r < (int)e.size() && e[l].x * e[r].y == e[l].y * e[r].x)
        {
            r++;
        }
        vector<int> touched;
        for (int i = l; i < r; i++)
        {
            for (int x : {e[i].u, e[i].v})
            {
                if (fa[x] == -1)
                {
                    fa[x] = x;
                    touched.push_back(x);
                }
            }
            int x = find(find, e[i].u);
            int y = find(find, e[i].v);
            if (x != y)
            {
                fa[x] = y;
            }
        }
        map<int, vector<int>> block;
        for (int x : touched)
        {
            block[find(find, x)].push_back(x);
        }
        for (int i = l; i < r; i++)
        {
            for (int x : {ord.front(), ord.back()})
            {
                if (x != e[i].u && x != e[i].v)
                {
                    mx = max(mx, triArea(e[i].u, e[i].v, x));
                }
            }
        }
        for (auto &[root, v] : block)
        {
            int x = m;
            int y = -1;
            for (int z : v)
            {
                x = min(x, pos[z]);
                y = max(y, pos[z]);
            }
            assert(y - x + 1 == (int)v.size()); // 调试检查，可删。
            if (v.size() >= 3)
            {
                mn = 0;
                hasMin = true;
            }
            else if (!hasMin || mn != 0)
            {
                int u = v[0];
                int z = v[1];
                for (int k : {x - 1, y + 1})
                {
                    if (0 <= k && k < m)
                    {
                        __int128 cur = triArea(u, z, ord[k]);
                        if (!hasMin || cur < mn)
                        {
                            mn = cur;
                            hasMin = true;
                        }
                    }
                }
            }
            reverse(ord.begin() + x, ord.begin() + y + 1);
            for (int k = x; k <= y; k++)
            {
                pos[ord[k]] = k;
            }
        }
        for (int x : touched)
        {
            fa[x] = -1;
        }
        l = r;
    }
    assert(hasMin); // 调试检查，可删。
    return pair<__int128, __int128>{mn, mx};
}

optional<pair<Real, Real>> triAreaRng2(const vector<P> &a)
{
    // a 是浮点点集；返回不同下标三点的最小、最大双倍面积，少于三个点时返回空。
    int n = (int)a.size();
    if (n < 3)
    {
        return nullopt;
    }
    Real mn = numeric_limits<Real>::infinity();
    Real mx = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            for (int k = j + 1; k < n; k++)
            {
                Real s = fabsl(cross(a[i], a[j], a[k]));
                if (sgn(s) == 0)
                {
                    s = 0;
                }
                mn = min(mn, s);
                mx = max(mx, s);
            }
        }
    }
    return pair<Real, Real>{mn, mx};
}
