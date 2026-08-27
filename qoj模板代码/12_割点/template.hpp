// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 12 割点


#include <bits/stdc++.h>
using namespace std;
#define int long long

class VertexBCC
{
    int n, ti = 0;
    vector<array<int, 2>> ed;
    vector<vector<array<int, 2>>> g;
    vector<int> dfn, low, st;
    vector<bool> cut;
    vector<vector<int>> bcc;

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
                    sort(cur.begin(), cur.end());
                    cur.erase(unique(cur.begin(), cur.end()), cur.end());
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
        // n 是点数；构造一张无边图。
    }

    int addEdge(int u, int v)
    {
        // u、v 是端点；加入允许重边的无向边并返回边编号。
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
    }

    const vector<bool> &cutVertices() const
    {
        // 无参数；返回割点标记。
        return cut;
    }

    const vector<vector<int>> &comps() const
    {
        // 无参数；返回全部点双的点集。
        return bcc;
    }
};
