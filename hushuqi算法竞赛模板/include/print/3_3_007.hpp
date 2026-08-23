#pragma once
#include "3_3_002.hpp"

template <class T, class F> class SparsePtSeg
{
    struct Node
    {
        // l、r 是孩子编号，pos 是本节点实际保存的位置，x、prod 分别是该点值与子树按下标顺序的积。
        int l = 0, r = 0;
        long long pos = 0;
        T x{}, prod{};
    };

    // lo、hi 是总半开值域，rt 是根编号，e 是空位置值，op 是按左到右合并的结合运算，t 是节点池。
    long long lo, hi;
    int rt = 0;
    T e;
    F op;
    vector<Node> t{Node{}};

    int newNode(long long x, const T &v)
    {
        // x 是新节点位置，v 是该点值；新建只保存此点的节点并返回编号。
        Node q;
        q.pos = x;
        q.x = q.prod = v;
        t.push_back(move(q));
        assert(t.size() <= (size_t)numeric_limits<int>::max()); // 调试检查，可删
        return (int)t.size() - 1;
    }

    void pull(int p)
    {
        // p 是非空节点；按下标顺序合并左右子树与本点，重算子树积。
        T cur = t[p].x;
        if (t[p].l)
        {
            cur = op(t[t[p].l].prod, cur);
        }
        if (t[p].r)
        {
            cur = op(cur, t[t[p].r].prod);
        }
        t[p].prod = cur;
    }

    int set(int p, long long l, long long r, long long x, T v)
    {
        // p、[l,r) 是当前稀疏子树，x、v 是待写位置和值；返回更新后的子树根。
        if (!p)
        {
            return newNode(x, v);
        }
        if (t[p].pos == x)
        {
            t[p].x = v;
            pull(p);
            return p;
        }
        long long m = l + (r - l) / 2;
        if (x < m)
        {
            if (t[p].pos < x)
            {
                swap(t[p].pos, x);
                swap(t[p].x, v);
            }
            int a = set(t[p].l, l, m, x, move(v));
            t[p].l = a;
        }
        else
        {
            if (x < t[p].pos)
            {
                swap(t[p].pos, x);
                swap(t[p].x, v);
            }
            int b = set(t[p].r, m, r, x, move(v));
            t[p].r = b;
        }
        pull(p);
        return p;
    }

    void prod(int p, long long l, long long r, long long ql, long long qr, T &ans) const
    {
        // p、[l,r) 是当前子树，[ql,qr) 是查询区间，ans 按从左到右顺序累积答案。
        ql = max(ql, l);
        qr = min(qr, r);
        if (ql >= qr || !p)
        {
            return;
        }
        if (ql == l && qr == r)
        {
            ans = op(ans, t[p].prod);
            return;
        }
        long long m = l + (r - l) / 2;
        prod(t[p].l, l, m, ql, qr, ans);
        if (ql <= t[p].pos && t[p].pos < qr)
        {
            ans = op(ans, t[p].x);
        }
        prod(t[p].r, m, r, ql, qr, ans);
    }

  public:
    SparsePtSeg(long long lo, long long hi, T e, F op) : lo(lo), hi(hi), e(move(e)), op(move(op))
    {
        // lo、hi 是非空半开整数值域，e 是未写位置的值，op 是保持左右顺序的结合运算。
        assert(lo < hi); // 调试检查，可删
    }

    void reserveNodes(int cap)
    {
        // cap 是预计出现的不同位置数；预留节点池容量，不改变结构语义。
        assert(cap >= 0); // 调试检查，可删
        t.reserve(cap + 1);
    }

    void set(long long x, const T &v)
    {
        // x 是值域内位置，v 是新值；把该点赋为 v，未写过的位置默认取 e。
        assert(lo <= x && x < hi); // 调试检查，可删
        rt = set(rt, lo, hi, x, v);
    }

    T get(long long x) const
    {
        // x 是值域内位置；返回该点当前值，未出现过时返回 e。
        assert(lo <= x && x < hi); // 调试检查，可删
        int p = rt;
        while (p)
        {
            if (t[p].pos == x)
            {
                return t[p].x;
            }
            p = x < t[p].pos ? t[p].l : t[p].r;
        }
        return e;
    }

    T prod(long long l, long long r) const
    {
        // l、r 是值域内半开端点；返回 [l,r) 按下标从左到右合并的积，空区间返回 e。
        assert(lo <= l && l <= r && r <= hi); // 调试检查，可删
        T ans = e;
        prod(rt, lo, hi, l, r, ans);
        return ans;
    }
};

class DynamicSeg
{
    struct Add
    {
        long long operator()(long long a, long long b) const
        {
            // a、b 是相邻两段的元素和；返回两段合并后的和。
            return a + b;
        }
    };

    // tr 是泛型稀疏线段树的加法实例，保留旧的单点加、区间和接口。
    SparsePtSeg<long long, Add> tr;

  public:
    DynamicSeg(long long lo, long long hi) : tr(lo, hi, 0, Add{})
    {
        // lo、hi 是非空半开值域边界；初始所有点均为零。
    }

    void reserveNodes(int cap)
    {
        // cap 是预计被修改过的不同位置数；转交给内部节点池预留容量。
        tr.reserveNodes(cap);
    }

    void add(long long x, long long v)
    {
        // x 是值域内位置，v 是有符号增量；把该点增加 v。
        tr.set(x, tr.get(x) + v);
    }

    long long sum(long long l, long long r) const
    {
        // l、r 是查询半开区间；返回其中元素和。
        return tr.prod(l, r);
    }
};
