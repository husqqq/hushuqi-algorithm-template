#pragma once

#include "11_3_001.hpp"

template <class T> T diameter2(const vector<Point<T>> &a)
{
    int n = (int)a.size();
    if (n <= 1)
    {
        return 0;
    }
    if (n == 2)
    {
        return norm(a[0] - a[1]);
    }
    T ans = 0;
    int j = 1;
    for (int i = 0; i < n; i++)
    {
        while (sgn(cross(a[(i + 1) % n] - a[i],
                         a[(j + 1) % n] - a[j])) > 0)
        {
            j = (j + 1) % n;
        }
        ans = max(ans, norm(a[i] - a[j]));
        ans = max(ans, norm(a[(i + 1) % n] - a[j]));
    }
    return ans;
}

template <class T> T weakDiam2(const vector<Point<T>> &a)
{
    // a 是按边界顺序给出的弱凸多边形；允许相邻边共线，返回最远点对距离平方。
    int n = a.size();
    if (n <= 2)
    {
        return n < 2 ? 0 : norm(a[0] - a[1]);
    }
    vector<Point<T>> h;
    for (int i = 0; i < n; i++)
    {
        if (cross(a[i] - a[(i + n - 1) % n], a[(i + 1) % n] - a[i]) != 0)
        {
            h.push_back(a[i]);
        }
    }
    if (h.empty())
    {
        auto [l, r] = minmax_element(a.begin(), a.end());
        return norm(*l - *r);
    }
    if (h.size() <= 2)
    {
        return h.size() < 2 ? 0 : norm(h[0] - h[1]);
    }
    T ans = 0;
    int m = h.size(), j = 1;
    for (int i = 0; i < m; i++)
    {
        auto area = [&](int k)
        {
            T x = cross(h[(i + 1) % m] - h[i], h[k] - h[i]);
            return x < 0 ? -x : x;
        };
        while (area((j + 1) % m) > area(j))
        {
            j = (j + 1) % m;
        }
        ans = max(ans, norm(h[i] - h[j]));
        ans = max(ans, norm(h[(i + 1) % m] - h[j]));
        int k = (j + 1) % m;
        if (area(k) == area(j))
        {
            ans = max(ans, norm(h[i] - h[k]));
            ans = max(ans, norm(h[(i + 1) % m] - h[k]));
        }
    }
    return ans;
}

template <class T> pair<int, int> farthestPair(const vector<Point<T>> &p)
{
    // p 是至少含两个点的点集；返回一对欧氏距离最大的原下标。
    assert(p.size() >= 2); // 调试检查，可删。
    map<Point<T>, int> id;
    for (int i = 0; i < (int)p.size(); i++)
    {
        id.try_emplace(p[i], i);
    }
    auto h = hull(p);
    if (h.size() == 1)
    {
        return {0, 1};
    }
    if (h.size() == 2)
    {
        return {id[h[0]], id[h[1]]};
    }
    int m = h.size(), j = 1;
    T best = -1;
    pair<int, int> ans{0, 1};
    auto update = [&](int x, int y)
    {
        T d = norm(h[x] - h[y]);
        if (best < d)
        {
            best = d;
            ans = {id[h[x]], id[h[y]]};
        }
    };
    for (int i = 0; i < m; i++)
    {
        while (cross(h[(i + 1) % m] - h[i],
                     h[(j + 1) % m] - h[j]) > 0)
        {
            j = (j + 1) % m;
        }
        update(i, j);
        update((i + 1) % m, j);
    }
    return ans;
}
