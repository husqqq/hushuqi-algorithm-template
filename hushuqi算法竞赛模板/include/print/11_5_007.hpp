#pragma once

#include "11_1_009.hpp"

__int128 swCross(Point<long long> a, Point<long long> b, Point<long long> c)
{
    // a、b 确定有向直线，c 是查询点；返回使用宽整数计算的叉积。
    __int128 x1 = (__int128)b.x - a.x;
    __int128 y1 = (__int128)b.y - a.y;
    __int128 x2 = (__int128)c.x - a.x;
    __int128 y2 = (__int128)c.y - a.y;
    return x1 * y2 - y1 * x2;
}

Real swCross(P a, P b, P c)
{
    // a、b 确定有向直线，c 是查询点；返回浮点叉积。
    return cross(b - a, c - a);
}

bool swOnSeg(Point<long long> p, Line<long long> l)
{
    // p 是整数点，l 是整数闭线段；返回 p 是否在线段上，方向判定使用宽整数。
    return swCross(l.a, l.b, p) == 0 && min(l.a.x, l.b.x) <= p.x && p.x <= max(l.a.x, l.b.x) &&
           min(l.a.y, l.b.y) <= p.y && p.y <= max(l.a.y, l.b.y);
}

bool swMeet(Line<long long> a, Line<long long> b)
{
    // a、b 是整数闭线段；返回二者是否有公共点，叉积使用宽整数。
    __int128 c1 = swCross(a.a, a.b, b.a);
    __int128 c2 = swCross(a.a, a.b, b.b);
    __int128 c3 = swCross(b.a, b.b, a.a);
    __int128 c4 = swCross(b.a, b.b, a.b);
    if (c1 == 0 && swOnSeg(b.a, a))
    {
        return true;
    }
    if (c2 == 0 && swOnSeg(b.b, a))
    {
        return true;
    }
    if (c3 == 0 && swOnSeg(a.a, b))
    {
        return true;
    }
    if (c4 == 0 && swOnSeg(a.b, b))
    {
        return true;
    }
    return ((c1 > 0) != (c2 > 0)) && ((c3 > 0) != (c4 > 0));
}

bool swMeet(Line<Real> a, Line<Real> b)
{
    // a、b 是浮点闭线段；按统一浮点误差返回二者是否有公共点。
    return segMeet(a, b);
}

template <class T> optional<pair<int, int>> anySegMeet(vector<Line<T>> seg)
{
    // seg 是 long long 或 Real 闭线段集；返回任意一对相交线段的原下标，不存在时返回空。
    // 整数坐标绝对值须不超过 4E18；浮点活动次序使用原值严格比较。
    int n = (int)seg.size();
    vector<pair<Point<T>, int>> endpoint;
    for (int i = 0; i < n; i++)
    {
        endpoint.push_back({seg[i].a, i});
        endpoint.push_back({seg[i].b, i});
    }
    sort(endpoint.begin(), endpoint.end());
    for (int i = 0; i < (int)endpoint.size();)
    {
        int j = i;
        int who = -1;
        while (j < (int)endpoint.size() && endpoint[j].first == endpoint[i].first)
        {
            if (who != -1 && who != endpoint[j].second)
            {
                return pair<int, int>{min(who, endpoint[j].second), max(who, endpoint[j].second)};
            }
            who = endpoint[j].second;
            j++;
        }
        i = j;
    }
    vector<tuple<T, int, int>> event; // 三维依次为横坐标、0 插入/1 竖线查询/2 删除、线段下标。
    for (int i = 0; i < n; i++)
    {
        if (seg[i].b.x < seg[i].a.x)
        {
            swap(seg[i].a, seg[i].b);
        }
        if (seg[i].a.x == seg[i].b.x)
        {
            event.push_back({seg[i].a.x, 1, i});
        }
        else
        {
            event.push_back({seg[i].a.x, 0, i});
            event.push_back({seg[i].b.x, 2, i});
        }
    }
    sort(event.begin(), event.end());
    Point<T> probe{};
    auto cmp = [&](int i, int j)
    {
        if (i == j)
        {
            return false;
        }
        if (j == -1)
        {
            auto c = swCross(seg[i].a, seg[i].b, probe);
            return c > 0;
        }
        if (i == -1)
        {
            auto c = swCross(seg[j].a, seg[j].b, probe);
            return c <= 0;
        }
        if (seg[i].a.x >= seg[j].a.x)
        {
            auto c = swCross(seg[j].a, seg[j].b, seg[i].a);
            if (c != 0)
            {
                return c < 0;
            }
        }
        else
        {
            auto c = swCross(seg[i].a, seg[i].b, seg[j].a);
            if (c != 0)
            {
                return c > 0;
            }
        }
        return i < j;
    };
    set<int, decltype(cmp)> active(cmp);
    for (int l = 0; l < (int)event.size();)
    {
        int r = l + 1;
        while (r < (int)event.size() && get<0>(event[r]) == get<0>(event[l]))
        {
            r++;
        }
        for (int i = l; i < r; i++)
        {
            auto [x, type, id] = event[i];
            if (type != 0)
            {
                continue;
            }
            auto it = active.insert(id).first;
            if (it != active.begin())
            {
                int j = *prev(it);
                if (swMeet(seg[id], seg[j]))
                {
                    return pair<int, int>{min(id, j), max(id, j)};
                }
            }
            if (next(it) != active.end())
            {
                int j = *next(it);
                if (swMeet(seg[id], seg[j]))
                {
                    return pair<int, int>{min(id, j), max(id, j)};
                }
            }
        }
        vector<int> vertical;
        for (int i = l; i < r; i++)
        {
            if (get<1>(event[i]) == 1)
            {
                vertical.push_back(get<2>(event[i]));
            }
        }
        sort(vertical.begin(),
             vertical.end(),
             [&](int i, int j)
             {
                 T il = min(seg[i].a.y, seg[i].b.y);
                 T ir = max(seg[i].a.y, seg[i].b.y);
                 T jl = min(seg[j].a.y, seg[j].b.y);
                 T jr = max(seg[j].a.y, seg[j].b.y);
                 return tuple(il, ir, i) < tuple(jl, jr, j);
             });
        int who = -1;
        T high{};
        for (int id : vertical)
        {
            T low = min(seg[id].a.y, seg[id].b.y);
            T curHigh = max(seg[id].a.y, seg[id].b.y);
            if (who != -1 && swMeet(seg[id], seg[who]))
            {
                return pair<int, int>{min(id, who), max(id, who)};
            }
            if (who == -1 || curHigh > high)
            {
                who = id;
                high = curHigh;
            }
            probe = {get<0>(event[l]), low};
            auto it = active.lower_bound(-1);
            if (it != active.begin())
            {
                int j = *prev(it);
                if (swMeet(seg[id], seg[j]))
                {
                    return pair<int, int>{min(id, j), max(id, j)};
                }
            }
            if (it != active.end() && swMeet(seg[id], seg[*it]))
            {
                return pair<int, int>{min(id, *it), max(id, *it)};
            }
        }
        for (int i = l; i < r; i++)
        {
            auto [x, type, id] = event[i];
            if (type != 2)
            {
                continue;
            }
            auto it = active.find(id);
            assert(it != active.end()); // 调试检查，可删。
            auto nx = next(it);
            if (it != active.begin() && nx != active.end())
            {
                int u = *prev(it);
                int v = *nx;
                if (swMeet(seg[u], seg[v]))
                {
                    return pair<int, int>{min(u, v), max(u, v)};
                }
            }
            active.erase(it);
        }
        l = r;
    }
    return nullopt;
}
