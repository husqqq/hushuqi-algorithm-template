#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <class T> struct PerTreap
{
    struct N
    {
        // l、r 是左右儿子编号，sz 是子树大小。
        int l = 0, r = 0, sz = 0;
        // p 是随机优先级；v 是本点值，s 是子树和。
        unsigned p = 0;
        T v{}, s{};
        // rev 表示当前子树的中序顺序需要翻转。
        bool rev = false;
    };

    // t 是持久化节点池，seed 是优先级生成器状态。
    vector<N> t{N{}};
    unsigned seed = 712367821;

    unsigned rnd()
    {
        // 推进内部伪随机状态并返回新值。
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        return seed;
    }

    int size(int x) const
    {
        // x 是节点编号；返回其子树大小，空节点返回 0。
        return t[x].sz;
    }
    T sum(int x) const
    {
        // x 是节点编号；返回其子树和，空节点返回零。
        return t[x].s;
    }

    int node(T v)
    {
        // v 是新元素的值；新建单点并返回节点编号。
        t.push_back({0, 0, 1, rnd(), v, v, false});
        return t.size() - 1;
    }
    int copy(int x)
    {
        // x 是旧节点编号；复制非空节点并返回副本编号，空节点仍返回 0。
        if (x)
        {
            t.push_back(t[x]);
        }
        return x ? (int)t.size() - 1 : 0;
    }
    void pull(int x)
    {
        // x 是非空节点编号；由两个儿子重算大小与和。
        t[x].sz = size(t[x].l) + 1 + size(t[x].r);
        t[x].s = sum(t[x].l) + t[x].v + sum(t[x].r);
    }
    int flip(int x)
    {
        // x 是旧子树根；复制根、交换儿子并翻转标记，返回新根。
        x = copy(x);
        if (x)
        {
            swap(t[x].l, t[x].r);
            t[x].rev ^= 1;
        }
        return x;
    }
    void push(int x)
    {
        // x 是已复制的非空节点；持久化下传翻转标记。
        if (!t[x].rev)
        {
            return;
        }
        t[x].l = flip(t[x].l);
        t[x].r = flip(t[x].r);
        t[x].rev = false;
    }

    pair<int, int> split(int x, int k)
    {
        // x 是旧版本根，k 是左段长度；返回前 k 项与其余项的新版本根。
        if (!x)
        {
            return {0, 0};
        }
        x = copy(x);
        push(x);
        if (size(t[x].l) >= k)
        {
            auto [a, b] = split(t[x].l, k);
            t[x].l = b;
            pull(x);
            return {a, x};
        }
        auto [a, b] = split(t[x].r, k - size(t[x].l) - 1);
        t[x].r = a;
        pull(x);
        return {x, b};
    }

    int merge(int x, int y)
    {
        // x、y 是相邻序列的根；返回保持中序顺序的合并版本根。
        if (!x || !y)
        {
            return x | y;
        }
        if (t[x].p < t[y].p)
        {
            x = copy(x);
            push(x);
            t[x].r = merge(t[x].r, y);
            pull(x);
            return x;
        }
        y = copy(y);
        push(y);
        t[y].l = merge(x, t[y].l);
        pull(y);
        return y;
    }

    int insert(int rt, int k, T v)
    {
        // rt 是旧版本根，k 是插入位置，v 是新值；返回插入后的版本根。
        assert(0 <= k && k <= size(rt)); // 调试检查，可删
        auto [a, b] = split(rt, k);
        return merge(merge(a, node(v)), b);
    }
    int erase(int rt, int k)
    {
        // rt 是旧版本根，k 是删除位置；返回删除后的版本根。
        assert(0 <= k && k < size(rt)); // 调试检查，可删
        auto [a, b] = split(rt, k);
        auto [c, d] = split(b, 1);
        return merge(a, d);
    }
    int reverse(int rt, int l, int r)
    {
        // rt 是旧版本根；翻转半开区间 [l,r) 并返回新版本根。
        assert(0 <= l && l <= r && r <= size(rt)); // 调试检查，可删
        auto [a, b] = split(rt, l);
        auto [c, d] = split(b, r - l);
        return merge(merge(a, flip(c)), d);
    }
    T query(int x, int l, int r, bool f) const
    {
        // x 是当前子树根，[l,r) 是相对子树的询问，f 是祖先累计的翻转状态。
        if (!x || l == r)
        {
            return T{};
        }
        if (l == 0 && r == size(x))
        {
            return sum(x);
        }
        int a = f ? t[x].r : t[x].l;
        int b = f ? t[x].l : t[x].r;
        bool g = f ^ t[x].rev;
        int z = size(a);
        T ans{};
        if (l < z)
        {
            ans += query(a, l, min(r, z), g);
        }
        if (l <= z && z < r)
        {
            ans += t[x].v;
        }
        if (z + 1 < r)
        {
            ans += query(b, max<int>(0, l - z - 1), r - z - 1, g);
        }
        return ans;
    }

    T query(int rt, int l, int r) const
    {
        // rt 是版本根；只读返回半开区间 [l,r) 的和，不新建节点。
        assert(0 <= l && l <= r && r <= size(rt)); // 调试检查，可删
        return query(rt, l, r, false);
    }

    vector<T> values(int rt) const
    {
        // rt 是版本根；返回该版本的序列值，顺序与隐式下标一致。
        vector<T> a;
        auto dfs = [&](auto &&self, int x, bool f) -> void
        {
            if (!x)
            {
                return;
            }
            int l = f ? t[x].r : t[x].l;
            int r = f ? t[x].l : t[x].r;
            bool g = f ^ t[x].rev;
            self(self, l, g);
            a.push_back(t[x].v);
            self(self, r, g);
        };
        dfs(dfs, rt, false);
        return a;
    }
};
