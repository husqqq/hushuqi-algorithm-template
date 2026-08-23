#include <bits/stdc++.h>
using namespace std;
#define int long long

struct SegBeats
{
    struct Node
    {
        // sum 是区间和，add 是待下传增量。
        long long sum = 0, add = 0;
        // mx、mn 是最值；mx2、mn2 仅在对应 has 标志为真时保存严格次值。
        long long mx = 0, mx2 = 0, mn = 0, mn2 = 0;
        bool hasMx2 = false, hasMn2 = false;
        // mxc、mnc 是最值出现次数，len 是区间长度。
        int mxc = 0, mnc = 0, len = 0;
    };

    // n 是数组长度，tr 保存各线段树节点的信息。
    int n;
    vector<Node> tr;

    SegBeats(const vector<long long> &a) : n(a.size()), tr(4 * n)
    {
        // a 是非空初始数组。
        assert(n); // 调试检查，可删
        build(1, 0, n, a);
    }

    void add(int l, int r, long long x)
    {
        // 给半开区间 [l,r) 增加 x。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        add(1, 0, n, l, r, x);
    }
    void chmin(int l, int r, long long x)
    {
        // 把半开区间 [l,r) 内每个值改成 min(value,x)。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        chmin(1, 0, n, l, r, x);
    }
    void chmax(int l, int r, long long x)
    {
        // 把半开区间 [l,r) 内每个值改成 max(value,x)。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        chmax(1, 0, n, l, r, x);
    }
    long long sum(int l, int r)
    {
        // 返回半开区间 [l,r) 的和，空区间返回 0。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return sum(1, 0, n, l, r);
    }
    long long min(int l, int r)
    {
        // 返回非空半开区间 [l,r) 的最小值。
        assert(0 <= l && l < r && r <= n); // 调试检查，可删
        return min(1, 0, n, l, r);
    }
    long long max(int l, int r)
    {
        // 返回非空半开区间 [l,r) 的最大值。
        assert(0 <= l && l < r && r <= n); // 调试检查，可删
        return max(1, 0, n, l, r);
    }

  private:
    static Node merge(const Node &a, const Node &b)
    {
        // a、b 是相邻区间节点；返回合并后的 Beats 信息。
        Node c;
        c.sum = a.sum + b.sum;
        c.len = a.len + b.len;
        c.mx = a.mx < b.mx ? b.mx : a.mx;
        c.mxc = (a.mx == c.mx ? a.mxc : 0) + (b.mx == c.mx ? b.mxc : 0);
        auto takeMax = [&](long long x)
        {
            if (x != c.mx && (!c.hasMx2 || c.mx2 < x))
            {
                c.mx2 = x;
                c.hasMx2 = true;
            }
        };
        takeMax(a.mx);
        takeMax(b.mx);
        if (a.hasMx2)
        {
            takeMax(a.mx2);
        }
        if (b.hasMx2)
        {
            takeMax(b.mx2);
        }
        c.mn = a.mn < b.mn ? a.mn : b.mn;
        c.mnc = (a.mn == c.mn ? a.mnc : 0) + (b.mn == c.mn ? b.mnc : 0);
        auto takeMin = [&](long long x)
        {
            if (x != c.mn && (!c.hasMn2 || x < c.mn2))
            {
                c.mn2 = x;
                c.hasMn2 = true;
            }
        };
        takeMin(a.mn);
        takeMin(b.mn);
        if (a.hasMn2)
        {
            takeMin(a.mn2);
        }
        if (b.hasMn2)
        {
            takeMin(b.mn2);
        }
        return c;
    }

    void build(int p, int l, int r, const vector<long long> &a)
    {
        // p 是当前节点，[l,r) 是其区间，a 是初始数组。
        if (r - l == 1)
        {
            tr[p].sum = tr[p].mx = tr[p].mn = a[l];
            tr[p].mxc = tr[p].mnc = tr[p].len = 1;
            return;
        }
        int m = (l + r) / 2;
        build(2 * p, l, m, a);
        build(2 * p + 1, m, r, a);
        pull(p);
    }

    void pull(int p)
    {
        // p 是内部节点；合并两个儿子的最值、次值、次数与区间和。
        tr[p] = merge(tr[2 * p], tr[2 * p + 1]);
    }

    void applyAdd(int p, long long x)
    {
        // p 是整段命中的节点，x 是区间增量。
        auto &v = tr[p];
        v.sum += x * v.len;
        v.mx += x;
        v.mn += x;
        if (v.hasMx2)
        {
            v.mx2 += x;
        }
        if (v.hasMn2)
        {
            v.mn2 += x;
        }
        v.add += x;
    }

    void applyChmin(int p, long long x)
    {
        // p 是可整段处理的节点，x 是上界；仅把当前最大值压到 x。
        auto &v = tr[p];
        if (v.mx <= x)
        {
            return;
        }
        v.sum += (x - v.mx) * v.mxc;
        long long old = v.mx;
        if (v.mn == old)
        {
            v.mn = x;
        }
        else if (v.hasMn2 && v.mn2 == old)
        {
            v.mn2 = x;
        }
        v.mx = x;
    }

    void applyChmax(int p, long long x)
    {
        // p 是可整段处理的节点，x 是下界；仅把当前最小值抬到 x。
        auto &v = tr[p];
        if (v.mn >= x)
        {
            return;
        }
        v.sum += (x - v.mn) * v.mnc;
        long long old = v.mn;
        if (v.mx == old)
        {
            v.mx = x;
        }
        else if (v.hasMx2 && v.mx2 == old)
        {
            v.mx2 = x;
        }
        v.mn = x;
    }

    void push(int p)
    {
        // p 是当前节点；依次下传区间加和父节点的最值界。
        if (tr[p].add)
        {
            applyAdd(2 * p, tr[p].add);
            applyAdd(2 * p + 1, tr[p].add);
            tr[p].add = 0;
        }
        applyChmin(2 * p, tr[p].mx);
        applyChmin(2 * p + 1, tr[p].mx);
        applyChmax(2 * p, tr[p].mn);
        applyChmax(2 * p + 1, tr[p].mn);
    }

    void add(int p, int l, int r, int ql, int qr, long long x)
    {
        // p、[l,r) 是当前节点， [ql,qr) 是修改区间，x 是增量。
        if (qr <= l || r <= ql)
        {
            return;
        }
        if (ql <= l && r <= qr)
        {
            applyAdd(p, x);
            return;
        }
        push(p);
        int m = (l + r) / 2;
        add(2 * p, l, m, ql, qr, x);
        add(2 * p + 1, m, r, ql, qr, x);
        pull(p);
    }

    void chmin(int p, int l, int r, int ql, int qr, long long x)
    {
        // p、[l,r) 是当前节点；把 [ql,qr) 内大于 x 的值压到 x。
        if (qr <= l || r <= ql || tr[p].mx <= x)
        {
            return;
        }
        if (ql <= l && r <= qr && (!tr[p].hasMx2 || tr[p].mx2 < x))
        {
            applyChmin(p, x);
            return;
        }
        push(p);
        int m = (l + r) / 2;
        chmin(2 * p, l, m, ql, qr, x);
        chmin(2 * p + 1, m, r, ql, qr, x);
        pull(p);
    }

    void chmax(int p, int l, int r, int ql, int qr, long long x)
    {
        // p、[l,r) 是当前节点；把 [ql,qr) 内小于 x 的值抬到 x。
        if (qr <= l || r <= ql || tr[p].mn >= x)
        {
            return;
        }
        if (ql <= l && r <= qr && (!tr[p].hasMn2 || x < tr[p].mn2))
        {
            applyChmax(p, x);
            return;
        }
        push(p);
        int m = (l + r) / 2;
        chmax(2 * p, l, m, ql, qr, x);
        chmax(2 * p + 1, m, r, ql, qr, x);
        pull(p);
    }

    long long sum(int p, int l, int r, int ql, int qr)
    {
        // p、[l,r) 是当前节点；返回 [ql,qr) 的区间和。
        if (qr <= l || r <= ql)
        {
            return 0;
        }
        if (ql <= l && r <= qr)
        {
            return tr[p].sum;
        }
        push(p);
        int m = (l + r) / 2;
        return sum(2 * p, l, m, ql, qr) + sum(2 * p + 1, m, r, ql, qr);
    }

    long long min(int p, int l, int r, int ql, int qr)
    {
        // p、[l,r) 是当前节点；返回 [ql,qr) 的最小值。
        if (qr <= l || r <= ql)
        {
            return numeric_limits<long long>::max();
        }
        if (ql <= l && r <= qr)
        {
            return tr[p].mn;
        }
        push(p);
        int m = (l + r) / 2;
        long long a = min(2 * p, l, m, ql, qr), b = min(2 * p + 1, m, r, ql, qr);
        return a < b ? a : b;
    }

    long long max(int p, int l, int r, int ql, int qr)
    {
        // p、[l,r) 是当前节点；返回 [ql,qr) 的最大值。
        if (qr <= l || r <= ql)
        {
            return numeric_limits<long long>::lowest();
        }
        if (ql <= l && r <= qr)
        {
            return tr[p].mx;
        }
        push(p);
        int m = (l + r) / 2;
        long long a = max(2 * p, l, m, ql, qr), b = max(2 * p + 1, m, r, ql, qr);
        return a < b ? b : a;
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
    SegBeats seg(a);
    while (q--)
    {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 3)
        {
            cout << seg.sum(l, r) << '\n';
            continue;
        }
        long long x;
        cin >> x;
        if (type == 0)
        {
            seg.chmin(l, r, x);
        }
        else if (type == 1)
        {
            seg.chmax(l, r, x);
        }
        else
        {
            seg.add(l, r, x);
        }
    }
    return 0;
}
