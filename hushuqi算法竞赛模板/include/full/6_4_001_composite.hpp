#pragma once

#include "../base.hpp"

template <long long P> class DynPathComp
{
public:
    struct Func
    {
        long long a = 1, b = 0; // 表示一次函数 a*x+b（模 P）。
    };

private:
    struct Node
    {
        int p = 0, ch[2]{};
        bool rev = false;
        Func val, fw, bw; // val 是本点函数；fw、bw 是辅助树中序正向、反向复合。
    };
    vector<Node> t;

    static Func compose(Func first, Func second)
    {
        // first、second 按调用先后给出；返回 second(first(x))。
        return {second.a * first.a % P, (second.a * first.b + second.b) % P};
    }
    bool root(int x) const
    {
        // x 是内部下标；返回 x 是否为当前辅助树根。
        int p = t[x].p;
        return !p || (t[p].ch[0] != x && t[p].ch[1] != x);
    }
    void pull(int x)
    {
        // x 是内部下标；由两个儿子和本点重算复合信息。
        int l = t[x].ch[0], r = t[x].ch[1];
        t[x].fw = compose(compose(t[l].fw, t[x].val), t[r].fw);
        t[x].bw = compose(compose(t[r].bw, t[x].val), t[l].bw);
    }
    void reverseNode(int x)
    {
        // x 是内部下标；翻转其辅助树路径方向。
        if (!x) return;
        swap(t[x].ch[0], t[x].ch[1]);
        swap(t[x].fw, t[x].bw);
        t[x].rev = !t[x].rev;
    }
    void push(int x)
    {
        // x 是内部下标；把路径翻转标记下传。
        if (!t[x].rev) return;
        reverseNode(t[x].ch[0]);
        reverseNode(t[x].ch[1]);
        t[x].rev = false;
    }
    void rotate(int x)
    {
        // x 是内部下标；把 x 向辅助树根方向旋转一层。
        int y = t[x].p, z = t[y].p, d = t[y].ch[1] == x, b = t[x].ch[d ^ 1];
        if (!root(y)) t[z].ch[t[z].ch[1] == y] = x;
        t[x].p = z;
        t[x].ch[d ^ 1] = y;
        t[y].p = x;
        t[y].ch[d] = b;
        if (b) t[b].p = y;
        pull(y);
        pull(x);
    }
    void splay(int x)
    {
        // x 是内部下标；把 x 旋到辅助树根。
        vector<int> st{x};
        for (int y = x; !root(y); y = t[y].p) st.push_back(t[y].p);
        while (!st.empty()) push(st.back()), st.pop_back();
        while (!root(x))
        {
            int y = t[x].p, z = t[y].p;
            if (!root(y)) rotate((t[y].ch[1] == x) == (t[z].ch[1] == y) ? y : x);
            rotate(x);
        }
    }
    void access(int x)
    {
        // x 是内部下标；暴露当前根到 x 的首选路径。
        for (int y = 0, z = x; z; z = t[y = z].p)
        {
            splay(z);
            t[z].ch[1] = y;
            pull(z);
        }
        splay(x);
    }
    void makeRoot(int x)
    {
        // x 是内部下标；把 repr tree 的根换到 x。
        access(x);
        reverseNode(x);
    }
    int findRoot(int x)
    {
        // x 是内部下标；返回所在 repr tree 的根。
        access(x);
        push(x);
        while (t[x].ch[0]) x = t[x].ch[0], push(x);
        splay(x);
        return x;
    }

public:
    explicit DynPathComp(const vector<Func> &f = {}) : t(f.size() + 1)
    {
        // f[u] 是点 u 的初始函数。
        t[0].val = t[0].fw = t[0].bw = {};
        for (int u = 0; u < (int)f.size(); u++) t[u + 1].val = t[u + 1].fw = t[u + 1].bw = f[u];
    }
    bool link(int u, int v)
    {
        // u、v 是不同树中的点；连边并返回是否成功。
        u++, v++;
        makeRoot(u);
        if (findRoot(v) == u) return false;
        t[u].p = v;
        return true;
    }
    bool cut(int u, int v)
    {
        // u、v 是点编号；删除直接边并返回是否成功。
        u++, v++;
        makeRoot(u);
        access(v);
        if (t[v].ch[0] != u || t[u].ch[1]) return false;
        t[v].ch[0] = t[u].p = 0;
        pull(v);
        return true;
    }
    void set(int u, Func f)
    {
        // u 是点编号，f 是新函数；替换点函数。
        u++;
        access(u);
        t[u].val = f;
        pull(u);
    }
    Func path(int u, int v)
    {
        // u、v 是连通点；返回从 u 到 v 依次调用点函数的复合。
        u++, v++;
        makeRoot(u);
        access(v);
        return t[v].fw;
    }
    long long apply(int u, int v, long long x)
    {
        // u、v 是连通点，x 是初值；返回动态路径复合在 x 处的值。
        Func f = path(u, v);
        return (f.a * x + f.b) % P;
    }
};
