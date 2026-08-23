#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

class Hungarian
{
    // nl、nr 是左右部点数，g[u] 保存左点 u 的全部邻点。
    int nl, nr;
    vector<vector<int>> g;
    // ml[u]、mr[v] 分别保存左右点当前匹配点，-1 表示未匹配。
    vector<int> ml, mr;
    // vis[v] 是本轮增广是否已经访问右点 v 的时间戳。
    vector<int> vis;
    int ti = 0;

    bool dfs(int u)
    {
        // u 是尝试增广的左点；找到增广路并改写匹配时返回 true。
        for (auto v : g[u])
        {
            if (vis[v] == ti)
            {
                continue;
            }
            vis[v] = ti;
            if (mr[v] == -1 || dfs(mr[v]))
            {
                ml[u] = v;
                mr[v] = u;
                return true;
            }
        }
        return false;
    }

  public:
    Hungarian(int nl, int nr) : nl(nl), nr(nr), g(nl), ml(nl, -1), mr(nr, -1), vis(nr)
    {
        // nl、nr 是左右部点数；构造空二分图，无返回值。
    }

    void addEdge(int u, int v)
    {
        // u 是左点，v 是右点；加入一条二分图边，无返回值。
        assert(0 <= u && u < nl && 0 <= v && v < nr); // 调试检查，可删。
        g[u].push_back(v);
    }

    int matching()
    {
        // 无参数；从空匹配重算并返回最大匹配大小。
        fill(ml.begin(), ml.end(), -1);
        fill(mr.begin(), mr.end(), -1);
        fill(vis.begin(), vis.end(), 0);
        ti = 0;
        for (int u = 0; u < nl; u++)
        {
            if (ml[u] == -1)
            {
                ti++;
                dfs(u);
            }
        }
        return count_if(ml.begin(),
                        ml.end(),
                        [](int v)
                        {
                            return v != -1;
                        });
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
