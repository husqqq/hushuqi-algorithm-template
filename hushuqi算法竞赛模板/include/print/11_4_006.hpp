#pragma once

#include "11_4_005.hpp"

C minCircle(vector<P> a, int seed = 0)
{
    mt19937 rng(seed);
    shuffle(a.begin(), a.end(), rng);
    C c{{0, 0}, 0};
    for (int i = 0; i < (int)a.size(); i++)
    {
        if (!in(c, a[i]))
        {
            c = {a[i], 0};
            for (int j = 0; j < i; j++)
            {
                if (!in(c, a[j]))
                {
                    c = circle(a[i], a[j]);
                    for (int k = 0; k < j; k++)
                    {
                        if (!in(c, a[k]))
                        {
                            c = circle(a[i], a[j], a[k]);
                        }
                    }
                }
            }
        }
    }
    return c;
}

string minCircleBd(const vector<Point<long long>> &p,
                                      unsigned long long seed = 0x9E3779B97F4A7C15ULL)
{
    // p 是整数点集，seed 是确定性洗牌种子；返回各点是否位于最小包围圆圆周的 0/1 串。
    struct Support
    {
        int size = 0; // 当前支撑点个数。
        array<Point<long long>, 3> p; // p[0..size) 是定义当前圆的支撑点。
    };
    auto cross128 = [](Point<long long> a, Point<long long> b)
    {
        return (__int128)a.x * b.y - (__int128)a.y * b.x;
    };
    auto norm128 = [](Point<long long> a)
    {
        return (__int128)a.x * a.x + (__int128)a.y * a.y;
    };
    auto diameter = [&](Point<long long> a, Point<long long> b, Point<long long> x)
    {
        Point<long long> u{2 * x.x - a.x - b.x, 2 * x.y - a.y - b.y};
        __int128 lhs = norm128(u), rhs = norm128(a - b);
        return lhs < rhs ? 1 : lhs == rhs ? 0 : -1;
    };
    auto contain = [&](const Support &s, Point<long long> x)
    {
        if (s.size == 0) return -1;
        if (s.size == 1) return x == s.p[0] ? 0 : -1;
        if (s.size == 2) return diameter(s.p[0], s.p[1], x);
        auto a = s.p[0] - x, b = s.p[1] - x, c = s.p[2] - x;
        __int128 det = norm128(a) * cross128(b, c) +
                       norm128(b) * cross128(c, a) +
                       norm128(c) * cross128(a, b);
        __int128 ori = cross128(s.p[1] - s.p[0], s.p[2] - s.p[0]);
        det = ori > 0 ? det : -det;
        return det > 0 ? 1 : det == 0 ? 0 : -1;
    };
    auto reduce = [&](Support &s)
    {
        if (s.size != 3 || cross128(s.p[1] - s.p[0], s.p[2] - s.p[0]) != 0)
        {
            return;
        }
        __int128 d01 = norm128(s.p[0] - s.p[1]);
        __int128 d12 = norm128(s.p[1] - s.p[2]);
        __int128 d02 = norm128(s.p[0] - s.p[2]);
        if (d12 >= d01 && d12 >= d02)
        {
            s.p[0] = s.p[1];
            s.p[1] = s.p[2];
        }
        else if (d02 >= d01)
        {
            s.p[1] = s.p[2];
        }
        s.size = 2;
    };
    vector<Point<long long>> q = p;
    mt19937_64 rng(seed);
    shuffle(q.begin(), q.end(), rng);
    Support s;
    for (int i = 0; i < (int)q.size(); i++)
    {
        if (contain(s, q[i]) >= 0) continue;
        s = {1, {q[i], {}, {}}};
        for (int j = 0; j < i; j++)
        {
            if (contain(s, q[j]) >= 0) continue;
            s = {2, {q[i], q[j], {}}};
            for (int k = 0; k < j; k++)
            {
                if (contain(s, q[k]) >= 0) continue;
                s = {3, {q[i], q[j], q[k]}};
                reduce(s);
            }
        }
    }
    string ans(p.size(), '0');
    for (int i = 0; i < (int)p.size(); i++)
    {
        ans[i] = contain(s, p[i]) == 0 ? '1' : '0';
    }
    return ans;
}
