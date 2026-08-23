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

struct PerDSU
{
    struct N
    {
        // l、r 是持久化数组的左右儿子；value 为叶子覆盖值，负数编码根的集合大小，非负数是父亲。
        int l = 0, r = 0, value = -1;
    };

    // n 是元素个数；t[0] 是全未写的初始版本，隐含每个元素自成集合。
    int n;
    vector<N> t{{}};

    PerDSU(int n) : n(n)
    {
        // n 是元素个数；允许 n=0，此时 build 返回空根 0。
        assert(n >= 0); // 调试检查，可删
    }

    int build(int l, int r) const
    {
        // [l,r) 是初始数组范围；初始值全是隐含单点集合，返回空根 0。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return 0;
    }

    int build() const
    {
        // 返回 n 个单点集合组成的初始版本根；空结构也返回 0。
        return 0;
    }

    int value(int x, int l, int r, int p) const
    {
        // x 是版本树节点，[l,r) 是其区间，p 是元素下标；返回覆盖值，未写位置返回 -1。
        if (!x)
        {
            return -1;
        }
        if (r - l == 1)
        {
            return t[x].value;
        }
        int m = (l + r) / 2;
        return p < m ? value(t[x].l, l, m, p) : value(t[x].r, m, r, p);
    }

    pair<int, int> get(int rt, int p) const
    {
        // rt 是版本根，p 是元素下标；返回 {父亲,根大小}，非根的第二项不使用。
        assert(0 <= p && p < n); // 调试检查，可删
        int v = value(rt, 0, n, p);
        return v < 0 ? pair<int, int>{p, -v} : pair<int, int>{v, 0};
    }

    int set(int x, int l, int r, int p, int v)
    {
        // x 是旧版本节点，[l,r) 是其区间，p 是元素下标；写入覆盖值 v 并返回新根。
        assert(t.size() < numeric_limits<int>::max()); // 调试检查，可删
        int y = (int)t.size();
        t.push_back(t[x]);
        if (r - l == 1)
        {
            t[y].value = v;
            return y;
        }
        int m = (l + r) / 2;
        if (p < m)
        {
            t[y].l = set(t[y].l, l, m, p, v);
        }
        else
        {
            t[y].r = set(t[y].r, m, r, p, v);
        }
        return y;
    }

    int find(int rt, int x) const
    {
        // rt 是版本根，x 是元素编号；沿父链返回根，不做路径压缩。
        while (true)
        {
            int p = get(rt, x).first;
            if (p == x)
            {
                return x;
            }
            x = p;
        }
    }

    int merge(int rt, int x, int y)
    {
        // rt 是旧版本根，x、y 是元素编号；返回按大小合并后的新版本根。
        x = find(rt, x);
        y = find(rt, y);
        if (x == y)
        {
            return rt;
        }
        int sx = get(rt, x).second;
        int sy = get(rt, y).second;
        if (sx < sy)
        {
            swap(x, y);
            swap(sx, sy);
        }
        rt = set(rt, 0, n, y, x);
        return set(rt, 0, n, x, -(sx + sy));
    }

    bool same(int rt, int x, int y) const
    {
        // rt 是版本根，x、y 是元素编号；返回二者是否同属一个集合。
        return find(rt, x) == find(rt, y);
    }

    int size(int rt, int x) const
    {
        // rt 是版本根，x 是元素编号；返回其集合大小。
        return get(rt, find(rt, x)).second;
    }
};
