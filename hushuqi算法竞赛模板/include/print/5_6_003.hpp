#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

class HopcroftKarp
{
    // nl、nr 是左右部点数，g 是从左部出发的邻接表。
    int nl, nr;
    vector<vector<int>> g;
    // ml、mr 是左右匹配点；dep、it、que 分别是层数、当前弧和复用队列。
    vector<int> ml, mr, dep, it, que;

    bool bfs()
    {
        // 无参数；给交替图分层，存在增广路时返回 true。
        fill(dep.begin(), dep.end(), -1);
        int ql = 0, qr = 0;
        bool found = false;
        for (int u = 0; u < nl; u++)
        {
            if (ml[u] == -1)
            {
                dep[u] = 0;
                que[qr++] = u;
            }
        }
        while (ql < qr)
        {
            int u = que[ql++];
            for (int v : g[u])
            {
                int x = mr[v];
                if (x == -1)
                {
                    found = true;
                }
                else if (dep[x] == -1)
                {
                    dep[x] = dep[u] + 1;
                    que[qr++] = x;
                }
            }
        }
        return found;
    }

    bool dfs(int u)
    {
        // u 是当前左点；沿 BFS 层寻找增广路，成功时返回 true。
        for (int &i = it[u]; i < (int)g[u].size(); i++)
        {
            int v = g[u][i];
            int x = mr[v];
            if (x == -1)
            {
                ml[u] = v;
                mr[v] = u;
                return true;
            }
            if (x != -1 && dep[x] == dep[u] + 1 && dfs(x))
            {
                ml[u] = v;
                mr[v] = u;
                return true;
            }
        }
        dep[u] = -1;
        return false;
    }

  public:
    HopcroftKarp(int nl, int nr)
        : nl(nl), nr(nr), g(nl), ml(nl, -1), mr(nr, -1), dep(nl), it(nl), que(nl)
    {
        // nl、nr 是左右部点数；构造空二分图，无返回值。
        assert(nl <= numeric_limits<int>::max() && nr <= numeric_limits<int>::max()); // 调试检查，可删。
    }

    void reserve(int u, int m)
    {
        // u 是左点，m 是预计邻边数；预留空间但不改变图。
        assert(0 <= u && u < nl && m >= 0); // 调试检查，可删。
        g[u].reserve(m);
    }

    void addEdge(int u, int v)
    {
        // u 是左点，v 是右点；加入一条边，无返回值。
        assert(0 <= u && u < nl && 0 <= v && v < nr); // 调试检查，可删。
        g[u].push_back((int)v);
    }

    int matching()
    {
        // 无参数；从空匹配重算并返回最大匹配边数。
        fill(ml.begin(), ml.end(), -1);
        fill(mr.begin(), mr.end(), -1);
        int ans = 0;
        while (bfs())
        {
            fill(it.begin(), it.end(), 0);
            for (int u = 0; u < nl; u++)
            {
                if (ml[u] == -1)
                {
                    ans += dfs(u);
                }
            }
        }
        return ans;
    }

    vector<int> leftMatch() const
    {
        // 无参数；返回左点到右点的匹配数组，未匹配为 -1。
        return {ml.begin(), ml.end()};
    }

    vector<int> matchR() const
    {
        // 无参数；返回右点到左点的匹配数组，未匹配为 -1。
        return {mr.begin(), mr.end()};
    }
};
