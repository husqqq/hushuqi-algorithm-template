#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // 若 b 更小则更新 a，返回是否更新。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // 若 b 更大则更新 a，返回是否更新。
    return a < b ? a = b, true : false;
}

class AddMinSeg
{
    // n 是数组长度；mn、lz 分别保存区间最小值与待下传增量。
    int n;
    vector<long long> mn, lz;

    void build(int p, int l, int r, const vector<long long> &a)
    {
        // p 是当前节点，[l,r) 是其区间，a 是初始数组。
        if (r - l == 1)
        {
            mn[p] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(2 * p, l, m, a);
        build(2 * p + 1, m, r, a);
        pull(p);
    }

    void apply(int p, long long v)
    {
        // p 是整段命中的节点，v 是区间增量。
        mn[p] += v;
        lz[p] += v;
    }

    void push(int p)
    {
        // p 是当前节点；把待执行增量下传给两个儿子。
        if (!lz[p])
        {
            return;
        }
        apply(2 * p, lz[p]);
        apply(2 * p + 1, lz[p]);
        lz[p] = 0;
    }

    void pull(int p)
    {
        // p 是内部节点；由两个儿子的最小值重算 mn[p]。
        mn[p] = min(mn[2 * p], mn[2 * p + 1]);
    }

    void add(int p, int l, int r, int ql, int qr, long long v)
    {
        // p、[l,r) 是当前节点，[ql,qr) 是修改区间，v 是增量。
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
        add(2 * p, l, m, ql, qr, v);
        add(2 * p + 1, m, r, ql, qr, v);
        pull(p);
    }

    long long rangeMin(int p, int l, int r, int ql, int qr)
    {
        // p、[l,r) 是当前节点；返回 [ql,qr) 内的最小值。
        if (qr <= l || r <= ql)
        {
            return numeric_limits<long long>::max();
        }
        if (ql <= l && r <= qr)
        {
            return mn[p];
        }
        push(p);
        int m = (l + r) / 2;
        return min(rangeMin(2 * p, l, m, ql, qr), rangeMin(2 * p + 1, m, r, ql, qr));
    }

  public:
    explicit AddMinSeg(const vector<long long> &a)
        : n(a.size()), mn(4 * max<int>(n, 1)), lz(4 * max<int>(n, 1))
    {
        // a 是初始数组；空数组建立空结构。
        if (n)
        {
            build(1, 0, n, a);
        }
    }

    void add(int l, int r, long long v)
    {
        // 给半开区间 [l,r) 增加 v；空区间无操作。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (l == r)
        {
            return;
        }
        add(1, 0, n, l, r, v);
    }

    long long rangeMin(int l, int r)
    {
        // 返回非空半开区间 [l,r) 的最小值。
        assert(0 <= l && l < r && r <= n); // 调试检查，可删
        return rangeMin(1, 0, n, l, r);
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a)
    {
        cin >> x;
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
            seg.add(l, r, x);
        }
        else
        {
            cout << seg.rangeMin(l, r) << '\n';
        }
    }
    return 0;
}
