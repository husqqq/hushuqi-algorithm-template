#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

class BitHungarian
{
    // nl、nr 是左右部点数，w 是存放 nr 位所需的 64 位块数。
    int nl, nr, w;
    // g[u][b] 的第 k 位表示左点 u 是否连接右点 64b+k。
    vector<vector<unsigned long long>> g;
    // ml、mr 是左右匹配点，vis 是一轮增广中访问过的右点位集。
    vector<int> ml, mr;
    vector<unsigned long long> vis;

    bool dfs(int u)
    {
        // u 是尝试增广的左点；找到增广路并改写匹配时返回 true。
        for (int b = 0; b < w; b++)
        {
            auto x = g[u][b] & ~vis[b];
            while (x)
            {
                int k = countr_zero(x);
                x &= x - 1;
                int v = b * 64 + k;
                if (v >= nr)
                {
                    continue;
                }
                vis[b] |= 1ULL << k;
                if (mr[v] == -1 || dfs(mr[v]))
                {
                    ml[u] = v;
                    mr[v] = u;
                    return true;
                }
                x &= ~vis[b];
            }
        }
        return false;
    }

  public:
    BitHungarian(int nl, int nr)
        : nl(nl), nr(nr), w((nr + 63) / 64), g(nl), ml(nl, -1), mr(nr, -1), vis(w)
    {
        // nl、nr 是左右部点数；构造位集邻接矩阵，无返回值。
        for (auto &row : g)
        {
            row.assign(w, 0);
        }
    }

    void addEdge(int u, int v)
    {
        // u 是左点，v 是右点；把对应邻接位设为 1，无返回值。
        assert(0 <= u && u < nl && 0 <= v && v < nr); // 调试检查，可删。
        g[u][v / 64] |= 1ULL << (v % 64);
    }

    int matching()
    {
        // 无参数；从空匹配重算并返回最大匹配边数。
        fill(ml.begin(), ml.end(), -1);
        fill(mr.begin(), mr.end(), -1);
        int ans = 0;
        for (int u = 0; u < nl; u++)
        {
            fill(vis.begin(), vis.end(), 0);
            ans += dfs(u);
        }
        return ans;
    }

    const vector<int> &leftMatch() const
    {
        // 无参数；返回每个左点匹配的右点，未匹配为 -1。
        return ml;
    }

    const vector<int> &matchR() const
    {
        // 无参数；返回每个右点匹配的左点，未匹配为 -1。
        return mr;
    }
};
