#pragma once
#include "3_3_003.hpp"

template <class T> class SparseAffSeg
{
    struct Node
    {
        // l、r 是孩子编号，info 是该完整区间的和与长度，tag 是尚未下传的仿射变换。
        int32_t l = 0, r = 0;
        AffineInfo<T> info;
        AffineTag<T> tag;
    };

    // lo、hi 是总半开值域，rt 是根编号，t 是动态节点池。
    long long lo, hi;
    int32_t rt = 0;
    vector<Node> t{Node{}};

    int32_t newNode(long long l, long long r)
    {
        // l、r 是新节点覆盖的非空半开区间；建立全零区间及恒等仿射标记。
        Node q;
        q.info = AffineInfo<T>(T{}, r - l);
        t.push_back(move(q));
        assert(t.size() <= (size_t)numeric_limits<int32_t>::max()); // 调试检查，可删
        return (int32_t)t.size() - 1;
    }

    bool idle(int32_t p) const
    {
        // p 是非空节点；返回其延迟标记是否为恒等变换。
        return t[p].tag.mul == T(1) && t[p].tag.add == T{};
    }

    void apply(int32_t p, long long, long long, const AffineTag<T> &tag)
    {
        // p、[l,r) 是完整命中节点，tag 是后执行的仿射变换；原地更新摘要和延迟标记。
        t[p].info.apply(tag);
        t[p].tag.apply(tag);
    }

    void push(int32_t p, long long l, long long r)
    {
        // p、[l,r) 是内部节点；若有标记则按持久化无关的普通方式下传给两个孩子。
        if (r - l == 1 || idle(p))
        {
            return;
        }
        long long m = l + (r - l) / 2;
        AffineTag<T> tag = t[p].tag;
        int32_t a = t[p].l;
        int32_t b = t[p].r;
        if (!a)
        {
            a = newNode(l, m);
        }
        if (!b)
        {
            b = newNode(m, r);
        }
        apply(a, l, m, tag);
        apply(b, m, r, tag);
        t[p].l = a;
        t[p].r = b;
        t[p].tag = AffineTag<T>{};
    }

    void pull(int32_t p, long long l, long long r)
    {
        // p 是已下传标记的内部节点，[l,r) 是其区间；由孩子的和重算摘要。
        T a = t[p].l ? t[t[p].l].info.sum : T{};
        T b = t[p].r ? t[t[p].r].info.sum : T{};
        t[p].info = AffineInfo<T>(a + b, r - l);
    }

    int32_t affine(int32_t p, long long l, long long r, long long ql, long long qr,
                   const AffineTag<T> &tag)
    {
        // p、[l,r) 是当前节点，[ql,qr) 是修改区间，tag 是后执行变换；返回更新后的根。
        if (qr <= l || r <= ql)
        {
            return p;
        }
        if (!p)
        {
            p = newNode(l, r);
        }
        if (ql <= l && r <= qr)
        {
            apply(p, l, r, tag);
            return p;
        }
        push(p, l, r);
        long long m = l + (r - l) / 2;
        int32_t a = affine(t[p].l, l, m, ql, qr, tag);
        int32_t b = affine(t[p].r, m, r, ql, qr, tag);
        t[p].l = a;
        t[p].r = b;
        pull(p, l, r);
        return p;
    }

    T sum(int32_t p, long long l, long long r, long long ql, long long qr,
          const AffineTag<T> &outer) const
    {
        // p、[l,r) 是当前节点，[ql,qr) 是查询区间，outer 是祖先尚未下传且后执行的变换。
        if (qr <= l || r <= ql)
        {
            return T{};
        }
        long long a = max(l, ql), b = min(r, qr);
        if (!p)
        {
            // 空子树原值全为零，仍要接受祖先已经施加的常数项。
            return T(b - a) * outer.add;
        }
        if (ql <= l && r <= qr)
        {
            return t[p].info.sum * outer.mul + T(r - l) * outer.add;
        }
        AffineTag<T> next = t[p].tag;
        next.apply(outer);
        long long m = l + (r - l) / 2;
        return sum(t[p].l, l, m, ql, qr, next)
             + sum(t[p].r, m, r, ql, qr, next);
    }

  public:
    SparseAffSeg(long long lo, long long hi) : lo(lo), hi(hi)
    {
        // lo、hi 是非空半开整数值域；初始所有位置为零。
        assert(lo < hi); // 调试检查，可删
    }

    void reserveNodes(int cap)
    {
        // cap 是预计节点数；预留节点池容量，不改变结构语义。
        assert(cap >= 0); // 调试检查，可删
        t.reserve(cap + 1);
    }

    void affine(long long l, long long r, const T &mul, const T &add)
    {
        // l、r 是值域内半开端点，mul、add 表示 x->x*mul+add；原地修改 [l,r)。
        assert(lo <= l && l <= r && r <= hi); // 调试检查，可删
        if (l == r)
        {
            return;
        }
        rt = affine(rt, lo, hi, l, r, AffineTag<T>{mul, add});
    }

    T sum(long long l, long long r) const
    {
        // l、r 是值域内半开端点；返回 [l,r) 的元素和，空区间返回零。
        assert(lo <= l && l <= r && r <= hi); // 调试检查，可删
        return sum(rt, lo, hi, l, r, AffineTag<T>{});
    }
};
