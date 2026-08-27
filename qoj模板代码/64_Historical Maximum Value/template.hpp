// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 64 Historical Maximum Value

#include <bits/stdc++.h>
using namespace std;
#define int long long

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

// 区间加、区间和实例。换题时通常只需要修改这两个结构。
struct Tag
{
    // add 是尚未下传的区间增量。
    long long add = 0;

    // 先前标记后再执行 t：区间加直接累加。
    void apply(const Tag &t)
    {
        // t 是后执行的区间加标记；与当前标记复合。
        add += t.add;
    }
};

struct Info
{
    // sum 是区间和，len 是区间叶子数。
    long long sum = 0;
    int len = 0;

    Info() = default; // 查询越界时的合并单位元
    Info(long long x) : sum(x), len(1)
    {
        // x 是叶子的初始值。
    }

    // 把区间加标记作用到当前节点。
    void apply(const Tag &t)
    {
        // t 是区间加标记；更新当前区间和。
        sum += t.add * len;
    }

    friend Info operator+(Info a, Info b)
    {
        // a、b 是相邻区间信息；返回合并结果。
        Info c;
        c.sum = a.sum + b.sum;
        c.len = a.len + b.len;
        return c;
    }
};

template <class Info, class Tag> struct LazySeg
{
    // n 是叶子数；tr 保存已计入本节点修改的区间信息，tag 和 has 保存尚未下传给儿子的修改。
    int n;
    vector<Info> tr;
    vector<Tag> tag;
    vector<unsigned char> has;

    LazySeg(int n = 0)
    {
        // n 是叶子数；每个叶子由 Info(0) 构造成零值信息。
        init(n);
    }
    LazySeg(const vector<Info> &a)
    {
        // a 是各叶子的初始信息。
        init(a);
    }

    void init(int n_)
    {
        // n_ 是新的叶子数；用 n_ 个 Info(0) 重建全零数组。
        assert(n_ >= 0); // 调试检查，可删
        n = n_;
        tr.assign(4 * max<int>(n, 1), Info{});
        tag.assign(4 * max<int>(n, 1), Tag{});
        has.assign(4 * max<int>(n, 1), 0);
        if (n)
        {
            vector<Info> a(n, Info(0));
            build(1, 0, n, a);
        }
    }

    void init(const vector<Info> &a)
    {
        // a 是新的叶子信息；清空旧状态并重建。
        n = a.size();
        tr.assign(4 * max<int>(n, 1), Info{});
        tag.assign(4 * max<int>(n, 1), Tag{});
        has.assign(4 * max<int>(n, 1), 0);
        if (n)
        {
            build(1, 0, n, a);
        }
    }

    void build(int p, int l, int r, const vector<Info> &a)
    {
        // p 是当前节点，[l,r) 是其区间，a 是叶子信息。
        if (r - l == 1)
        {
            tr[p] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(2 * p, l, m, a);
        build(2 * p + 1, m, r, a);
        pull(p);
    }

    void pull(int p)
    {
        // p 是内部节点编号；按左右顺序合并两个儿子。
        tr[p] = tr[2 * p] + tr[2 * p + 1];
    }

    void apply(int p, const Tag &v)
    {
        // p 是整段命中的节点，v 是本次区间修改标记。
        tr[p].apply(v);
        tag[p].apply(v);
        has[p] = 1;
    }

    void push(int p)
    {
        // p 是当前节点；仅在有待执行标记时把它下传给两个儿子。
        if (!has[p])
        {
            return;
        }
        apply(2 * p, tag[p]);
        apply(2 * p + 1, tag[p]);
        tag[p] = Tag{};
        has[p] = 0;
    }

    void apply(int p, int l, int r, int ql, int qr, const Tag &v)
    {
        // p、[l,r) 是当前节点， [ql,qr) 是修改区间，v 是本次标记。
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
        apply(2 * p, l, m, ql, qr, v);
        apply(2 * p + 1, m, r, ql, qr, v);
        pull(p);
    }

    Info query(int p, int l, int r, int ql, int qr)
    {
        // p、[l,r) 是当前节点， [ql,qr) 是询问区间；返回相交部分的信息。
        if (qr <= l || r <= ql)
        {
            return Info{};
        }
        if (ql <= l && r <= qr)
        {
            return tr[p];
        }
        push(p);
        int m = (l + r) / 2;
        return query(2 * p, l, m, ql, qr) + query(2 * p + 1, m, r, ql, qr);
    }

    Info get(int p, int l, int r, int i) const
    {
        // p、[l,r) 是当前节点，i 是目标叶子下标；只读返回该叶子的最新信息。
        if (r - l == 1)
        {
            return tr[p];
        }
        int m = (l + r) / 2;
        Info ans;
        if (i < m)
        {
            ans = get(2 * p, l, m, i);
        }
        else
        {
            ans = get(2 * p + 1, m, r, i);
        }
        if (has[p])
        {
            ans.apply(tag[p]);
        }
        return ans;
    }

    void apply(int l, int r, const Tag &v)
    {
        // l、r 是半开区间端点，v 是区间修改标记。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        apply(1, 0, n, l, r, v);
    }
    Info query(int l, int r)
    {
        // l、r 是半开区间端点；返回 [l,r) 的信息。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return query(1, 0, n, l, r);
    }
    Info get(int i) const
    {
        // i 是叶子下标；只读返回该位置在所有待下传标记生效后的信息。
        assert(0 <= i && i < n); // 调试检查，可删
        return get(1, 0, n, i);
    }
};

// a 是初始叶子信息。
// LazySeg<Info, Tag> seg(a);
// seg.apply(1, 4, Tag{2}); // 给 [1,4) 的每个数加 2。
// long long ans = seg.query(0, 4).sum; // 查询 [0,4) 的区间和。


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
