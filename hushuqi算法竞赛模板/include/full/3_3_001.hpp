#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

#ifndef HUSHUQI_FULL_COMMON
#define HUSHUQI_FULL_COMMON
constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是当前值，b 是候选值；若 b 更小则更新 a 并返回 true。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是当前值，b 是候选值；若 b 更大则更新 a 并返回 true。
    return a < b ? a = b, true : false;
}
#endif

struct Sum
{
    // val 是当前区间的元素和。
    long long val = 0;

    Sum() = default;
    Sum(long long x) : val(x)
    {
        // x 是单个叶子的值。
    }

    friend Sum operator+(Sum a, Sum b)
    {
        // a、b 是相邻区间信息；返回合并后的区间和。
        return a.val + b.val;
    }
};

struct MaxInfo
{
    // val 是区间最大值；空区间以 long long 最小值作为 max 的单位元。
    long long val = numeric_limits<long long>::lowest();

    MaxInfo() = default;
    MaxInfo(long long x) : val(x)
    {
        // x 是单个叶子的值。
    }

    friend MaxInfo operator+(MaxInfo a, MaxInfo b)
    {
        // a、b 是按顺序相邻的区间信息；返回两段的最大值。
        return max(a.val, b.val);
    }
};

template <class Info> struct Seg
{
    // n 是真实叶子数，size 是不小于 n 的二次幂；tr 保存区间信息。
    int n;
    int size;
    vector<Info> tr;

    Seg(int n = 0)
    {
        // n 是叶子数；初始信息均为合并单位元。
        init(n);
    }
    Seg(const vector<Info> &a)
    {
        // a 是各叶子的初始信息。
        init(a);
    }

    void init(int n_)
    {
        // n_ 是新的叶子数；幂次二基址让每个内部节点对应连续区间。
        assert(n_ >= 0); // 调试检查，可删
        n = n_;
        size = 1;
        while (size < n)
        {
            size <<= 1;
        }
        tr.assign(2 * size, Info{});
    }

    void init(const vector<Info> &a)
    {
        // a 是新的叶子信息；线性重建整棵树。
        init(a.size());
        copy(a.begin(), a.end(), tr.begin() + size);
        for (int i = size - 1; i > 0; --i)
        {
            pull(i);
        }
    }

    void pull(int p)
    {
        // p 是内部节点编号；按左右顺序合并两个儿子。
        tr[p] = tr[2 * p] + tr[2 * p + 1];
    }

    void set(int p, Info v)
    {
        // p 是 0 下标位置，v 是新叶子信息；修改后重算祖先。
        assert(0 <= p && p < n); // 调试检查，可删
        for (tr[p += size] = v; p >>= 1;)
        {
            pull(p);
        }
    }

    Info query(int l, int r) const
    {
        // l、r 是半开区间端点；按原顺序返回 [l,r) 的合并结果。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        Info x{}, y{};
        for (l += size, r += size; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1)
            {
                x = x + tr[l++];
            }
            if (r & 1)
            {
                y = tr[--r] + y;
            }
        }
        return x + y;
    }

    template <class F>
    int findFirst(int p, int l, int r, int ql, int qr, F &ok) const
    {
        if (r <= ql || qr <= l)
        {
            return -1;
        }
        if (ql <= l && r <= qr && !ok(tr[p]))
        {
            return -1;
        }
        if (r - l == 1)
        {
            return l < n ? l : -1;
        }
        int m = l + (r - l) / 2;
        int x = findFirst(2 * p, l, m, ql, qr, ok);
        return x == -1 ? findFirst(2 * p + 1, m, r, ql, qr, ok) : x;
    }

    template <class F>
    int findFirst(int l, int r, F ok) const
    {
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return l == r ? -1 : findFirst(1, 0, size, l, r, ok);
    }

    template <class F>
    int findLast(int p, int l, int r, int ql, int qr, F &ok) const
    {
        if (r <= ql || qr <= l)
        {
            return -1;
        }
        if (ql <= l && r <= qr && !ok(tr[p]))
        {
            return -1;
        }
        if (r - l == 1)
        {
            return l < n ? l : -1;
        }
        int m = l + (r - l) / 2;
        int x = findLast(2 * p + 1, m, r, ql, qr, ok);
        return x == -1 ? findLast(2 * p, l, m, ql, qr, ok) : x;
    }

    template <class F>
    int findLast(int l, int r, F ok) const
    {
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return l == r ? -1 : findLast(1, 0, size, l, r, ok);
    }
};
