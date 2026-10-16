
#include <bits/stdc++.h>
using namespace std;
#define int long long



#include <limits>

using namespace std;

constexpr long long INF = numeric_limits<long long>::max() / 4;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 b 更小则更新 a 并返回 true。
    if (b < a)
    {
        a = b;
        return true;
    }
    return false;
}

template <class T> bool chmax(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 b 更大则更新 a 并返回 true。
    if (a < b)
    {
        a = b;
        return true;
    }
    return false;
}

constexpr int inf = 1E9;
constexpr long double eps = 1E-12L;

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
    int len = 1;

    Info() = default; // 默认构造一个值为 0 的叶子信息
    Info(long long x) : sum(x), len(1)
    {
        // x 是叶子的初始值。
    }

    static Info identity()
    {
        // 无参数；返回长度为 0 的空区间信息，作为合并单位元。
        Info c;
        c.len = 0;
        return c;
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

    static Info identity()
    {
        // 无参数；Info 自带 identity() 时用它，否则用默认构造值作为合并单位元。
        if constexpr (requires { Info::identity(); })
            return Info::identity();
        else
            return Info{};
    }

    LazySeg(int n = 0)
    {
        // n 是叶子数；每个叶子由 Info{} 默认构造成初始信息。
        init(n);
    }
    LazySeg(const vector<Info> &a)
    {
        // a 是各叶子的初始信息。
        init(a);
    }

    void init(int n_)
    {
        // n_ 是新的叶子数；用 n_ 个 Info{} 重建默认数组。
        assert(n_ >= 0); // 调试检查，可删
        n = n_;
        tr.assign(4 * max<int>(n, 1), Info{});
        tag.assign(4 * max<int>(n, 1), Tag{});
        has.assign(4 * max<int>(n, 1), 0);
        if (n)
        {
            vector<Info> a(n);
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
            return identity();
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

    template <class F>
    int findFirst(int p, int l, int r, int ql, int qr, F &ok)
    {
        // p、[l,r) 是当前节点， [ql,qr) 是搜索区间；返回其中第一个满足条件的下标。
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
        push(p);
        int m = l + (r - l) / 2;
        int x = findFirst(2 * p, l, m, ql, qr, ok);
        return x == -1 ? findFirst(2 * p + 1, m, r, ql, qr, ok) : x;
    }

    template <class F>
    int findLast(int p, int l, int r, int ql, int qr, F &ok)
    {
        // p、[l,r) 是当前节点， [ql,qr) 是搜索区间；返回其中最后一个满足条件的下标。
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
        push(p);
        int m = l + (r - l) / 2;
        int x = findLast(2 * p + 1, m, r, ql, qr, ok);
        return x == -1 ? findLast(2 * p, l, m, ql, qr, ok) : x;
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

    template <class F>
    int findFirst(int l, int r, F ok)
    {
        // l、r 是半开搜索区间，ok 判断节点摘要是否可能包含答案；返回第一个满足条件的下标。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return l == r ? -1 : findFirst(1, 0, n, l, r, ok);
    }

    template <class F>
    int findLast(int l, int r, F ok)
    {
        // l、r 是半开搜索区间，ok 判断节点摘要是否可能包含答案；返回最后一个满足条件的下标。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return l == r ? -1 : findLast(1, 0, n, l, r, ok);
    }
};

// 依赖 3.3.002 懒标记线段树的 LazySeg 与区间加标记 Tag。
struct MinInfo
{
    long long mn = 0; // mn 是非空区间的最小值；默认构造一个值为 0 的叶子。

    MinInfo() = default;
    MinInfo(long long x) : mn(x)
    {
        // x 是叶子的初始值。
    }

    static MinInfo identity()
    {
        // 无参数；返回空区间的合并单位元，不向该临时结果施加区间加。
        return MinInfo(numeric_limits<long long>::max());
    }

    void apply(const Tag &t)
    {
        // t 是区间增量；统一平移非空区间的最小值。
        mn += t.add;
    }

    friend MinInfo operator+(MinInfo a, MinInfo b)
    {
        // a、b 是相邻区间的信息；返回合并后的最小值。
        return MinInfo(min(a.mn, b.mn));
    }
    // 改最大值时，将 mn/最小值/min/max() 成对换为 mx/最大值/max/lowest()。
    // 默认叶子仍为 0；Tag、增量符号与更新方向不变，不需要取反输入。
};

using AddMinSeg = LazySeg<MinInfo, Tag>;

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<MinInfo> a(n);
    for (auto &x : a)
    {
        cin >> x.mn;
    }
    AddMinSeg seg(a);
    while (q--)
    {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 0)
        {
            long long x;
            cin >> x;
            seg.apply(l, r, Tag{x});
        }
        else
        {
            cout << seg.query(l, r).mn << '\n';
        }
    }
    return 0;
}
