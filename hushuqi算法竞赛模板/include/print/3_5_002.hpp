#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T> struct FHQ
{
    struct Node
    {
        // x 保存键值；l、r 保存左右儿子；sz 保存子树大小；pri 保存随机优先级。
        T x{};
        int l = 0, r = 0, sz = 1;
        unsigned pri = 0;
    };

    // tr 保存节点池；root 保存根；seed 保存随机数状态。
    vector<Node> tr{{}};
    int root = 0;
    unsigned seed = 712367821;

    unsigned rnd()
    {
        // 推进内部伪随机状态并返回新的优先级。
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        return seed;
    }

    int size(int p) const
    {
        // p 是节点编号；返回其子树大小，空节点返回 0。
        return p ? tr[p].sz : 0;
    }

    void pull(int p)
    {
        // p 是非空节点；根据两个儿子重算子树大小。
        tr[p].sz = size(tr[p].l) + size(tr[p].r) + 1;
    }

    pair<int, int> split(int p, const T &x)
    {
        // p 是树根，x 是分界键；返回键值小于 x 与不小于 x 的两棵树根。
        if (!p)
        {
            return {0, 0};
        }
        if (tr[p].x < x)
        {
            auto [a, b] = split(tr[p].r, x);
            tr[p].r = a;
            pull(p);
            return {p, b};
        }
        auto [a, b] = split(tr[p].l, x);
        tr[p].l = b;
        pull(p);
        return {a, p};
    }

    pair<int, int> splitLE(int p, const T &x)
    {
        // p 是树根，x 是分界键；返回键值不大于 x 与大于 x 的两棵树根。
        if (!p)
        {
            return {0, 0};
        }
        if (!(x < tr[p].x))
        {
            auto [a, b] = splitLE(tr[p].r, x);
            tr[p].r = a;
            pull(p);
            return {p, b};
        }
        auto [a, b] = splitLE(tr[p].l, x);
        tr[p].l = b;
        pull(p);
        return {a, p};
    }

    int merge(int p, int q)
    {
        // p、q 是两棵树根，且 p 中所有键不大于 q 中所有键；返回合并后的根。
        if (!p || !q)
        {
            return p | q;
        }
        if (tr[p].pri < tr[q].pri)
        {
            tr[p].r = merge(tr[p].r, q);
            pull(p);
            return p;
        }
        tr[q].l = merge(p, tr[q].l);
        pull(q);
        return q;
    }

    void insert(const T &x)
    {
        // x 是待插入键；允许重复值。
        auto [p, q] = split(root, x);
        int u = tr.size();
        tr.push_back({x, 0, 0, 1, rnd()});
        root = merge(merge(p, u), q);
    }

    bool erase(const T &x)
    {
        // x 是待删除键；只删除一个副本，成功返回 true。
        auto [p, q] = split(root, x);
        auto [u, v] = splitLE(q, x);
        bool ok = u;
        if (u)
        {
            u = merge(tr[u].l, tr[u].r);
        }
        root = merge(merge(p, u), v);
        return ok;
    }

    int rank(const T &x)
    {
        // x 是查询键；返回严格小于 x 的元素个数。
        auto [p, q] = split(root, x);
        int ans = size(p);
        root = merge(p, q);
        return ans;
    }

    T kth(int k) const
    {
        // k 是 0 基排名；返回第 k 小键。
        assert(0 <= k && k < size(root));
        int p = root;
        while (p)
        {
            int z = size(tr[p].l);
            if (k < z)
            {
                p = tr[p].l;
            }
            else if (k == z)
            {
                return tr[p].x;
            }
            else
            {
                k -= z + 1;
                p = tr[p].r;
            }
        }
        return T{};
    }
};
