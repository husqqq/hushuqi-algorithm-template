#include <bits/stdc++.h>
using namespace std;
#define int long long

template <long long P> struct MInt
{
    static_assert(P > 1);
    using V = conditional_t<(P <= numeric_limits<int32_t>::max()),
                            uint32_t, unsigned long long>;
    static constexpr V M = (V)P; // 与 x 同宽的模数，避免热路径被 P 提升为有符号 64 位
    V x = 0; // 当前剩余类在 [0,P) 内的代表元；常见小模使用无符号 32 位

    MInt() = default;

    MInt(long long v)
    {
        // v 是要转入模 P 剩余类的整数；构造其最小非负代表元。
        v %= P;
        if (v < 0)
        {
            v += P;
        }
        x = (V)v;
    }

    static constexpr long long mod()
    {
        // 无参数；返回编译期模数 P。
        return P;
    }

    long long val() const
    {
        // 无参数；返回当前剩余类的最小非负代表元。
        return x;
    }

    MInt operator-() const
    {
        // 无参数；返回当前剩余类的加法逆元。
        MInt ans;
        ans.x = x ? M - x : 0;
        return ans;
    }

    MInt &operator+=(const MInt &o)
    {
        // o 是要加到当前值上的同模剩余类；原地完成模加并返回当前对象引用。
        x += o.x;
        if (x >= M)
        {
            x -= M;
        }
        return *this;
    }

    MInt &operator-=(const MInt &o)
    {
        // o 是要从当前值减去的同模剩余类；原地完成模减并返回当前对象引用。
        x += M - o.x;
        if (x >= M)
        {
            x -= M;
        }
        return *this;
    }

    MInt &operator*=(const MInt &o)
    {
        // o 是要乘到当前值上的同模剩余类；原地完成模乘并返回当前对象引用。
        if constexpr (P <= 2147483647LL)
        {
            x = (unsigned long long)x * o.x % M;
        }
        else
        {
            x = (unsigned __int128)x * o.x % M;
        }
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

    MInt pow(unsigned long long b) const
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

template <long long P> const vector<MInt<P>> &invTable(int n)
{
    // n 是需要的最大下标且 0<=n<P；返回至少覆盖 0..n 的共享模逆元表，其中位置 0 为 0。
    assert(0 <= n && n < P); // 调试检查，可删
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

constexpr int mod = 998244353;
using Z = MInt<mod>;
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

template <class T, class F> class SparsePtSeg
{
    struct Node
    {
        // l、r 是孩子编号，pos 是本节点实际保存的位置，x、prod 分别是该点值与子树按下标顺序的积。
        int32_t l = 0, r = 0;
        long long pos = 0;
        T x{}, prod{};
    };

    // lo、hi 是总半开值域，rt 是根编号，e 是空位置值，op 是按左到右合并的结合运算，t 是节点池。
    long long lo, hi;
    int32_t rt = 0;
    T e;
    F op;
    vector<Node> t{Node{}};

    int32_t newNode(long long x, const T &v)
    {
        // x 是新节点位置，v 是该点值；新建只保存此点的节点并返回编号。
        Node q;
        q.pos = x;
        q.x = q.prod = v;
        t.push_back(move(q));
        assert(t.size() <= (size_t)numeric_limits<int32_t>::max()); // 调试检查，可删
        return (int32_t)t.size() - 1;
    }

    void pull(int32_t p)
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

    int32_t set(int32_t p, long long l, long long r, long long x, T v)
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
            int32_t a = set(t[p].l, l, m, x, move(v));
            t[p].l = a;
        }
        else
        {
            if (x < t[p].pos)
            {
                swap(t[p].pos, x);
                swap(t[p].x, v);
            }
            int32_t b = set(t[p].r, m, r, x, move(v));
            t[p].r = b;
        }
        pull(p);
        return p;
    }

    void prod(int32_t p, long long l, long long r, long long ql, long long qr, T &ans) const
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
        int32_t p = rt;
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

struct Aff
{
    Z a = 1, b = 0;
};

struct Compose
{
    Aff operator()(const Aff &f, const Aff &g) const
    {
        return {g.a * f.a, g.a * f.b + g.b};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    SparsePtSeg<Aff, Compose> st(0, n, Aff{}, Compose{});
    st.reserveNodes(q);
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 0)
        {
            int p, a, b;
            cin >> p >> a >> b;
            st.set(p, {Z(a), Z(b)});
        }
        else
        {
            int l, r, x;
            cin >> l >> r >> x;
            auto f = st.prod(l, r);
            cout << (f.a * Z(x) + f.b).val() << '\n';
        }
    }
    return 0;
}
