#pragma once

// TreePathSum 的完整实现与动态树公共模块共用，避免重复维护。
#include "6_4_001.hpp"

template <long long P> class TreePathComp
{
public:
    struct Func
    {
        long long a = 1, b = 0; // 表示一次函数 a*x+b（模 P）。
    };

private:
    vector<int> fa, dep, siz, son, top, in;
    int n = 1;
    vector<Func> fw, bw; // fw、bw 分别保存区间正序与逆序复合。

    static Func compose(Func first, Func second)
    {
        // first、second 按调用先后给出；返回 second(first(x))。
        return {second.a * first.a % P, (second.a * first.b + second.b) % P};
    }

    pair<Func, Func> range(int l, int r) const
    {
        // l、r 是 HLD 序半开区间；返回正序和逆序复合。
        Func lf, rf, lb, rb;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1)
            {
                lf = compose(lf, fw[l]);
                lb = compose(bw[l], lb);
                l++;
            }
            if (r & 1)
            {
                --r;
                rf = compose(fw[r], rf);
                rb = compose(rb, bw[r]);
            }
        }
        return {compose(lf, rf), compose(rb, lb)};
    }

public:
    TreePathComp(const vector<vector<int>> &g, const vector<Func> &f, int root = 0)
        : fa(g.size(), -1), dep(g.size()), siz(g.size(), 1), son(g.size(), -1),
          top(g.size()), in(g.size())
    {
        // g 是非空树，f[x] 是点 x 的一次函数，root 是 HLD 根。
        assert(g.size() == f.size());
        vector<int> order{root};
        for (int i = 0; i < (int)order.size(); i++)
        {
            int u = order[i];
            for (int v : g[u]) if (v != fa[u]) fa[v] = u, dep[v] = dep[u] + 1, order.push_back(v);
        }
        for (int i = (int)order.size() - 1; i > 0; i--)
        {
            int u = order[i], p = fa[u];
            siz[p] += siz[u];
            if (son[p] < 0 || siz[u] > siz[son[p]]) son[p] = u;
        }
        int timer = 0;
        for (int u : order)
        {
            if (u != root && son[fa[u]] == u) continue;
            for (int v = u; v >= 0; v = son[v]) top[v] = u, in[v] = timer++;
        }
        while (n < (int)g.size()) n <<= 1;
        fw.assign(2 * n, {});
        bw.assign(2 * n, {});
        for (int u = 0; u < (int)g.size(); u++) fw[n + in[u]] = bw[n + in[u]] = f[u];
        for (int i = n - 1; i; i--)
        {
            fw[i] = compose(fw[2 * i], fw[2 * i + 1]);
            bw[i] = compose(bw[2 * i + 1], bw[2 * i]);
        }
    }

    void set(int u, Func f)
    {
        // u 是点编号，f 是新函数；替换点函数，无返回值。
        int p = n + in[u];
        fw[p] = bw[p] = f;
        while (p >>= 1)
        {
            fw[p] = compose(fw[2 * p], fw[2 * p + 1]);
            bw[p] = compose(bw[2 * p + 1], bw[2 * p]);
        }
    }

    Func path(int u, int v) const
    {
        // u、v 是路径端点；返回从 u 到 v 依次调用点函数的复合函数。
        Func left, right;
        while (top[u] != top[v])
        {
            if (dep[top[u]] >= dep[top[v]])
            {
                auto [forward, backward] = range(in[top[u]], in[u] + 1);
                left = compose(left, backward);
                u = fa[top[u]];
            }
            else
            {
                auto [forward, backward] = range(in[top[v]], in[v] + 1);
                right = compose(forward, right);
                v = fa[top[v]];
            }
        }
        if (dep[u] >= dep[v])
        {
            auto [forward, backward] = range(in[v], in[u] + 1);
            left = compose(left, backward);
        }
        else
        {
            auto [forward, backward] = range(in[u], in[v] + 1);
            right = compose(forward, right);
        }
        return compose(left, right);
    }

    long long apply(int u, int v, long long x) const
    {
        // u、v 是路径端点，x 是初值；返回路径复合在 x 处的值。
        Func f = path(u, v);
        return (f.a * x + f.b) % P;
    }
};
