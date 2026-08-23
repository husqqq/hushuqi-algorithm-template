#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

class CentroidTree
{
    // n 是原树点数，g 是原树邻接表。
    int n;
    vector<vector<int>> g;
    // siz、tmp 是当前块的子树大小与父亲，ban 标记已选重心。
    vector<int> siz, tmp;
    vector<bool> ban;
    // par、dep 是点在点分树中的父节点与深度。
    vector<int> par, dep;

    int cen(int s)
    {
        // s 是当前未删除连通块中的一点；返回该块重心。
        vector<int> ord;
        auto dfs = [&](auto &&self, int u, int p) -> void
        {
            tmp[u] = p;
            siz[u] = 1;
            ord.push_back(u);
            for (int v : g[u])
            {
                if (v == p || ban[v])
                {
                    continue;
                }
                self(self, v, u);
                siz[u] += siz[v];
            }
        };
        dfs(dfs, s, -1);
        int total = ord.size(), c = s, best = total;
        for (int u : ord)
        {
            int mx = total - siz[u];
            for (int v : g[u])
            {
                if (!ban[v] && tmp[v] == u)
                {
                    mx = max(mx, siz[v]);
                }
            }
            if (mx < best)
            {
                best = mx;
                c = u;
            }
        }
        return c;
    }

    int build(int s, int p, int d)
    {
        // s 是当前块中的一点，p、d 是点分树父节点与深度；返回当前块重心。
        int c = cen(s);
        par[c] = p;
        dep[c] = d;
        ban[c] = true;
        for (int v : g[c])
        {
            if (!ban[v])
            {
                build(v, c, d + 1);
            }
        }
        return c;
    }

  public:
    explicit CentroidTree(const vector<vector<int>> &g)
        : n(g.size()), g(g), siz(n), tmp(n), ban(n), par(n, -1), dep(n)
    {
        // g 是 0 下标无向树；构造点分树，空树返回空父节点和深度表。
        if (n)
        {
            build(0, -1, 0);
        }
    }

    const vector<int> &parents() const
    {
        // 无参数；返回点分树父节点数组，点分树根为 -1。
        return par;
    }

    const vector<int> &depths() const
    {
        // 无参数；返回每个原树点在点分树中的深度。
        return dep;
    }
};
