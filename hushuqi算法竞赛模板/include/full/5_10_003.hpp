#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Dominator
{
    // n 是点数，ti 是本次 DFS 到达的点数。
    int n, ti = 0;
    // g 是原图，rg 是按 DFS 序存的反图，buc 是半支配点桶。
    vector<vector<int>> g, rg, buc;
    // dfn、rev、par、sdom、dom、dsu、lab 分别保存支配树算法的各项状态。
    vector<int> dfn, rev, par, sdom, dom, dsu, lab;

    Dominator(const vector<vector<int>> &g)
        : n(g.size()), g(g), rg(n + 1), buc(n + 1), dfn(n), rev(n + 1), par(n + 1), sdom(n + 1), dom(n + 1), dsu(n + 1),
          lab(n + 1)
    {
        // g 是有向邻接表；构造求解器。
    }

    void buildDfs(int rt)
    {
        // 从 rt 递归 DFS，并按 DFS 序建立树父点与反图。
        dfn[rt] = ++ti;
        rev[ti] = rt;
        auto dfs = [&](auto &&self, int u) -> void
        {
            for (int v : g[u])
            {
                assert(0 <= v && v < n); // 调试检查，可删。
                if (!dfn[v])
                {
                    dfn[v] = ++ti;
                    rev[ti] = v;
                    par[ti] = dfn[u];
                    self(self, v);
                }
            }
        };
        dfs(dfs, rt);
        for (int u = 0; u < n; u++)
        {
            if (!dfn[u])
            {
                continue;
            }
            for (int v : g[u])
            {
                if (dfn[v])
                {
                    rg[dfn[v]].push_back(dfn[u]);
                }
            }
        }
        for (int i = 1; i <= ti; i++)
        {
            sdom[i] = dsu[i] = lab[i] = i;
        }
    }

    int find(int u)
    {
        // 返回并查集路径上半支配序最小的标签，并递归压缩路径。
        if (u == dsu[u])
        {
            return u;
        }
        if (dsu[u] == dsu[dsu[u]])
        {
            return lab[u];
        }
        auto compress = [&](auto &&self, int x) -> int
        {
            int p = dsu[x];
            if (p == dsu[p])
            {
                return x;
            }
            int base = self(self, p);
            if (sdom[lab[p]] < sdom[lab[x]])
            {
                lab[x] = lab[p];
            }
            dsu[x] = base;
            return base;
        };
        compress(compress, u);
        return lab[u];
    }

    vector<int> work(int rt)
    {
        // rt 是入口；返回直接支配点，不可达点为 -1。
        assert(0 <= rt && rt < n); // 调试检查，可删。
        ti = 0;
        rg.assign(n + 1, {});
        buc.assign(n + 1, {});
        dfn.assign(n, 0);
        rev.assign(n + 1, 0);
        par.assign(n + 1, 0);
        sdom.assign(n + 1, 0);
        dom.assign(n + 1, 0);
        dsu.assign(n + 1, 0);
        lab.assign(n + 1, 0);
        buildDfs(rt);
        for (int i = ti; i; i--)
        {
            for (int v : rg[i])
            {
                sdom[i] = min(sdom[i], sdom[find(v)]);
            }
            if (i > 1)
            {
                buc[sdom[i]].push_back(i);
            }
            for (int v : buc[i])
            {
                int u = find(v);
                dom[v] = sdom[u] == sdom[v] ? sdom[v] : u;
            }
            if (i > 1)
            {
                dsu[i] = par[i];
            }
        }
        for (int i = 2; i <= ti; i++)
        {
            if (dom[i] != sdom[i])
            {
                dom[i] = dom[dom[i]];
            }
        }
        vector<int> ans(n, -1);
        ans[rt] = rt;
        for (int i = 2; i <= ti; i++)
        {
            ans[rev[i]] = rev[dom[i]];
        }
        return ans;
    }
};
