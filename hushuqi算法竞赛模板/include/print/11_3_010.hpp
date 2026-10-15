#pragma once

#include "11_1_001.hpp"

namespace integer_hpi
{
// ILine[0..2] 依次是整数半平面 a*x+b*y+c>=0 的系数 a、b、c。
using ILine = array<int, 3>;
// IRatPoint[0..2] 依次是有理点横坐标分子、纵坐标分子和公共正分母。
using IRatPoint = array<__int128, 3>;

int half(int x, int y)
{
    // x、y 是非零方向向量的坐标；返回它在极角排序中所属的半平面编号。
    return y < 0 || (y == 0 && x < 0);
}

__int128 det(const ILine &p, const ILine &q)
{
    // p、q 是两条半平面的系数；返回它们法向量的叉积。
    return (__int128)p[0] * q[1] - (__int128)p[1] * q[0];
}

bool sameDir(const ILine &p, const ILine &q)
{
    // p、q 是两条半平面；返回它们的边界方向是否相同，而不是相反。
    return det(p, q) == 0 && (__int128)p[0] * q[0] + (__int128)p[1] * q[1] > 0;
}

bool stricter(const ILine &p, const ILine &q)
{
    // p、q 必须同向；返回 p 是否比 q 的可行半平面更小。
    auto ab = [](__int128 x)
    {
        return x < 0 ? -x : x;
    };
    __int128 xp = p[0] ? ab(p[0]) : ab(p[1]);
    __int128 xq = q[0] ? ab(q[0]) : ab(q[1]);
    return (__int128)p[2] * xq < (__int128)q[2] * xp;
}

bool compatible(const ILine &p, const ILine &q)
{
    // p、q 的法向量必须反向；返回两个闭半平面是否相容。
    __int128 d = (__int128)p[0] * p[0] + (__int128)p[1] * p[1];
    __int128 x = -(__int128)p[0] * p[2];
    __int128 y = -(__int128)p[1] * p[2];
    return (__int128)q[0] * x + (__int128)q[1] * y + (__int128)q[2] * d >= 0;
}

bool outside(const ILine &a, const ILine &b, const ILine &c)
{
    // a 是待检查半平面，b、c 是两条不平行边界；返回 b、c 交点是否严格落在 a 外。
    __int128 d = det(b, c);
    assert(d != 0); // 调试检查，可删。
    __int128 x = (__int128)b[1] * c[2] - (__int128)b[2] * c[1];
    __int128 y = (__int128)b[2] * c[0] - (__int128)b[0] * c[2];
    __int128 v = (__int128)a[0] * x + (__int128)a[1] * y + (__int128)a[2] * d;
    return v != 0 && ((v < 0) != (d < 0));
}

optional<vector<ILine>> halfPlane(vector<ILine> a)
{
    // a 是全部整数半平面；空 optional 表示非空但无界。
    for (auto &l : a)
    {
        assert(l[0] != 0 || l[1] != 0); // 调试检查，可删。
    }
    sort(a.begin(),
         a.end(),
         [](const ILine &p, const ILine &q)
         {
             int hp = half(p[1], -p[0]);
             int hq = half(q[1], -q[0]);
             if (hp != hq)
             {
                 return hp < hq;
             }
             auto d = det(p, q);
             if (d != 0)
             {
                 return d > 0;
             }
             if (sameDir(p, q))
             {
                 return stricter(p, q);
             }
             return false;
         });

    vector<ILine> b;
    for (auto l : a)
    {
        if (!b.empty() && sameDir(b.back(), l))
        {
            continue;
        }
        b.push_back(l);
    }

    if (b.empty())
    {
        return nullopt;
    }
    auto ab = [](__int128 x)
    {
        return x < 0 ? -x : x;
    };
    auto gcd128 = [&](__int128 x, __int128 y)
    {
        x = ab(x);
        y = ab(y);
        while (y != 0)
        {
            __int128 r = x % y;
            x = y;
            y = r;
        }
        return x;
    };
    map<pair<__int128, __int128>, ILine> dir;
    for (const ILine &l : b)
    {
        __int128 g = gcd128(l[0], l[1]);
        __int128 x = (__int128)l[0] / g;
        __int128 y = (__int128)l[1] / g;
        auto it = dir.find({-x, -y});
        if (it != dir.end() && !compatible(l, it->second))
        {
            return vector<ILine>{};
        }
        dir[{x, y}] = l;
    }
    for (int i = 0; i < (int)b.size(); i++)
    {
        if (det(b[i], b[(i + 1) % b.size()]) <= 0)
        {
            return nullopt;
        }
    }

    deque<ILine> q;
    for (auto l : b)
    {
        while (q.size() >= 2 && outside(l, q[q.size() - 2], q.back()))
        {
            q.pop_back();
        }
        while (q.size() >= 2 && outside(l, q[0], q[1]))
        {
            q.pop_front();
        }
        q.push_back(l);
    }
    while (q.size() >= 3 && outside(q.front(), q[q.size() - 2], q.back()))
    {
        q.pop_back();
    }
    while (q.size() >= 3 && outside(q.back(), q[0], q[1]))
    {
        q.pop_front();
    }
    if (q.size() < 3)
    {
        return vector<ILine>{};
    }
    return vector<ILine>{q.begin(), q.end()};
}

vector<IRatPoint> vertices(const vector<ILine> &q)
{
    // q 是 halfPlane 返回的有界边界；返回相邻边界交点的“横分子、纵分子、正分母”。
    vector<IRatPoint> p;
    auto ab = [](__int128 z)
    {
        return z < 0 ? -z : z;
    };
    auto gcd128 = [&](__int128 u, __int128 v)
    {
        u = ab(u);
        v = ab(v);
        while (v != 0)
        {
            __int128 r = u % v;
            u = v;
            v = r;
        }
        return u;
    };
    for (int i = 0; i < (int)q.size(); i++)
    {
        auto a = q[i];
        auto b = q[(i + 1) % q.size()];
        __int128 d = det(a, b);
        __int128 x = (__int128)a[1] * b[2] - (__int128)a[2] * b[1];
        __int128 y = (__int128)a[2] * b[0] - (__int128)a[0] * b[2];
        if (d < 0)
        {
            x = -x;
            y = -y;
            d = -d;
        }
        __int128 g = gcd128(gcd128(x, y), d);
        x /= g;
        y /= g;
        d /= g;
        IRatPoint z{x, y, d};
        if (p.empty() || p.back() != z)
        {
            p.push_back(z);
        }
    }
    if (p.size() > 1 && p.front() == p.back())
    {
        p.pop_back();
    }
    return p;
}
}
