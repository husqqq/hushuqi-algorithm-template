#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

class HopcroftKarp
{
    // nl、nr 是左右部点数；es 暂存加边，off、to 是建成后的 CSR 邻接表。
    int nl, nr;
    vector<array<int, 2>> es; // 每项依次为左点和右点。
    vector<int32_t> off, to;
    // ml、mr 是匹配，dep 是分层距离，it 是当前弧，que 是复用的 BFS 队列。
    vector<int32_t> ml, mr, dep, it, que;
    int32_t lim;

    void build()
    {
        // 无参数；把暂存边建成连续 CSR 邻接表。
        off.assign(nl + 1, 0);
        for (auto [u, v] : es)
        {
            off[u + 1]++;
        }
        for (int i = 1; i <= nl; i++)
        {
            off[i] += off[i - 1];
        }
        to.resize(es.size());
        vector<int32_t> cur = off;
        for (auto [u, v] : es)
        {
            to[cur[u]++] = v;
        }
    }

    bool bfs()
    {
        // 无参数；建立最短增广路分层，返回是否存在增广路。
        fill(dep.begin(), dep.end(), -1);
        int32_t ql = 0, qr = 0;
        lim = -1;
        for (int32_t u = 0; u < nl; u++)
        {
            if (ml[u] == -1)
            {
                dep[u] = 0;
                que[qr++] = u;
            }
        }
        while (ql < qr)
        {
            int32_t u = que[ql++];
            if (lim != -1 && dep[u] >= lim)
            {
                continue;
            }
            for (int32_t i = off[u]; i < off[u + 1]; i++)
            {
                int32_t w = mr[to[i]];
                if (w == -1)
                {
                    lim = dep[u] + 1;
                }
                else if (dep[w] == -1)
                {
                    dep[w] = dep[u] + 1;
                    que[qr++] = w;
                }
            }
        }
        return lim != -1;
    }

    bool dfs(int32_t u)
    {
        // u 是左部点；沿分层图寻找增广路，返回是否成功。
        for (int32_t &i = it[u]; i < off[u + 1]; i++)
        {
            int32_t v = to[i], w = mr[v];
            if (w == -1)
            {
                if (dep[u] + 1 != lim)
                {
                    continue;
                }
            }
            else if (dep[w] != dep[u] + 1 || !dfs(w))
            {
                continue;
            }
            ml[u] = v;
            mr[v] = u;
            return true;
        }
        dep[u] = -1;
        return false;
    }

  public:
    HopcroftKarp(int nl, int nr)
        : nl(nl), nr(nr), ml(nl, -1), mr(nr, -1), dep(nl),
          it(nl), que(nl)
    {
        // nl、nr 是左右部点数；建立空二分图。
        assert(nl >= 0 && nr >= 0);
        assert(nl <= INT32_MAX && nr <= INT32_MAX);
    }

    void reserve(int m)
    {
        // m 是预计边数；预留暂存空间，不改变图。
        assert(m >= 0);
        es.reserve(m);
    }

    void addEdge(int u, int v)
    {
        // u、v 是左右部点编号；加入一条边。
        assert(0 <= u && u < nl && 0 <= v && v < nr);
        es.push_back({u, v});
    }

    int matching()
    {
        // 无参数；清空匹配并返回最大匹配边数。
        build();
        fill(ml.begin(), ml.end(), -1);
        fill(mr.begin(), mr.end(), -1);
        int ans = 0;
        while (bfs())
        {
            copy(off.begin(), off.begin() + nl, it.begin());
            for (int32_t u = 0; u < nl; u++)
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
        // 无参数；返回左点匹配数组，未匹配为 -1。
        return {ml.begin(), ml.end()};
    }

    vector<int> rightMatch() const
    {
        // 无参数；返回右点匹配数组，未匹配为 -1。
        return {mr.begin(), mr.end()};
    }
};
