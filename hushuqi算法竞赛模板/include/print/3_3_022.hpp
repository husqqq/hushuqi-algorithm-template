#pragma once
#include "3_3_002.hpp"

template <class T, class F> class RangeSetSeg
{
    struct Node
    {
        // prod 是该区间按从左到右顺序合并的积，tag 是等待下传的统一赋值幂表编号，-1 表示无标记。
        T prod{};
        int tag = -1;
    };

    // n 是实际序列长度，base 是补齐后的二次幂叶子数，height 是树高。
    int n = 0, base = 1, height = 0;
    T e;
    F op;
    vector<Node> tr;
    // powers 的第 id 块保存同一次赋值 x 的 x^(2^0),...,x^(2^height)。
    vector<T> powers;

    const T &power(int id, int level) const
    {
        // id 是赋值幂表编号，level 对应长度 2^level；返回 x^(2^level)。
        return powers[(size_t)id * (height + 1) + level];
    }

    int makeTag(const T &x)
    {
        // x 是一次新的统一赋值；建立按二次幂长度索引的重复积并返回编号。
        assert(powers.size() / (height + 1) < (size_t)numeric_limits<int>::max()); // 调试检查，可删
        int id = (int)(powers.size() / (height + 1));
        T cur = x;
        for (int level = 0; level <= height; level++)
        {
            powers.push_back(cur);
            if (level != height)
            {
                cur = op(cur, cur);
            }
        }
        return id;
    }

    void pull(int p)
    {
        // p 是内部节点；按原下标顺序合并两个孩子的积。
        tr[p].prod = op(tr[2 * p].prod, tr[2 * p + 1].prod);
    }

    void apply(int p, int level, int tag)
    {
        // p 是长度 2^level 的完整命中节点，tag 是统一赋值幂表编号；覆盖该段并记录标记。
        tr[p].prod = power(tag, level);
        tr[p].tag = tag;
    }

    void push(int p, int level)
    {
        // p 是长度 2^level 的当前节点；把待执行统一赋值按幂表编号下传给两个孩子。
        if (level == 0 || tr[p].tag < 0)
        {
            return;
        }
        apply(2 * p, level - 1, tr[p].tag);
        apply(2 * p + 1, level - 1, tr[p].tag);
        tr[p].tag = -1;
    }

    void build(const vector<T> &a)
    {
        // a 是完整初始序列；补齐幺元叶子后自底向上建立有序区间积。
        for (int i = 0; i < n; i++)
        {
            tr[base + i].prod = a[i];
        }
        for (int i = n; i < base; i++)
        {
            tr[base + i].prod = e;
        }
        for (int p = base - 1; p; p--)
        {
            pull(p);
        }
    }

    void assign(int p, int l, int r, int ql, int qr, int tag, int level)
    {
        // p、[l,r) 是长度 2^level 的当前节点，[ql,qr) 是赋值区间，tag 是统一新值幂表编号。
        if (qr <= l || r <= ql)
        {
            return;
        }
        if (ql <= l && r <= qr)
        {
            apply(p, level, tag);
            return;
        }
        push(p, level);
        int m = (l + r) / 2;
        assign(2 * p, l, m, ql, qr, tag, level - 1);
        assign(2 * p + 1, m, r, ql, qr, tag, level - 1);
        pull(p);
    }

    T prod(int p, int l, int r, int ql, int qr, int level)
    {
        // p、[l,r) 是当前节点；返回 [ql,qr) 与该节点交集按原顺序的积。
        if (qr <= l || r <= ql)
        {
            return e;
        }
        if (ql <= l && r <= qr)
        {
            return tr[p].prod;
        }
        push(p, level);
        int m = (l + r) / 2;
        return op(prod(2 * p, l, m, ql, qr, level - 1),
                  prod(2 * p + 1, m, r, ql, qr, level - 1));
    }

  public:
    RangeSetSeg(const vector<T> &a, T e, F op) : n(a.size()), e(move(e)), op(move(op))
    {
        // a 是初始序列，e 是运算幺元，op 是从左段到右段的结合运算；允许空序列。
        while (base < max<int>(n, 1))
        {
            base *= 2;
            height++;
        }
        tr.assign(2 * base, Node{});
        build(a);
    }

    void reserveTags(int cap)
    {
        // cap 是预计的非空赋值总次数；预留幂缓存容量，不改变结构语义。
        assert(cap >= 0); // 调试检查，可删
        powers.reserve((size_t)cap * (height + 1));
    }

    void assign(int l, int r, const T &x)
    {
        // l、r 是半开端点，x 是新值；把 [l,r) 每个元素统一赋为 x，空区间无操作。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (l == r)
        {
            return;
        }
        assign(1, 0, base, l, r, makeTag(x), height);
    }

    T prod(int l, int r)
    {
        // l、r 是半开端点；返回 [l,r) 的有序积，空区间返回 e。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (l == r)
        {
            return e;
        }
        return prod(1, 0, base, l, r, height);
    }
};
