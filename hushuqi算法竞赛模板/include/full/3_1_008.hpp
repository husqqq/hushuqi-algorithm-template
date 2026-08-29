#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

template <class Group> struct PotentialDSU
{
    using T = typename Group::T;

    // f[x] 是 x 的父亲；s[x] 仅在 x 为根时表示集合大小。
    vector<int> f, s;
    // pot[x] 满足 value[x]=value[f[x]]*pot[x]；根的 pot 是单位元。
    vector<T> pot;

    PotentialDSU(int n = 0)
    {
        // n 是元素个数；初始时每个元素自成集合且势能为群单位元。
        init(n);
    }

    void init(int n)
    {
        // n 是新的元素个数；清空旧状态并重建单点集合。
        assert(n >= 0); // 调试检查，可删
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        s.assign(n, 1);
        pot.assign(n, Group::id());
    }

    int find(int x)
    {
        // x 是元素编号；压缩父链、更新到根的右势能并返回集合根。
        if (x == f[x])
        {
            return x;
        }
        int p = f[x];
        f[x] = find(p);
        pot[x] = Group::op(pot[p], pot[x]);
        return f[x];
    }

    bool merge(int x, int y, const T &d)
    {
        // 施加 value[x]*d=value[y]；返回约束是否与已有信息相容。
        int a = find(x), b = find(y);
        if (a == b)
        {
            return Group::op(Group::inv(pot[x]), pot[y]) == d;
        }
        if (s[a] < s[b])
        {
            f[a] = b;
            pot[a] = Group::op(Group::op(pot[y], Group::inv(d)), Group::inv(pot[x]));
            s[b] += s[a];
        }
        else
        {
            f[b] = a;
            pot[b] = Group::op(Group::op(pot[x], d), Group::inv(pot[y]));
            s[a] += s[b];
        }
        return true;
    }

    bool same(int x, int y)
    {
        // x、y 是元素编号；返回二者是否同属一个集合。
        return find(x) == find(y);
    }

    T diff(int x, int y)
    {
        // x、y 必须连通；返回 value[x] 的逆乘 value[y]，即 value[x]^{-1}*value[y]。
        assert(same(x, y)); // 调试检查，可删
        return Group::op(Group::inv(pot[x]), pot[y]);
    }

    int size(int x)
    {
        // x 是元素编号；返回 x 所在集合的元素个数。
        return s[find(x)];
    }
};
