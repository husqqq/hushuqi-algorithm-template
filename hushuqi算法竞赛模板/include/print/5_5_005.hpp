#pragma once

#include "5_5_001.hpp"

struct MstU256
{
    // a 从低到高保存四个 64 位肢。
    array<unsigned long long, 4> a{};

    static MstU256 mul(unsigned __int128 x, unsigned __int128 y)
    {
        // x、y 是无符号 128 位整数；返回精确的 256 位乘积。
        unsigned long long p[2] = {(unsigned long long)x, (unsigned long long)(x >> 64)};
        unsigned long long q[2] = {(unsigned long long)y, (unsigned long long)(y >> 64)};
        MstU256 z;
        for (int i = 0; i < 2; i++)
        {
            unsigned __int128 c = 0;
            for (int j = 0; j < 2; j++)
            {
                unsigned __int128 t = (unsigned __int128)p[i] * q[j] + z.a[i + j] + c;
                z.a[i + j] = (unsigned long long)t;
                c = t >> 64;
            }
            z.a[i + 2] = (unsigned long long)c;
        }
        return z;
    }
};

int mstProdCmp(__int128 a, __int128 b, __int128 c, __int128 d)
{
    // 返回 a*b 与 c*d 的大小关系。
    auto mag = [](__int128 x) -> unsigned __int128
    {
        return x < 0 ? 0 - (unsigned __int128)x : (unsigned __int128)x;
    };
    auto x = MstU256::mul(mag(a), mag(b));
    auto y = MstU256::mul(mag(c), mag(d));
    bool nx = (a < 0) != (b < 0), ny = (c < 0) != (d < 0);
    if (x.a == array<unsigned long long, 4>{})
    {
        nx = false;
    }
    if (y.a == array<unsigned long long, 4>{})
    {
        ny = false;
    }
    if (nx != ny)
    {
        return nx ? -1 : 1;
    }
    int s = 0;
    for (int i = 3; i >= 0 && !s; i--)
    {
        s = (x.a[i] > y.a[i]) - (x.a[i] < y.a[i]);
    }
    return nx ? -s : s;
}

struct ProductMST
{
    using Cost = pair<__int128, __int128>;

    struct E
    {
        // u、v 是无向边端点。
        int u, v;
        // x、y 是两种非负边权。
        int x, y;
    };

    // n 是图的点数。
    int n;
    // e 是全部无向边。
    vector<E> e;

    static bool better(const Cost &a, const Cost &b)
    {
        // a、b 是两棵生成树的两种总权；返回 a 的乘积是否更优。
        int c = mstProdCmp(a.first, a.second, b.first, b.second);
        return c != 0 ? c < 0 : a < b;
    }

    ProductMST(int n = 0) : n(n)
    {
        // n 是图的点数；构造一张没有边的图。
    }

    void add(int u, int v, int x, int y)
    {
        // u、v 是端点，x、y 是两种非负边权；加入一条无向边。
        assert(x >= 0 && y >= 0); // 调试检查，可删。
        e.push_back({u, v, x, y});
    }

    optional<Cost> mst(__int128 a, __int128 b) const
    {
        // a、b 是线性目标系数；返回该方向下 MST 的两种原始总权。
        vector<int> p(e.size());
        iota(p.begin(), p.end(), 0);
        sort(p.begin(),
             p.end(),
             [&](int i, int j)
             {
                 int c = mstProdCmp(a,
                                       (__int128)e[i].x - e[j].x,
                                       b,
                                       (__int128)e[j].y - e[i].y);
                 if (c != 0)
                 {
                     return c < 0;
                 }
                 if (e[i].x != e[j].x)
                 {
                     return e[i].x < e[j].x;
                 }
                 return e[i].y < e[j].y;
             });
        DSU d(n);
        Cost s{};
        int z = 0;
        for (int i : p)
        {
            if (d.merge(e[i].u, e[i].v))
            {
                s.first += (__int128)e[i].x;
                s.second += (__int128)e[i].y;
                z++;
            }
        }
        if (z != n - 1)
        {
            return nullopt;
        }
        return s;
    }

    optional<Cost> solve() const
    {
        // 无参数；返回使两种边权总和乘积最小的生成树两项权值和。
        if (n <= 1)
        {
            return Cost{0, 0};
        }
        auto a = mst(1, 0);
        auto b = mst(0, 1);
        if (!a || !b)
        {
            return nullopt;
        }
        Cost ans = better(*a, *b) ? *a : *b;
        auto divide = [&](auto &&self, Cost l, Cost r) -> void
        {
            __int128 x = l.second - r.second;
            __int128 y = r.first - l.first;
            auto c = mst(x, y);
            if (!c || *c == l || *c == r)
            {
                return;
            }
            int cr = mstProdCmp(r.first - l.first,
                                   c->second - l.second,
                                   r.second - l.second,
                                   c->first - l.first);
            if (cr >= 0)
            {
                return;
            }
            if (better(*c, ans))
            {
                ans = *c;
            }
            self(self, l, *c);
            self(self, *c, r);
        };
        divide(divide, *a, *b);
        return ans;
    }
};
