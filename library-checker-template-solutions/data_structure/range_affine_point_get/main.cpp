#ifndef HUSHUQI_STATIC_MINT_HPP
#define HUSHUQI_STATIC_MINT_HPP
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <uint32_t P> struct MInt
{
    static_assert(1 < P && P <= 1000000007);
    uint32_t x = 0; // 固定 32 位以保证对象为 4 字节，提高大数组的缓存密度。

    MInt(long long v = 0)
    {
        // v 是要转入模 P 剩余类的整数；构造其最小非负代表元。
        v %= (long long)P;
        if (v < 0)
        {
            v += P;
        }
        x = (uint32_t)v;
    }

    static constexpr uint32_t mod()
    {
        // 无参数；返回编译期模数 P。
        return P;
    }

    uint32_t val() const
    {
        // 无参数；返回当前剩余类的最小非负代表元。
        return x;
    }

    MInt operator-() const
    {
        // 无参数；返回当前剩余类的加法逆元。
        MInt ans;
        ans.x = x ? P - x : 0;
        return ans;
    }

    MInt &operator+=(const MInt &o)
    {
        // o 是要加到当前值上的同模剩余类；原地完成模加并返回当前对象引用。
        x += o.x;
        if (x >= P)
        {
            x -= P;
        }
        return *this;
    }

    MInt &operator-=(const MInt &o)
    {
        // o 是要从当前值减去的同模剩余类；原地完成模减并返回当前对象引用。
        x += P - o.x;
        if (x >= P)
        {
            x -= P;
        }
        return *this;
    }

    MInt &operator*=(const MInt &o)
    {
        // o 是要乘到当前值上的同模剩余类；原地完成模乘并返回当前对象引用。
        // 乘积可能超过 32 位，显式提升到无符号 64 位。
        x = (uint64_t)x * o.x % P;
        return *this;
    }

    MInt &operator/=(const MInt &o)
    {
        // o 是非零同模剩余类且 P 必须为素数；原地乘以 o 的逆元并返回当前对象引用。
        return *this *= o.inv();
    }

    friend MInt operator+(MInt a, const MInt &b)
    {
        // a、b 是同模剩余类；返回 a+b。
        return a += b;
    }

    friend MInt operator-(MInt a, const MInt &b)
    {
        // a、b 是同模剩余类；返回 a-b。
        return a -= b;
    }

    friend MInt operator*(MInt a, const MInt &b)
    {
        // a、b 是同模剩余类；返回 a*b。
        return a *= b;
    }

    friend MInt operator/(MInt a, const MInt &b)
    {
        // a、b 是同模剩余类且 b 非零；返回 a/b。
        return a /= b;
    }

    friend bool operator==(const MInt &, const MInt &) = default;

    MInt pow(uint64_t b) const
    {
        // b 是非负指数；返回当前剩余类的 b 次幂。
        MInt a = *this;
        MInt ans = 1;
        while (b)
        {
            if (b & 1)
            {
                ans *= a;
            }
            a *= a;
            b >>= 1;
        }
        return ans;
    }

    MInt inv() const
    {
        // 当前值必须非零且 P 必须为素数；返回乘法逆元。
        assert(x != 0); // 调试检查，可删
        return pow(P - 2);
    }
};

template <uint32_t P> const vector<MInt<P>> &invTable(int n)
{
    // n 是需要的最大下标且 0<=n<P；返回至少覆盖 0..n 的共享模逆元表，其中位置 0 为 0。
    assert(0 <= n && (uint64_t)n < P); // 调试检查，可删
    static vector<MInt<P>> iv{0, 1};
    int old = iv.size();
    if (old <= n)
    {
        iv.resize(n + 1);
        for (int i = old; i <= n; i++)
        {
            iv[i] = MInt<P>(0) - MInt<P>(P / i) * iv[P % i];
        }
    }
    return iv;
}

constexpr uint32_t mod = 998244353;
using Z = MInt<mod>;

#endif
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
    int n; // n 是叶子数。
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

class HistMaxSeg
{
  public:
    struct Info
    {
        // cur、hist 分别是区间当前最大值和历次修改后的历史最大值。
        long long cur, hist;
    };

  private:
    static constexpr long long neg = numeric_limits<long long>::lowest() / 2; // neg 表示 max-plus 意义下的负无穷，取一半防止相加溢出。
    struct Tag
    {
        // cc、ch、hh、vc、vh 描述 max-plus 变换 (cur,hist,1) 的五个有限系数。
        long long cc = 0, ch = neg, hh = 0, vc = neg, vh = neg;
    };
    int n; // n 是叶子数。
    vector<Info> tr; // tr[p] 是节点 p 区间的当前最大值与历史最大值。
    vector<Tag> lz; // lz[p] 是节点 p 尚未下传给儿子的复合标记。

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

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<Z> a;
    a.reserve(n);
    for (int i = 0; i < n; i++)
    {
        long long x;
        cin >> x;
        a.emplace_back(Z(x));
    }
    AffPointSeg<Z> seg(move(a));
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 0)
        {
            int l, r;
            long long b, c;
            cin >> l >> r >> b >> c;
            seg.affine(l, r, Z(b), Z(c));
        }
        else
        {
            int i;
            cin >> i;
            cout << seg.get(i).val() << '\n';
        }
    }
    return 0;
}
