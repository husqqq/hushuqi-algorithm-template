#pragma once

#include "11_3_003.hpp"

template <class T, class F> int hullExtreme(const vector<Point<T>> &a, F f)
{
    // a 是严格逆时针凸包，f(x,y) 比较相邻方向；返回对应循环单峰的极值下标。
    int l = 0, r = (int)a.size() - 1, d = 1;
    if (f(a[r], a[l]))
    {
        swap(l, r);
        d = -1;
    }
    while (d * (r - l) > 1)
    {
        int m = (l + r) / 2;
        if (f(a[m], a[l]) && f(a[m], a[m - d]))
        {
            l = m;
        }
        else
        {
            r = m;
        }
    }
    return l;
}

template <class T> pair<int, int> hullTangents(const vector<Point<T>> &a, Point<T> p)
{
    // a 是严格逆时针凸包，p 是查询点；返回 p 的两个切点下标。
    int n = a.size();
    if (!n || inConvex(a, p))
    {
        return {-1, -1};
    }
    if (n <= 2)
    {
        return {0, n - 1};
    }
    int x = hullExtreme(a, [&](Point<T> u, Point<T> v)
    {
        return sgn(cross(p, v, u)) > 0;
    });
    int y = hullExtreme(a, [&](Point<T> u, Point<T> v)
    {
        return sgn(cross(p, u, v)) > 0;
    });
    return {x, y};
}

P nearSegment(P p, P a, P b)
{
    // p 是查询点，a、b 是不同端点；返回线段 ab 上离 p 最近的点。
    P v = b - a;
    Real t = clamp(dot(p - a, v) / norm(v), Real(0), Real(1));
    return a + v * t;
}

optional<pair<Real, P>> closestHull(const vector<P> &a, P p)
{
    // a 是严格逆时针凸包，p 是查询点；返回最短距离和一个最近点。
    int n = a.size();
    if (!n)
    {
        return nullopt;
    }
    if (inConvex(a, p))
    {
        return pair<Real, P>{0, p};
    }
    if (n == 1)
    {
        return pair<Real, P>{abs(p - a[0]), a[0]};
    }
    if (n == 2)
    {
        P q = nearSegment(p, a[0], a[1]);
        return pair<Real, P>{abs(p - q), q};
    }
    auto at = [&](int i)
    {
        return a[(i % n + n) % n];
    };
    auto chain = [&](int l, int r)
    {
        if (l > r)
        {
            r += n;
        }
        auto get = [&](int i)
        {
            P q = nearSegment(p, at(i), at(i + 1));
            return pair<Real, P>{abs(p - q), q};
        };
        auto ans = get(l);
        int side = sgn(dot(p - at(l), at(l + 1) - at(l)));
        while (l + 1 < r)
        {
            int m = (l + r) / 2;
            int cur = sgn(dot(p - at(m), at(m + 1) - at(m)));
            if (cur == side)
            {
                l = m;
            }
            else
            {
                r = m;
            }
        }
        auto q = get(l);
        if (q.first < ans.first)
        {
            ans = q;
        }
        return ans;
    };
    auto [x, y] = hullTangents(a, p);
    auto u = chain(x, y);
    auto v = chain(y, x);
    return u.first < v.first ? u : v;
}
