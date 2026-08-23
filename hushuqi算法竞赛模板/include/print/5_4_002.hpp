#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

class Kosaraju2SAT
{
    // n 是变量数，g、rg 是文字蕴含图及其反图。
    int n;
    vector<vector<int>> g, rg;

    void edge(int u, int v)
    {
        // u、v 是文字节点；加入 u 到 v 的蕴含边及反边索引，无返回值。
        g[u].push_back(v);
        rg[v].push_back(u);
    }

  public:
    Kosaraju2SAT(int n) : n(n), g(2 * n), rg(2 * n)
    {
        // n 是变量数；构造没有子句的 2-SAT，无返回值。
    }

    void imply(int x, bool xv, int y, bool yv)
    {
        // x=xv 是前件，y=yv 是后件；加入蕴含及其逆否边。
        int a = 2 * x + xv, b = 2 * y + yv;
        edge(a, b);
        edge(b ^ 1, a ^ 1);
    }

    void addOr(int x, bool xv, int y, bool yv)
    {
        // x=xv、y=yv 是两个文字；加入二元子句。
        imply(x, !xv, y, yv);
    }

    optional<vector<bool>> solve() const
    {
        // 无参数；返回一组可行变量值，不可满足时返回空。
        vector<int> ord, bel(2 * n, -1);
        vector<bool> vis(2 * n);
        auto dfs1 = [&](auto &&self, int u) -> void
        {
            // u 是原图文字；记录 DFS 结束顺序，无返回值。
            vis[u] = true;
            for (auto v : g[u])
            {
                if (!vis[v])
                {
                    self(self, v);
                }
            }
            ord.push_back(u);
        };
        int cc = 0;
        auto dfs2 = [&](auto &&self, int u) -> void
        {
            // u 是反图文字；标记当前强连通分量，无返回值。
            bel[u] = cc;
            for (auto v : rg[u])
            {
                if (bel[v] == -1)
                {
                    self(self, v);
                }
            }
        };
        for (int u = 0; u < 2 * n; u++)
        {
            if (!vis[u])
            {
                dfs1(dfs1, u);
            }
        }
        reverse(ord.begin(), ord.end());
        for (auto u : ord)
        {
            if (bel[u] == -1)
            {
                dfs2(dfs2, u);
                cc++;
            }
        }
        vector<bool> ans(n);
        for (int x = 0; x < n; x++)
        {
            if (bel[2 * x] == bel[2 * x + 1])
            {
                return nullopt;
            }
            ans[x] = bel[2 * x] < bel[2 * x + 1];
        }
        return ans;
    }
};
