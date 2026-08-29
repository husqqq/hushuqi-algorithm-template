#pragma once
#include "3_3_002.hpp"

template <class T> struct AffineTag
{
    // mul、add 表示待执行变换 x -> mul*x+add。
    T mul = T(1), add{};

    // 已有变换后再执行 t。
    void apply(const AffineTag<T> &t)
    {
        // t 是后执行的仿射变换；把它复合到当前标记之后。
        mul *= t.mul;
        add = add * t.mul + t.add;
    }
};

template <class T> struct AffineInfo
{
    // sum 是区间和，len 是区间叶子数。
    T sum{};
    int len = 0;

    AffineInfo() = default;
    AffineInfo(const T &x) : sum(x), len(1)
    {
        // x 是单个叶子的初值。
    }

    AffineInfo(const T &s, int n) : sum(s), len(n)
    {
        // s 是当前区间元素和，n 是该区间叶子数。
    }

    void apply(const AffineTag<T> &t)
    {
        // t 是本次仿射标记；更新当前区间和。
        sum = sum * t.mul + T(len) * t.add;
    }

    friend AffineInfo operator+(AffineInfo a, AffineInfo b)
    {
        // a、b 是相邻区间信息；返回合并结果。
        AffineInfo c;
        c.sum = a.sum + b.sum;
        c.len = a.len + b.len;
        return c;
    }
};

template <class T> class AffPointSeg
{
    // n 是叶子数；a 保存初始点值；tag 和 has 保存尚未下传的仿射修改。
    int n;
    vector<T> a;
    vector<AffineTag<T>> tag;
    vector<unsigned char> has;

    void apply(int p, const AffineTag<T> &v)
    {
        // p 是完整命中节点，v 是后执行的仿射变换；只复合该段延迟标记。
        tag[p].apply(v);
        has[p] = 1;
    }

    void push(int p)
    {
        // p 是内部节点；把较早的整段修改下传后，才允许对子段施加较晚修改。
        if (!has[p])
        {
            return;
        }
        apply(2 * p, tag[p]);
        apply(2 * p + 1, tag[p]);
        tag[p] = AffineTag<T>{};
        has[p] = 0;
    }

    void affine(int p, int l, int r, int ql, int qr, const AffineTag<T> &v)
    {
        // p、[l,r) 是当前节点，[ql,qr) 是修改区间，v 是后执行的仿射变换。
        if (qr <= l || r <= ql)
        {
            return;
        }
        if (ql <= l && r <= qr)
        {
            apply(p, v);
            return;
        }
        push(p);
        int m = (l + r) / 2;
        affine(2 * p, l, m, ql, qr, v);
        affine(2 * p + 1, m, r, ql, qr, v);
    }

    T get(int p, int l, int r, int i) const
    {
        // p、[l,r) 是当前节点，i 是目标叶子下标；只读返回所有仿射修改后的点值。
        T ans;
        if (r - l == 1)
        {
            ans = a[i];
        }
        else
        {
            int m = (l + r) / 2;
            ans = i < m ? get(2 * p, l, m, i) : get(2 * p + 1, m, r, i);
        }
        if (has[p])
        {
            ans = ans * tag[p].mul + tag[p].add;
        }
        return ans;
    }

  public:
    AffPointSeg(vector<T> a) : n(a.size()), a(move(a))
    {
        // a 是初始点值；构造仅分配延迟标记，不维护区间摘要。
        tag.assign(4 * max<int>(n, 1), AffineTag<T>{});
        has.assign(4 * max<int>(n, 1), 0);
    }

    void affine(int l, int r, const T &mul, const T &add)
    {
        // l、r 是半开端点；把 [l,r) 内每个值变为 value*mul+add。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (l == r)
        {
            return;
        }
        affine(1, 0, n, l, r, {mul, add});
    }

    T get(int i) const
    {
        // i 是叶子下标；只读返回该点当前值。
        assert(0 <= i && i < n); // 调试检查，可删
        return get(1, 0, n, i);
    }
};

// a 是初始叶子信息。
// LazySeg<AffineInfo<Z>, AffineTag<Z>> seg(a);
// seg.apply(l, r, {Z(1), x}); // 给区间加 x。
// seg.apply(l, r, {x, Z(0)}); // 给区间乘 x。
// seg.apply(l, r, {Z(0), x}); // 把区间赋值为 x。
// Z ans = seg.query(l, r).sum; // 查询区间和。

class HistMaxSeg
{
  public:
    struct Info
    {
        // cur、hist 分别是区间当前最大值和历次修改后的历史最大值。
        long long cur, hist;
    };

  private:
    static constexpr long long neg = numeric_limits<long long>::lowest() / 2;
    struct Tag
    {
        // cc、ch、hh、vc、vh 描述 max-plus 变换 (cur,hist,1) 的五个有限系数。
        long long cc = 0, ch = neg, hh = 0, vc = neg, vh = neg;
    };
    int n;
    vector<Info> tr;
    vector<Tag> lz;

    static long long plus(long long a, long long b)
    {
        // a、b 是普通值或负无穷；返回 max-plus 乘法。
        return a == neg || b == neg ? neg : a + b;
    }

    static Tag compose(const Tag &a, const Tag &b)
    {
        // a、b 是先后执行的标记；返回先 a 后 b 的复合标记。
        return {plus(a.cc, b.cc),
                max(plus(a.cc, b.ch), plus(a.ch, b.hh)),
                plus(a.hh, b.hh),
                max(plus(a.vc, b.cc), b.vc),
                max({plus(a.vh, b.hh), plus(a.vc, b.ch), b.vh})};
    }

    static Info apply(Info a, const Tag &t)
    {
        // a 是节点摘要，t 是区间标记；返回变换后的摘要。
        return {max(plus(a.cur, t.cc), t.vc),
                max({plus(a.cur, t.ch), plus(a.hist, t.hh), t.vh})};
    }

    void pull(int p)
    {
        // p 是内部节点；由两个儿子重算摘要。
        tr[p] = {max(tr[2 * p].cur, tr[2 * p + 1].cur),
                 max(tr[2 * p].hist, tr[2 * p + 1].hist)};
    }

    void build(int p, int l, int r, const vector<long long> &a)
    {
        // p、[l,r) 是当前节点，a 是初始数组。
        if (r - l == 1) return void(tr[p] = {a[l], a[l]});
        int m = (l + r) / 2;
        build(2 * p, l, m, a), build(2 * p + 1, m, r, a), pull(p);
    }

    void apply(int p, const Tag &t)
    {
        // p 是整段命中节点，t 是后执行的标记。
        tr[p] = apply(tr[p], t);
        lz[p] = compose(lz[p], t);
    }

    void push(int p)
    {
        // p 是内部节点；把累计标记下传。
        apply(2 * p, lz[p]), apply(2 * p + 1, lz[p]), lz[p] = {};
    }

    void update(int p, int l, int r, int ql, int qr, const Tag &t)
    {
        // p、[l,r) 是当前节点，[ql,qr) 是修改区间，t 是修改标记。
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) return apply(p, t);
        push(p);
        int m = (l + r) / 2;
        update(2 * p, l, m, ql, qr, t), update(2 * p + 1, m, r, ql, qr, t), pull(p);
    }

    Info query(int p, int l, int r, int ql, int qr)
    {
        // p、[l,r) 是当前节点，[ql,qr) 是询问区间；返回相交部分摘要。
        if (qr <= l || r <= ql) return {neg, neg};
        if (ql <= l && r <= qr) return tr[p];
        push(p);
        int m = (l + r) / 2;
        Info a = query(2 * p, l, m, ql, qr), b = query(2 * p + 1, m, r, ql, qr);
        return {max(a.cur, b.cur), max(a.hist, b.hist)};
    }

  public:
    explicit HistMaxSeg(const vector<long long> &a) : n(a.size()), tr(4 * max<int>(n, 1), {neg, neg}), lz(4 * max<int>(n, 1))
    {
        // a 是非空初始数组；建立当前最大值与历史最大值线段树。
        assert(n > 0); // 调试检查，可删
        build(1, 0, n, a);
    }

    void add(int l, int r, long long v)
    {
        // l、r 是半开端点，v 是增量；给区间加 v 并记录修改后的历史峰值。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        update(1, 0, n, l, r, {v, v, 0, neg, neg});
    }

    void assign(int l, int r, long long v)
    {
        // l、r 是半开端点，v 是新值；给区间赋值并记录修改后的历史峰值。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        update(1, 0, n, l, r, {neg, neg, 0, v, v});
    }

    Info query(int l, int r)
    {
        // l、r 是非空半开端点；返回区间当前最大值和历史最大值。
        assert(0 <= l && l < r && r <= n); // 调试检查，可删
        return query(1, 0, n, l, r);
    }
};
