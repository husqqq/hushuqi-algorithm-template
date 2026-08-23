#pragma once

#include "5_5_001.hpp"

struct MstTop2
{
    // a[0..1] 是一段路径上最大、严格次大的边权，不存在时为空。
    array<optional<int>, 2> a;
};

MstTop2 mstMergeTop(const MstTop2 &a, const MstTop2 &b)
{
    // a、b 是两段路径的最大值记录；返回合并后的两个互异最大值。
    vector<int> v;
    for (auto x : a.a)
    {
        if (x)
        {
            v.push_back(*x);
        }
    }
    for (auto x : b.a)
    {
        if (x)
        {
            v.push_back(*x);
        }
    }
    sort(v.begin(), v.end(), greater<>());
    v.erase(unique(v.begin(), v.end()), v.end());
    MstTop2 c;
    for (int i = 0; i < min<int>(2, v.size()); i++)
    {
        c.a[i] = v[i];
    }
    return c;
}

struct MstVariants
{
    // mst 是最小生成树权值。
    int mst;
    // other 是不同边集的最小权值，不存在时为空。
    optional<int> other;
    // strict 是严格大于 mst 的最小生成树权值，不存在时为空。
    optional<int> strict;
};

optional<MstVariants> secondMST(int n, const vector<array<int, 3>> &edges)
{
    // edges[i][0..2] 是端点 u、v 与边权；图不连通时返回空。
    if (n == 0)
    {
        return MstVariants{0, nullopt, nullopt};
    }
    int m = edges.size();
    vector<int> ord(m);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(),
         ord.end(),
         [&](int x, int y)
         {
             return edges[x][2] < edges[y][2];
         });
    DSU dsu(n);
    vector<bool> used(m);
    vector<vector<array<int, 2>>> g(n);
    int mst = 0;
    int cnt = 0;
    for (auto id : ord)
    {
        auto [u, v, w] = edges[id];
        if (!dsu.merge(u, v))
        {
            continue;
        }
        used[id] = true;
        mst += w;
        cnt++;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    if (cnt != n - 1)
    {
        return nullopt;
    }
    int lg = bit_width((unsigned long long)max<int>(1, n));
    vector up(lg, vector<int>(n));
    vector mx(lg, vector<MstTop2>(n));
    vector<int> dep(n);
    vector<int> par(n, -1), seq{0};
    par[0] = 0;
    for (int i = 0; i < (int)seq.size(); i++)
    {
        int u = seq[i];
        up[0][u] = par[u];
        for (auto [v, w] : g[u])
        {
            if (v == par[u])
            {
                continue;
            }
            par[v] = u;
            dep[v] = dep[u] + 1;
            mx[0][v].a[0] = w;
            seq.push_back(v);
        }
    }
    for (int j = 1; j < lg; j++)
    {
        for (int u = 0; u < n; u++)
        {
            mx[j][u] = mstMergeTop(mx[j - 1][u], mx[j - 1][up[j - 1][u]]);
            up[j][u] = up[j - 1][up[j - 1][u]];
        }
    }
    auto pathMax = [&](int u, int v) -> MstTop2
    {
        // u、v 是树上点；返回路径边权中最大的两个互异值。
        MstTop2 ans;
        if (dep[u] < dep[v])
        {
            swap(u, v);
        }
        for (int j = lg - 1; j >= 0; j--)
        {
            if (dep[u] - (1LL << j) >= dep[v])
            {
                ans = mstMergeTop(ans, mx[j][u]);
                u = up[j][u];
            }
        }
        if (u == v)
        {
            return ans;
        }
        for (int j = lg - 1; j >= 0; j--)
        {
            if (up[j][u] != up[j][v])
            {
                ans = mstMergeTop(ans, mx[j][u]);
                ans = mstMergeTop(ans, mx[j][v]);
                u = up[j][u];
                v = up[j][v];
            }
        }
        ans = mstMergeTop(ans, mx[0][u]);
        return mstMergeTop(ans, mx[0][v]);
    };
    optional<int> other, strict;
    auto upd = [](optional<int> &x, int v)
    {
        // x 是当前最优候选，v 是新候选；保留较小值。
        if (!x || v < *x)
        {
            x = v;
        }
    };
    for (int id = 0; id < m; id++)
    {
        if (used[id])
        {
            continue;
        }
        auto [u, v, w] = edges[id];
        auto top = pathMax(u, v);
        if (top.a[0])
        {
            int cur = mst + w - *top.a[0];
            upd(other, cur);
            if (w > *top.a[0])
            {
                upd(strict, cur);
            }
            else if (w == *top.a[0] && top.a[1])
            {
                upd(strict, mst + w - *top.a[1]);
            }
        }
    }
    return MstVariants{mst, other, strict};
}
