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

template <class T, class F> class RangeSetSeg
{
    struct Node
    {
        // prod 是该区间按从左到右顺序合并的积，tag 是等待下传的统一赋值幂表编号，-1 表示无标记。
        T prod{};
        int32_t tag = -1;
    };

    // n 是实际序列长度，base 是补齐后的二次幂叶子数，height 是树高。
    int n = 0, base = 1, height = 0;
    T e;
    F op;
    vector<Node> tr;
    // powers 的第 id 块保存同一次赋值 x 的 x^(2^0),...,x^(2^height)。
    vector<T> powers;

    const T &power(int32_t id, int level) const
    {
        // id 是赋值幂表编号，level 对应长度 2^level；返回 x^(2^level)。
        return powers[(size_t)id * (height + 1) + level];
    }

    int32_t makeTag(const T &x)
    {
        // x 是一次新的统一赋值；建立按二次幂长度索引的重复积并返回编号。
        assert(powers.size() / (height + 1) < (size_t)numeric_limits<int32_t>::max()); // 调试检查，可删
        int32_t id = (int32_t)(powers.size() / (height + 1));
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

    void apply(int p, int level, int32_t tag)
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

    void assign(int p, int l, int r, int ql, int qr, int32_t tag, int level)
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

using Fn = array<Z, 2>;

struct Compose
{
    Fn operator()(const Fn &left, const Fn &right) const
    {
        return {left[0] * right[0], left[1] * right[0] + right[1]};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<Fn> a(n);
    for (auto &[mul, add] : a)
    {
        long long x, y;
        cin >> x >> y;
        mul = Z(x);
        add = Z(y);
    }
    RangeSetSeg<Fn, Compose> seg(a, Fn{Z(1), Z(0)}, Compose{});
    seg.reserveTags(q);
    while (q--)
    {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 0)
        {
            long long c, d;
            cin >> c >> d;
            seg.assign(l, r, Fn{Z(c), Z(d)});
        }
        else
        {
            long long x;
            cin >> x;
            Fn f = seg.prod(l, r);
            cout << (f[0] * Z(x) + f[1]).val() << '\n';
        }
    }
    return 0;
}
