#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <class T, class C = T> struct PerLiChao
{
    // 当前实现维护最小值。改最大值可反向所有优劣比较并把空值改为负无穷，
    // 或在 k、b 与函数值都能安全取反时对插入直线同时取反、再对 query 结果取反；后者不改变持久化不变量。
    struct Line
    {
        // k、b 是一次函数 y=kx+b 的斜率与截距。
        T k{}, b{};
        C get(T x) const
        {
            // x 是查询横坐标；用比较类型 C 计算函数值。
            return (C)k * x + b;
        }
    };
    struct Node
    {
        // f 是当前区间中点占优的直线，l、r 是左右儿子编号。
        optional<Line> f;
        int l = -1, r = -1;
    };

    // lo、hi 是整数定义域，t 是所有版本共享的节点池。
    T lo, hi;
    vector<Node> t;

    PerLiChao(T lo, T hi) : lo(lo), hi(hi)
    {
        // lo、hi 定义非空整数半开区间；初始空版本根为 -1。
        static_assert(is_integral_v<T> && is_signed_v<T>);
        assert(lo < hi); // 调试检查，可删
    }

    int clone(int p)
    {
        // p 是旧节点编号；复制它，-1 则新建空节点，返回新编号。
        t.push_back(p < 0 ? Node{} : t[p]);
        return t.size() - 1;
    }

    int add(int root, Line f)
    {
        // root 是旧版本根，f 是新直线；返回插入后的版本根。
        return add(root, lo, hi, f);
    }

    int add(int p, T l, T r, Line f)
    {
        // p 是旧节点，[l,r) 是当前整数域，f 是新直线；返回新节点。
        p = clone(p);
        if (!t[p].f)
        {
            t[p].f = f;
            return p;
        }
        T m = midpoint(l, r);
        bool x = f.get(l) < t[p].f->get(l);
        bool y = f.get(m) < t[p].f->get(m);
        if (y)
        {
            swap(f, *t[p].f);
        }
        if (l + 1 == r)
        {
            return p;
        }
        if (x != y)
        {
            t[p].l = add(t[p].l, l, m, f);
        }
        else
        {
            t[p].r = add(t[p].r, m, r, f);
        }
        return p;
    }

    optional<C> query(int p, T x) const
    {
        // p 是版本根，x 是查询横坐标；返回最小值，空版本返回空。
        // 最大值版按直线取反时，对非空返回值取负，空版本仍返回 nullopt。
        assert(lo <= x && x < hi); // 调试检查，可删
        return qval(p, lo, hi, x);
    }

    optional<C> qval(int p, T l, T r, T x) const
    {
        // p 是当前节点，[l,r) 是其整数域，x 是横坐标；返回路径最小值。
        // 若改为直接维护最大值，必须同步反向这里的比较与空值语义。
        if (p < 0)
        {
            return nullopt;
        }
        optional<C> ans;
        if (t[p].f)
        {
            ans = t[p].f->get(x);
        }
        if (l + 1 == r)
        {
            return ans;
        }
        T m = midpoint(l, r);
        auto z = x < m ? qval(t[p].l, l, m, x) : qval(t[p].r, m, r, x);
        if (!ans || (z && *z < *ans))
        {
            ans = z;
        }
        return ans;
    }
};
