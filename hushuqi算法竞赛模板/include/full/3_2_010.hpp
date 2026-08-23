#pragma once

#include <bits/stdc++.h>

using namespace std;

#define int long long

template <class T> struct StaticRect
{
    struct P
    {
        // x、y 是静态点坐标，w 是该点权值。
        long long x, y;
        T w;
    };

    struct Q
    {
        // l、d、r、u 依次是半开查询矩形的左、下、右、上边界。
        long long l, d, r, u;
    };

    // p 保存全部静态带权点，q 保存全部待回答的矩形询问。
    vector<P> p;
    vector<Q> q;

    void add(long long x, long long y, T w)
    {
        // 把坐标为 (x,y)、权值为 w 的静态点加入点集。
        p.push_back({x, y, w});
    }

    int query(long long l, long long d, long long r, long long u)
    {
        // 登记半开矩形 [l,r) x [d,u) 的点权和询问，返回其编号。
        q.push_back({l, d, r, u});
        return (int)q.size() - 1;
    }

    vector<T> work() const
    {
        // 离线回答全部登记询问，返回值按 query 的返回编号排列。
        struct E
        {
            // x 是扫描线横坐标；d、u 是纵向半开范围；id 是询问编号，sgn 是容斥符号。
            long long x, d, u;
            int id, sgn;
        };

        vector<P> a = p;
        vector<E> e;
        e.reserve(2 * q.size());
        for (int i = 0; i < (int)q.size(); ++i)
        {
            auto [l, d, r, u] = q[i];
            e.push_back({l, d, u, i, -1});
            e.push_back({r, d, u, i, 1});
        }
        sort(a.begin(), a.end(), [](const P &x, const P &y)
             {
                 return x.x < y.x;
             });
        sort(e.begin(), e.end(), [](const E &x, const E &y)
             {
                 return x.x < y.x;
             });

        vector<long long> ys;
        ys.reserve(a.size());
        for (auto [x, y, w] : a)
        {
            ys.push_back(y);
        }
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
        vector<T> bit(ys.size() + 1);
        auto addBit = [&](int x, T w)
        {
            // x 是 0 下标离散纵坐标，w 是加入该点的权值。
            for (++x; x < (int)bit.size(); x += x & -x)
            {
                bit[x] += w;
            }
        };
        auto sumBit = [&](int x)
        {
            // x 是离散前缀长度，返回所有离散下标小于 x 的权值和。
            T ans{};
            for (; x; x -= x & -x)
            {
                ans += bit[x];
            }
            return ans;
        };

        vector<T> ans(q.size());
        int at = 0;
        for (auto [x, d, u, id, sgn] : e)
        {
            while (at < (int)a.size() && a[at].x < x)
            {
                int y = lower_bound(ys.begin(), ys.end(), a[at].y) - ys.begin();
                addBit(y, a[at].w);
                ++at;
            }
            int l = lower_bound(ys.begin(), ys.end(), d) - ys.begin();
            int r = lower_bound(ys.begin(), ys.end(), u) - ys.begin();
            T cur = sumBit(r);
            cur -= sumBit(l);
            if (sgn == 1)
            {
                ans[id] += cur;
            }
            else
            {
                ans[id] -= cur;
            }
        }
        return ans;
    }
};
