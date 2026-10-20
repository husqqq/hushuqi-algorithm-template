#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

class VertexBCC
{
    int n, ti = 0; // 点数与 DFS 时间戳。
    vector<array<int, 2>> ed; // 每条边的两个端点。
    vector<vector<array<int, 2>>> g; // 邻接项依次为邻点、边编号。
    vector<int> dfn, low, st; // 访问次序、最低可达次序、待归属边栈。
    vector<bool> cut; // 各点是否为割点。
    vector<vector<int>> bcc; // 各点双的升序点集。

    void dfs(int u, int pe)
    {
        // u 是当前点，pe 是父边编号；划分点双并标记割点。
        dfn[u] = low[u] = ++ti;
        int son = 0;
        for (auto [v, id] : g[u])
        {
            if (id == pe)
            {
                continue;
            }
            if (!dfn[v])
            {
                son++;
                st.push_back(id);
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u])
                {
                    if (pe != -1 || son > 1)
                    {
                        cut[u] = true;
                    }
                    vector<int> cur;
                    while (true)
                    {
                        int x = st.back();
                        st.pop_back();
                        cur.push_back(ed[x][0]);
                        cur.push_back(ed[x][1]);
                        if (x == id)
                        {
                            break;
                        }
                    }
                    bcc.push_back(move(cur));
                }
            }
            else if (dfn[v] < dfn[u])
            {
                st.push_back(id);
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (pe == -1 && son == 0)
        {
            bcc.push_back({u});
        }
    }

  public:
    VertexBCC(int n) : n(n), g(n), dfn(n), low(n), cut(n)
    {
        // n>=0 是点数；构造一张无边图。
    }

    int addEdge(int u, int v)
    {
        // u、v 是 [0,n) 内的端点；加入无向边并返回编号，允许重边，自环不参与分解。
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
        int id = ed.size();
        ed.push_back({u, v});
        g[u].push_back({v, id});
        g[v].push_back({u, id});
        return id;
    }

    void build()
    {
        // 无参数；清空旧结果并重新求割点和点双。
        ti = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(cut.begin(), cut.end(), false);
        st.clear();
        bcc.clear();
        for (int u = 0; u < n; u++)
        {
            if (!dfn[u])
            {
                dfs(u, -1);
            }
        }
        // 按分量编号登记归属，再按点编号回填，线性去重并保持点集升序。
        vector<int> head(n, -1); // 每个点的归属链表表头。
        vector<array<int, 2>> link; // 每项依次为分量编号、同点的上一归属位置。
        link.reserve(n + ed.size());
        for (int i = 0; i < (int)bcc.size(); i++)
        {
            for (int u : bcc[i])
            {
                if (head[u] != -1 && link[head[u]][0] == i) continue;
                link.push_back({i, head[u]});
                head[u] = (int)link.size() - 1;
            }
            bcc[i].clear();
        }
        for (int u = 0; u < n; u++)
        {
            for (int j = head[u]; j != -1; j = link[j][1])
            {
                bcc[link[j][0]].push_back(u);
            }
        }
    }

    const vector<bool> &cutVertices() const
    {
        // 无参数；返回割点标记。
        return cut;
    }

    const vector<vector<int>> &comps() const
    {
        // 无参数；返回全部点双，分量内点号升序，分量顺序为 DFS 出栈顺序。
        return bcc;
    }
};
