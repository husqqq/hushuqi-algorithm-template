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

struct PerSeg
{
    struct Node
    {
        // l、r 是左右儿子编号，sum、weight 是当前值域的计数和与附加权值和。
        int l = 0, r = 0;
        long long sum = 0;
        long long weight = 0;
    };

    // n 是离散值域大小，t 是所有版本共享的节点池。
    int n;
    vector<Node> t{{}};

    PerSeg(int n) : n(n)
    {
        // n 是非空离散值域大小；0 号根表示空版本。
        assert(n > 0); // 调试检查，可删
    }

    int add(int root, int x, long long v)
    {
        // root 是旧版本根，x 是值域下标，v 是计数增量；返回新版本根。
        return add(root, x, v, 0);
    }

    int add(int root, int x, long long dc, long long dw)
    {
        // root 是旧版本根，x 是值域下标；dc、dw 是计数与附加权值增量，返回新版本根。
        assert(0 <= x && x < n); // 调试检查，可删
        return add(root, 0, n, x, dc, dw);
    }

    long long query(int root, int l, int r) const
    {
        // root 是版本根；返回值域 [l,r) 的计数和。
        return queryInfo(root, l, r)[0];
    }

    array<long long, 2> queryInfo(int root, int l, int r) const
    {
        // root 是版本根，l、r 是值域半开区间；返回 {计数和,附加权值和}。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return queryInfo(root, 0, n, l, r);
    }

    // 返回右版本减左版本后的第 k 小下标，k 从 0 开始；sum 必须表示非负计数。
    int kth(int rootL, int rootR, long long k) const
    {
        // rootL、rootR 是两个前缀版本根，k 是 0 基排名；返回版本差中的第 k 小值域下标。
        assert(0 <= k && k < t[rootR].sum - t[rootL].sum); // 调试检查，可删
        int l = 0, r = n;
        while (r - l > 1)
        {
            int m = (l + r) / 2;
            long long z = t[t[rootR].l].sum - t[t[rootL].l].sum;
            if (k < z)
            {
                rootL = t[rootL].l;
                rootR = t[rootR].l;
                r = m;
            }
            else
            {
                k -= z;
                rootL = t[rootL].r;
                rootR = t[rootR].r;
                l = m;
            }
        }
        return l;
    }

  private:
    int clone(int p)
    {
        // p 是旧节点编号；复制它并返回新节点编号。
        t.push_back(t[p]);
        return (int)t.size() - 1;
    }

    int add(int p, int l, int r, int x, long long dc, long long dw)
    {
        // p 是旧节点，[l,r) 是其值域，x 是更新位置；两个增量分别修改计数与附加权值，返回新节点。
        int q = clone(p);
        t[q].sum += dc;
        t[q].weight += dw;
        if (r - l == 1)
        {
            return q;
        }
        int m = (l + r) / 2;
        if (x < m)
        {
            t[q].l = add(t[q].l, l, m, x, dc, dw);
        }
        else
        {
            t[q].r = add(t[q].r, m, r, x, dc, dw);
        }
        return q;
    }

    array<long long, 2> queryInfo(int p, int l, int r, int ql, int qr) const
    {
        // p 是当前节点，[l,r) 是其值域；返回 [ql,qr) 的计数和与附加权值和。
        if (!p || qr <= l || r <= ql)
        {
            return {0, 0};
        }
        if (ql <= l && r <= qr)
        {
            return {t[p].sum, t[p].weight};
        }
        int m = (l + r) / 2;
        auto a = queryInfo(t[p].l, l, m, ql, qr);
        auto b = queryInfo(t[p].r, m, r, ql, qr);
        return {a[0] + b[0], a[1] + b[1]};
    }
};

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q; vector<int> a(n), value;
    for (auto &x : a) cin >> x;
    value = a; sort(value.begin(), value.end());
    value.erase(unique(value.begin(), value.end()), value.end());
    PerSeg st(value.size()); vector<int> root(n + 1);
    for (int i = 0; i < n; ++i)
        root[i + 1] = st.add(root[i], lower_bound(value.begin(), value.end(), a[i]) - value.begin(), 1);
    while (q--)
    {
        int l, r, k; cin >> l >> r >> k;
        cout << value[st.kth(root[l], root[r], k)] << '\n';
    }
}
