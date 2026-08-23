#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct KruskalTree
{
    // n 是原始点数，tot 是当前重构树结点数。
    int n, tot;
    // val[x] 是内部结点代表的边权，叶子处为空。
    vector<optional<int>> val;
    // ch[x] 是内部结点的两个孩子，叶子为 {-1,-1}。
    vector<array<int, 2>> ch;
    // fa 是建树时使用的并查集父亲。
    vector<int> fa;
    // dep、up 是 build 后的深度和倍增祖先表。
    vector<int> dep;
    vector<vector<int>> up;

    KruskalTree(int n) : n(n), tot(n), val(2 * n), ch(2 * n, {-1, -1}), fa(2 * n)
    {
        // n 是原始点数；建立 n 个叶子。
        iota(fa.begin(), fa.end(), 0);
    }

    int find(int x)
    {
        // x 是当前结点；迭代查找并压缩所在块的根。
        int r = x;
        while (fa[r] != r)
        {
            r = fa[r];
        }
        while (fa[x] != x)
        {
            int p = fa[x];
            fa[x] = r;
            x = p;
        }
        return r;
    }

    void addEdge(int u, int v, int w)
    {
        // u、v 是原图端点，w 是当前边权；按非降权序合并两个块。
        int ru = find(u);
        int rv = find(v);
        if (ru == rv)
        {
            return;
        }
        int c = tot++;
        val[c] = w;
        ch[c] = {ru, rv};
        fa[ru] = c;
        fa[rv] = c;
        fa[c] = c;
    }

    void build()
    {
        // 无参数；加完所有边后重建深度和 LCA 倍增表。
        int lg = bit_width((unsigned long long)max<int>(1, tot));
        up.assign(lg, vector<int>(tot, -1));
        dep.assign(tot, 0);
        vector<char> vis(tot);
        for (int i = 0; i < tot; i++)
        {
            int r = find(i);
            if (vis[r])
            {
                continue;
            }
            vis[r] = 1;
            auto dfs = [&](auto &&self, int x) -> void
            {
                for (int c : {ch[x][0], ch[x][1]})
                {
                    if (c < 0)
                    {
                        continue;
                    }
                    dep[c] = dep[x] + 1;
                    up[0][c] = x;
                    self(self, c);
                }
            };
            dfs(dfs, r);
        }
        for (int k = 1; k < lg; k++)
        {
            for (int x = 0; x < tot; x++)
            {
                if (up[k - 1][x] >= 0)
                {
                    up[k][x] = up[k - 1][up[k - 1][x]];
                }
            }
        }
    }

    int lca(int u, int v)
    {
        // u、v 是原图点；返回重构森林 LCA，不连通时返回 -1。
        if (find(u) != find(v))
        {
            return -1;
        }
        if (dep[u] < dep[v])
        {
            swap(u, v);
        }
        int d = dep[u] - dep[v];
        for (int k = 0; d; k++)
        {
            if (d & 1)
            {
                u = up[k][u];
            }
            d >>= 1;
        }
        if (u == v)
        {
            return u;
        }
        for (int k = (int)up.size() - 1; k >= 0; k--)
        {
            if (up[k][u] != up[k][v])
            {
                u = up[k][u];
                v = up[k][v];
            }
        }
        return up[0][u];
    }

    optional<int> bottleneck(int u, int v)
    {
        // u、v 是原图点；返回瓶颈路权，不连通时返回空，同一点返回 0。
        if (u == v)
        {
            return 0;
        }
        int l = lca(u, v);
        if (l < 0)
        {
            return nullopt;
        }
        return val[l];
    }
};
