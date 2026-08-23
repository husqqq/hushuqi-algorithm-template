#pragma once

#include "11_1_009.hpp"

template <class T> struct HullChain
{
    int dir;          // dir=1 维护上凸链，dir=-1 维护下凸链。
    set<Point<T>> a;  // a 按 x、y 保存当前严格凸链。

    HullChain(int dir) : dir(dir)
    {
        assert(abs(dir) == 1);
    }

    T turn(Point<T> x, Point<T> y, Point<T> z) const
    {
        // x、y、z 是三个点；返回按当前上下壳方向修正后的转向。
        return dir * cross(x, y, z);
    }

    bool add(Point<T> p)
    {
        // p 是新点；若 p 改变当前凸链则插入并返回 true。
        auto it = a.lower_bound({p.x, numeric_limits<T>::lowest()});
        if (it != a.end() && it->x == p.x)
        {
            if (dir * (p.y - it->y) <= 0)
            {
                return false;
            }
            a.erase(it);
        }
        it = a.lower_bound({p.x, numeric_limits<T>::lowest()});
        if (it != a.begin() && it != a.end() && turn(*prev(it), *it, p) <= 0)
        {
            return false;
        }
        auto q = a.insert(p).first;
        while (q != a.begin())
        {
            auto y = prev(q);
            if (y == a.begin() || turn(*prev(y), *y, *q) < 0)
            {
                break;
            }
            a.erase(y);
        }
        while (next(q) != a.end())
        {
            auto y = next(q);
            auto z = next(y);
            if (z == a.end() || turn(*q, *y, *z) < 0)
            {
                break;
            }
            a.erase(y);
        }
        return true;
    }

    bool contains(Point<T> p) const
    {
        // p 是查询点；返回 p 是否不越过当前凸链。
        if (a.empty())
        {
            return false;
        }
        auto it = a.lower_bound({p.x, numeric_limits<T>::lowest()});
        if (it != a.end() && it->x == p.x)
        {
            return dir * (p.y - it->y) <= 0;
        }
        if (it == a.begin() || it == a.end())
        {
            return false;
        }
        return turn(*prev(it), *it, p) <= 0;
    }
};

template <class T> struct DynHull
{
    HullChain<T> up{1};    // up 保存从左到右的上凸链。
    HullChain<T> down{-1}; // down 保存从左到右的下凸链。

    void add(Point<T> p)
    {
        // p 是新点；把 p 插入动态凸包。
        up.add(p);
        down.add(p);
    }

    bool contains(Point<T> p) const
    {
        // p 是查询点；返回 p 是否在当前凸包内部或边界上。
        return up.contains(p) && down.contains(p);
    }

    vector<Point<T>> points() const
    {
        // 无参数；返回逆时针排列的当前严格凸包点。
        vector<Point<T>> h(up.a.begin(), up.a.end());
        for (auto it = down.a.rbegin(); it != down.a.rend(); it++)
        {
            if (h.empty() || (*it != h.front() && *it != h.back()))
            {
                h.push_back(*it);
            }
        }
        return h;
    }
};
