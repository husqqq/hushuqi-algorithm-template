#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Cactus
{
    // n 是图的点数，ti 是 DFS 时间戳。
    int n, ti = 0;
    // e[id] 是第 id 条无向边的两个端点。
    vector<pair<int, int>> e;
    // g[u] 的每项是邻点及无向边编号。
    vector<vector<pair<int, int>>> g;
    // Tarjan 时间戳、边栈及端点去重时间戳。
    vector<int> dfn, low, st, mark;
    int tag = 0;
    // cycles 的每项是一条简单环的边编号集合。
    vector<vector<int>> cycles;
    // ok 表示每个非桥点双是否都是单环。
    bool ok = true;

    Cactus(int n) : n(n), g(n), dfn(n), low(n), mark(n)
    {
        // n 是图的点数；构造一张没有边的图。
    }

    void add(int u, int v)
    {
        // u、v 是简单无向边端点；加入一条边。
        assert(0 <= u && u < n && 0 <= v && v < n && u != v); // 调试检查，可删。
        int id = e.size();
        e.push_back({u, v});
        g[u].push_back({v, id});
        g[v].push_back({u, id});
    }

    void dfs(int u, int pe)
    {
        // u 是当前点，pe 是父边编号；划分以 u 为根的点双。
        dfn[u] = low[u] = ++ti;
        for (auto [v, id] : g[u])
        {
            if (id == pe)
            {
                continue;
            }
            if (!dfn[v])
            {
                st.push_back(id);
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u])
                {
                    vector<int> q;
                    while (true)
                    {
                        int x = st.back();
                        st.pop_back();
                        q.push_back(x);
                        if (x == id)
                        {
                            break;
                        }
                    }
                    if (q.size() > 1)
                    {
                        tag++;
                        int cnt = 0;
                        for (int x : q)
                        {
                            for (int v : {e[x].first, e[x].second})
                            {
                                if (mark[v] != tag)
                                {
                                    mark[v] = tag;
                                    cnt++;
                                }
                            }
                        }
                        if (cnt != (int)q.size())
                        {
                            ok = false;
                        }
                        else
                        {
                            cycles.push_back(q);
                        }
                    }
                }
            }
            else if (dfn[v] < dfn[u])
            {
                st.push_back(id);
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    bool build()
    {
        // 无参数；重新分解全部点双，返回图是否为仙人掌。
        ti = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(mark.begin(), mark.end(), 0);
        st.clear();
        cycles.clear();
        ok = true;
        tag = 0;
        for (int i = 0; i < n; i++)
        {
            if (!dfn[i])
            {
                dfs(i, -1);
            }
        }
        return ok;
    }
};
