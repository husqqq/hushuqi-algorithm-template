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

template <class T> struct Treap
{
    struct Node
    {
        T x{};
        // pri 是随机优先级；l、r 是儿子；cnt 是键重复数；sz 是含重复的子树大小。
        unsigned pri = 0;
        int l = 0, r = 0, cnt = 0, sz = 0;
    };

    vector<Node> t{{}};
    // root 是树根，seed 是优先级生成器状态。
    int root = 0;
    unsigned seed = 712367821;

    unsigned rnd()
    {
        // 推进内部伪随机状态并返回新值。
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        return seed;
    }

    int size(int u) const
    {
        // u 是节点编号；返回含重复键的子树大小，空节点返回 0。
        return u ? t[u].sz : 0;
    }

    void pull(int u)
    {
        // u 是非空节点；由两个儿子和本键重数重算子树大小。
        t[u].sz = size(t[u].l) + size(t[u].r) + t[u].cnt;
    }

    int rot(int u, int d)
    {
        // u 是子树根；d=1 右旋、d=0 左旋，返回旋转后的根。
        int v = d ? t[u].l : t[u].r;
        if (d)
        {
            t[u].l = t[v].r;
            t[v].r = u;
        }
        else
        {
            t[u].r = t[v].l;
            t[v].l = u;
        }
        pull(u);
        pull(v);
        return v;
    }

    int add(int u, const T &x)
    {
        // u 是子树根，x 是待插入键；返回插入后的根。
        if (!u)
        {
            u = t.size();
            t.push_back({x, rnd(), 0, 0, 1, 1});
            return u;
        }
        if (t[u].x == x)
        {
            t[u].cnt++;
        }
        else if (x < t[u].x)
        {
            t[u].l = add(t[u].l, x);
            if (t[t[u].l].pri < t[u].pri)
            {
                u = rot(u, 1);
            }
        }
        else
        {
            t[u].r = add(t[u].r, x);
            if (t[t[u].r].pri < t[u].pri)
            {
                u = rot(u, 0);
            }
        }
        pull(u);
        return u;
    }

    void add(const T &x)
    {
        // x 是待插入键；插入一份并更新树根。
        root = add(root, x);
    }

    pair<int, bool> erase(int u, const T &x)
    {
        // u 是子树根，x 是待删键；返回删除后的根与删除前是否存在。
        if (!u)
        {
            return {0, false};
        }
        bool ok;
        if (x < t[u].x)
        {
            auto [v, z] = erase(t[u].l, x);
            t[u].l = v;
            ok = z;
        }
        else if (t[u].x < x)
        {
            auto [v, z] = erase(t[u].r, x);
            t[u].r = v;
            ok = z;
        }
        else
        {
            ok = true;
            if (t[u].cnt > 1)
            {
                t[u].cnt--;
            }
            else if (!t[u].l || !t[u].r)
            {
                u = t[u].l | t[u].r;
            }
            else
            {
                int d = t[t[u].l].pri < t[t[u].r].pri;
                u = rot(u, d);
                if (d)
                {
                    auto [v, z] = erase(t[u].r, x);
                    t[u].r = v;
                }
                else
                {
                    auto [v, z] = erase(t[u].l, x);
                    t[u].l = v;
                }
            }
        }
        if (u)
        {
            pull(u);
        }
        return {u, ok};
    }

    bool erase(const T &x)
    {
        // x 是待删键；删除一份并返回删除前是否存在。
        auto [u, ok] = erase(root, x);
        root = u;
        return ok;
    }

    int rank(const T &x) const
    {
        // x 是查询键；返回严格小于 x 的元素数。
        int u = root, ans = 0;
        while (u)
        {
            if (x <= t[u].x)
            {
                u = t[u].l;
            }
            else
            {
                ans += size(t[u].l) + t[u].cnt;
                u = t[u].r;
            }
        }
        return ans;
    }

    T kth(int k) const
    {
        // k 是 0 基排名；返回第 k 小键。
        assert(0 <= k && k < size(root)); // 调试检查，可删
        int u = root;
        while (true)
        {
            int z = size(t[u].l);
            if (k < z)
            {
                u = t[u].l;
            }
            else if (k < z + t[u].cnt)
            {
                return t[u].x;
            }
            else
            {
                k -= z + t[u].cnt;
                u = t[u].r;
            }
        }
    }

    optional<T> prev(const T &x) const
    {
        // x 是查询键；返回严格小于 x 的最大键，不存在时返回空。
        int k = rank(x);
        return k ? optional<T>(kth(k - 1)) : nullopt;
    }

    optional<T> next(const T &x) const
    {
        // x 是查询键；返回严格大于 x 的最小键，不存在时返回空。
        int u = root, k = 0;
        while (u)
        {
            if (x < t[u].x)
            {
                u = t[u].l;
            }
            else
            {
                k += size(t[u].l) + t[u].cnt;
                u = t[u].r;
            }
        }
        return k < size(root) ? optional<T>(kth(k)) : nullopt;
    }
};
