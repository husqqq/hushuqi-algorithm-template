#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

class HopKarpCSR
{
    int nl, nr;
    // edge[i][0]、edge[i][1] 是第 i 条边的左右端点。
    vector<array<int, 2>> edge;
    // 左点 u 的邻边存放在 to[start[u]..start[u+1])。
    vector<int> start, to;
    // ml、mr 是左右匹配点，dep 是左点层数，lim 是本轮最短增广路长度。
    vector<int> ml, mr, dep;
    int lim = -1;

    void build()
    {
        // 无参数；按当前边集重建 CSR 邻接表，无返回值。
        start.assign(nl + 1, 0);
        for (auto e : edge)
        {
            start[e[0] + 1]++;
        }
        partial_sum(start.begin(), start.end(), start.begin());
        vector<int> cur = start;
        to.assign(edge.size(), 0);
        for (auto e : edge)
        {
            to[cur[e[0]]++] = e[1];
        }
    }

    bool bfs()
    {
        // 无参数；给全部最短增广路分层，存在增广路时返回 true。
        queue<int> q;
        fill(dep.begin(), dep.end(), -1);
        lim = -1;
        for (int u = 0; u < nl; u++)
        {
            if (ml[u] == -1)
            {
                dep[u] = 0;
                q.push(u);
            }
        }
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            if (lim != -1 && dep[u] >= lim)
            {
                continue;
            }
            for (int i = start[u]; i < start[u + 1]; i++)
            {
                int x = mr[to[i]];
                if (x == -1)
                {
                    lim = dep[u] + 1;
                }
                else if (dep[x] == -1 && (lim == -1 || dep[u] + 1 < lim))
                {
                    dep[x] = dep[u] + 1;
                    q.push(x);
                }
            }
        }
        return lim != -1;
    }

    bool dfs(int u)
    {
        // u 是当前左点；沿 BFS 层寻找最短增广路，成功时返回 true。
        for (int i = start[u]; i < start[u + 1]; i++)
        {
            int v = to[i];
            int x = mr[v];
            if (x == -1 && dep[u] + 1 == lim)
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
    HopKarpCSR(int nl, int nr) : nl(nl), nr(nr), ml(nl, -1), mr(nr, -1), dep(nl)
    {
        // nl、nr 是左右部点数；构造空边集，无返回值。
    }

    void addEdge(int u, int v)
    {
        // u 是左点，v 是右点；暂存一条边，无返回值。
        assert(0 <= u && u < nl && 0 <= v && v < nr); // 调试检查，可删。
        edge.push_back({u, v});
    }

    int matching()
    {
        // 无参数；重建 CSR，并从空匹配重算最大匹配边数。
        build();
        fill(ml.begin(), ml.end(), -1);
        fill(mr.begin(), mr.end(), -1);
        int ans = 0;
        while (bfs())
        {
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

    const vector<int> &leftMatch() const
    {
        // 无参数；返回左点到右点的匹配数组，未匹配为 -1。
        return ml;
    }

    const vector<int> &matchR() const
    {
        // 无参数；返回右点到左点的匹配数组，未匹配为 -1。
        return mr;
    }
};
