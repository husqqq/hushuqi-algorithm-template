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

template <class T> struct SplitSeg
{
    struct Node
    {
        // l、r 是左右儿子编号，sum 是子树内的非负计数和。
        int l = 0, r = 0;
        T sum{};
    };

    // lo、hi 是整棵计数树的半开值域，t 是所有树共享的节点池。
    int lo, hi;
    vector<Node> t{{}};

    SplitSeg(int lo, int hi) : lo(lo), hi(hi)
    {
        // lo、hi 是非空半开值域边界。
        assert(lo < hi); // 调试检查，可删
    }
    int newNode()
    {
        // 分配并返回一个全零节点编号。
        t.push_back({});
        return (int)t.size() - 1;
    }
    int makeRoot() const
    {
        // 返回表示空树的根编号 0。
        return 0;
    }

    void pull(int p)
    {
        // p 是内部节点；由两个儿子的计数和重算当前节点。
        t[p].sum = t[t[p].l].sum + t[t[p].r].sum;
    }

    int add(int p, int l, int r, int x, T v)
    {
        // p、[l,r) 是当前计数树，x 是值域位置，v 是计数增量；返回新根。
        if (!p)
        {
            p = newNode();
        }
        if (l + 1 == r)
        {
            t[p].sum += v;
            assert(t[p].sum >= T{}); // 调试检查，可删
            return t[p].sum == T{} ? 0 : p;
        }
        int m = midpoint(l, r);
        if (x < m)
        {
            t[p].l = add(t[p].l, l, m, x, v);
        }
        else
        {
            t[p].r = add(t[p].r, m, r, x, v);
        }
        pull(p);
        return t[p].sum == T{} ? 0 : p;
    }

    void add(int &p, int x, T v)
    {
        // p 是根，x 是值域位置，v 是计数增量；更新后所有叶子计数必须非负。
        assert(lo <= x && x < hi); // 调试检查，可删
        p = add(p, lo, hi, x, v);
    }

    pair<int, int> split(int p, int l, int r, int x, int y)
    {
        // p、[l,r) 是当前树，[x,y) 是分出值域；返回 {保留根,分出根}。
        if (!p || y <= l || r <= x)
        {
            return {p, 0};
        }
        if (x <= l && r <= y)
        {
            return {0, p};
        }
        int q = newNode(), m = midpoint(l, r);
        auto [al, bl] = split(t[p].l, l, m, x, y);
        auto [ar, br] = split(t[p].r, m, r, x, y);
        t[p].l = al;
        t[p].r = ar;
        t[q].l = bl;
        t[q].r = br;
        pull(p);
        pull(q);
        if (t[p].sum == T{})
        {
            p = 0;
        }
        if (t[q].sum == T{})
        {
            q = 0;
        }
        return {p, q};
    }

    int split(int &p, int l, int r)
    {
        // p 是原根；把值域 [l,r) 分出并返回新根，p 保留其余部分。
        assert(lo <= l && l <= r && r <= hi); // 调试检查，可删
        auto [a, b] = split(p, lo, hi, l, r);
        p = a;
        return b;
    }

    int merge(int p, int q, int l, int r)
    {
        // p、q 是当前值域 [l,r) 上的两棵树根；破坏性合并并返回根。
        if (!p || !q)
        {
            return p | q;
        }
        if (l + 1 == r)
        {
            t[p].sum += t[q].sum;
            return p;
        }
        int m = midpoint(l, r);
        t[p].l = merge(t[p].l, t[q].l, l, m);
        t[p].r = merge(t[p].r, t[q].r, m, r);
        pull(p);
        return p;
    }

    int merge(int p, int q)
    {
        // p、q 是两棵所有权独立的计数树根；破坏性合并并返回根。
        return merge(p, q, lo, hi);
    }

    T sum(int p, int l, int r, int x, int y) const
    {
        // p、[l,r) 是当前树；返回值域 [x,y) 的计数和。
        if (!p || y <= l || r <= x)
        {
            return T{};
        }
        if (x <= l && r <= y)
        {
            return t[p].sum;
        }
        int m = midpoint(l, r);
        return sum(t[p].l, l, m, x, y) + sum(t[p].r, m, r, x, y);
    }

    T rangeSum(int p, int l, int r) const
    {
        // p 是根；返回值域 [l,r) 内的计数和。
        assert(lo <= l && l <= r && r <= hi); // 调试检查，可删
        return sum(p, lo, hi, l, r);
    }

    int kth(int p, T k) const
    {
        // p 是根，k 是从 1 开始的排名；返回第 k 小值，越界返回 -1。
        if (!p || k <= T{} || t[p].sum < k)
        {
            return -1;
        }
        int l = lo, r = hi;
        while (l + 1 < r)
        {
            int m = midpoint(l, r), q = t[p].l;
            if (t[q].sum >= k)
            {
                p = q;
                r = m;
            }
            else
            {
                k -= t[q].sum;
                p = t[p].r;
                l = m;
            }
        }
        return l;
    }
};
