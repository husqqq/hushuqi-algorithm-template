#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct EulerTour
{
    // 每棵树存成一条欧拉序，序列用带父指针的隐式 treap 维护；节点 0 为空，1..n 是点节点，
    // 之后每条树边对应两个边节点 (u->v 与 v->u)。点节点带权，边节点权恒为 0。
    // l、r、p 是左右儿子与父亲；sz 是子树节点数，cnt 是子树内点节点数。
    vector<int> l, r, p, sz, cnt;
    vector<char> own;                 // own[x] 表示 x 是点节点。
    vector<unsigned long long> pri;   // pri 是 treap 随机优先级。
    vector<__int128> val, sum, lz;    // val 是点权，sum 是子树权和，lz 是整棵子树待加的值。
    map<pair<int, int>, pair<int, int>> edge; // edge[{u,v}] (u<v) 是该边的两个边节点。
    mt19937_64 rng{712367};

    EulerTour(int n)
    {
        // n 是点数，点编号为 [0,n)；初始为 n 个孤立点，点权为 0。
        make(false);
        sz[0] = 0; // 节点 0 是空节点，大小为 0。
        for (int i = 0; i < n; i++)
        {
            make(true);
        }
    }

    int make(bool v)
    {
        // 新建一个节点，v 表示是否为点节点；返回节点编号。
        for (auto *a : {&l, &r, &p})
        {
            a->push_back(0);
        }
        sz.push_back(1);
        cnt.push_back(v);
        own.push_back(v);
        pri.push_back(rng());
        val.push_back(0);
        sum.push_back(0);
        lz.push_back(0);
        return l.size() - 1;
    }

    void apply(int x, __int128 d)
    {
        // 给 x 的子树内所有点节点加 d。
        if (x)
        {
            val[x] += own[x] * d;
            sum[x] += d * cnt[x];
            lz[x] += d;
        }
    }

    void push(int x)
    {
        // 把 x 的懒标记下传给儿子。
        if (lz[x])
        {
            apply(l[x], lz[x]);
            apply(r[x], lz[x]);
            lz[x] = 0;
        }
    }

    void pull(int x)
    {
        // 由儿子重算 x 的子树信息，并修正儿子的父指针。
        sz[x] = 1 + sz[l[x]] + sz[r[x]];
        cnt[x] = own[x] + cnt[l[x]] + cnt[r[x]];
        sum[x] = val[x] + sum[l[x]] + sum[r[x]];
        p[l[x]] = p[r[x]] = x;
        p[0] = 0;
    }

    int merge(int a, int b)
    {
        // 拼接序列 a、b，返回新根（其父指针为 0）。
        if (!a || !b)
        {
            p[a | b] = 0;
            return a | b;
        }
        int x;
        if (pri[a] > pri[b])
        {
            push(a);
            r[a] = merge(r[a], b);
            x = a;
        }
        else
        {
            push(b);
            l[b] = merge(a, l[b]);
            x = b;
        }
        pull(x);
        p[x] = 0;
        return x;
    }

    pair<int, int> split(int x, int k)
    {
        // 把序列 x 拆成前 k 个节点和其余部分，两个根的父指针都为 0。
        if (!x)
        {
            return {0, 0};
        }
        push(x);
        pair<int, int> res;
        if (sz[l[x]] >= k)
        {
            auto [a, b] = split(l[x], k);
            l[x] = b;
            res = {a, x};
        }
        else
        {
            auto [a, b] = split(r[x], k - sz[l[x]] - 1);
            r[x] = a;
            res = {x, b};
        }
        pull(x);
        p[res.first] = p[res.second] = 0;
        return res;
    }

    int root(int x) const
    {
        // 返回 x 所在序列的根。
        while (p[x])
        {
            x = p[x];
        }
        return x;
    }

    int index(int x) const
    {
        // 返回 x 在所在序列中的下标（从 0 开始）。
        int i = sz[l[x]];
        for (; p[x]; x = p[x])
        {
            if (x == r[p[x]])
            {
                i += sz[l[p[x]]] + 1;
            }
        }
        return i;
    }

    int reroot(int u)
    {
        // 把点 u 所在树的欧拉序旋转为以 u 开头，返回序列根。
        int x = u + 1;
        auto [a, b] = split(root(x), index(x));
        return merge(b, a);
    }

    bool connected(int u, int v) const
    {
        // 返回 u、v 是否在同一棵树中。
        return root(u + 1) == root(v + 1);
    }

    bool link(int u, int v)
    {
        // 连边 u-v；已连通时不操作并返回 false。
        if (connected(u, v))
        {
            return false;
        }
        int a = reroot(u), b = reroot(v), uv = make(false), vu = make(false);
        edge[minmax(u, v)] = {uv, vu};
        merge(merge(merge(a, uv), b), vu);
        return true;
    }

    bool cut(int u, int v)
    {
        // 删除树边 u-v；边不存在时返回 false。
        auto it = edge.find(minmax(u, v));
        if (it == edge.end())
        {
            return false;
        }
        auto [x, y] = it->second;
        int i = index(x), j = index(y);
        if (i > j)
        {
            swap(i, j);
        }
        // 序列形如 A [e1] B [e2] C：B 是被切下的子树，A+C 是剩下的树。
        auto [a, rest] = split(root(x), i);
        auto [e1, tail] = split(rest, 1);
        auto [b, last] = split(tail, j - i - 1);
        auto [e2, c] = split(last, 1);
        merge(a, c);
        edge.erase(it);
        return true;
    }

    __int128 compSum(int u) const
    {
        // 返回 u 所在树的点权和。
        return sum[root(u + 1)];
    }

    int compSize(int u) const
    {
        // 返回 u 所在树的点数。
        return cnt[root(u + 1)];
    }

    void compAdd(int u, int d)
    {
        // 给 u 所在树的所有点加 d。
        apply(root(u + 1), d);
    }

    void add(int u, int d)
    {
        // 给点 u 加 d：先自顶向下下传标记，再自底向上重算。
        int x = u + 1;
        vector<int> path;
        for (int y = x; y; y = p[y])
        {
            path.push_back(y);
        }
        for (int i = path.size() - 1; i >= 0; i--)
        {
            push(path[i]);
        }
        val[x] += d;
        for (int y : path)
        {
            pull(y);
        }
    }
};
