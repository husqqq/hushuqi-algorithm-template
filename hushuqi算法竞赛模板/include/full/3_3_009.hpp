#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

struct MergeSeg
{
    struct Node
    {
        // l、r 是左右儿子编号，sum 是当前值域的权值和。
        int l = 0, r = 0;
        long long sum = 0;
    };

    // lo、hi 是整棵权值树的半开值域，t 是所有树共享的节点池。
    int lo, hi;
    vector<Node> t{{}};

    MergeSeg(int lo, int hi) : lo(lo), hi(hi)
    {
        // lo、hi 是非空半开值域边界。
        assert(lo < hi); // 调试检查，可删
    }
    int newNode()
    {
        // 分配并返回一个全零节点编号。
        t.push_back({});
        return (int)t.size() - 1;
    }
    int makeRoot() const
    {
        // 返回表示空树的根编号 0。
        return 0;
    }

    int add(int p, int l, int r, int x, long long v)
    {
        // p、[l,r) 是当前树，x 是值域位置，v 是权值增量；返回更新后的根。
        if (!p)
        {
            p = newNode();
        }
        t[p].sum += v;
        if (l + 1 == r)
        {
            return p;
        }
        int m = midpoint(l, r);
        if (x < m)
        {
            t[p].l = add(t[p].l, l, m, x, v);
        }
        else
        {
            t[p].r = add(t[p].r, m, r, x, v);
        }
        return p;
    }

    void add(int &p, int x, long long v)
    {
        // p 是根，x 是值域位置，v 是权值增量；原地更新 p。
        assert(lo <= x && x < hi); // 调试检查，可删
        p = add(p, lo, hi, x, v);
    }

    int merge(int p, int q, int l, int r)
    {
        // p、q 是当前值域 [l,r) 上的两棵树根；破坏性合并并返回根。
        if (!p || !q)
        {
            return p | q;
        }
        if (l + 1 == r)
        {
            t[p].sum += t[q].sum;
            return p;
        }
        int m = midpoint(l, r);
        t[p].l = merge(t[p].l, t[q].l, l, m);
        t[p].r = merge(t[p].r, t[q].r, m, r);
        t[p].sum = t[t[p].l].sum + t[t[p].r].sum;
        return p;
    }

    int merge(int p, int q)
    {
        // p、q 是两棵共享同一值域的树根；破坏性合并并返回根。
        return merge(p, q, lo, hi);
    }

    long long sum(int p, int l, int r, int x, int y) const
    {
        // p、[l,r) 是当前树；返回值域 [x,y) 的权值和。
        if (!p || y <= l || r <= x)
        {
            return 0;
        }
        if (x <= l && r <= y)
        {
            return t[p].sum;
        }
        int m = midpoint(l, r);
        return sum(t[p].l, l, m, x, y) + sum(t[p].r, m, r, x, y);
    }

    long long rangeSum(int p, int l, int r) const
    {
        // p 是树根；返回值域 [l,r) 的权值和。
        assert(lo <= l && l <= r && r <= hi); // 调试检查，可删
        return sum(p, lo, hi, l, r);
    }
};
