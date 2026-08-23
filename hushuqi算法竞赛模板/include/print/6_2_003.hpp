#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

class EdgeDecomp
{
    // original 是原树点数；扩展图中编号 >= original 的点都是零权虚点。
    int original;
    // g[u] 的每项依次是邻点、边权、扩展树边编号。
    vector<vector<array<int, 3>>> g;
    // real[u] 表示该扩展点代表几个原点，ban[id] 标记已经切断的扩展边。
    vector<int> real, sub, par, pe;
    vector<bool> ban;
    // cut[i] 依次是父切分记录编号、当前切边两个扩展端点。
    vector<array<int, 3>> cut;

    int node(bool isReal)
    {
        // isReal 表示是否对应原点；新增扩展点并返回编号。
        g.push_back({});
        real.push_back(isReal);
        sub.push_back(0);
        par.push_back(-1);
        pe.push_back(-1);
        return g.size() - 1;
    }

    void edge(int u, int v, int w)
    {
        // u、v 是扩展点，w 是边权；加入一条带编号无向边。
        int id = ban.size();
        ban.push_back(false);
        g[u].push_back({v, w, id});
        g[v].push_back({u, w, id});
    }

    void build(int s, int cutPar)
    {
        // s 是当前块内一点，cutPar 是分治树父切边记录编号。
        vector<int> ord;
        auto dfs = [&](auto &&self, int u, int p, int pid) -> void
        {
            par[u] = p;
            pe[u] = pid;
            sub[u] = real[u];
            ord.push_back(u);
            for (auto x : g[u])
            {
                int v = x[0], id = x[2];
                if (v == p || ban[id])
                {
                    continue;
                }
                self(self, v, u, id);
                sub[u] += sub[v];
            }
        };
        dfs(dfs, s, -1, -1);
        int total = sub[s];
        if (total <= 1)
        {
            return;
        }
        array<int, 4> best{numeric_limits<int>::max(), -1, -1, -1};
        for (int v : ord)
        {
            if (v == s)
            {
                continue;
            }
            array<int, 4> cur{max(sub[v], total - sub[v]), pe[v], par[v], v};
            if (cur[0] < best[0])
            {
                best = cur;
            }
        }
        assert(best[1] != -1); // 调试检查，可删
        int id = cut.size();
        cut.push_back({cutPar, best[2], best[3]});
        ban[best[1]] = true;
        build(best[2], id);
        build(best[3], id);
    }

  public:
    explicit EdgeDecomp(const vector<vector<array<int, 2>>> &tree) : original(tree.size())
    {
        // tree[u] 的每项依次是邻点和边权；二叉化后建立边分治树。
        for (int i = 0; i < original; i++)
        {
            node(true);
        }
        vector<int> p(original, -1);
        vector<vector<array<int, 2>>> child(original);
        if (original)
        {
            auto dfs = [&](auto &&self, int u) -> void
            {
                for (auto [v, w] : tree[u])
                {
                    if (v == p[u])
                    {
                        continue;
                    }
                    p[v] = u;
                    child[u].push_back({v, w});
                    self(self, v);
                }
            };
            dfs(dfs, 0);
        }
        for (int u = 0; u < original; u++)
        {
            int cur = u;
            for (int i = 0; i < (int)child[u].size(); i++)
            {
                auto [v, w] = child[u][i];
                if (i + 1 == (int)child[u].size())
                {
                    edge(cur, v, w);
                }
                else
                {
                    int z = node(false);
                    edge(cur, z, 0);
                    edge(z, v, w);
                    cur = z;
                }
            }
        }
        if (original)
        {
            build(0, -1);
        }
    }

    const vector<array<int, 3>> &cuts() const
    {
        // 无参数；返回每条切分的父切分编号和当前切边两端点。
        return cut;
    }

    int origNodes() const
    {
        // 无参数；返回原树点数，用于区分扩展图虚点。
        return original;
    }
};
