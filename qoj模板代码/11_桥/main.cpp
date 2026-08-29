// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 11 桥


#include <bits/stdc++.h>
using namespace std;
#define int long long

class EdgeBCC
{
    int n, ti = 0;
    vector<array<int, 2>> ed;
    vector<vector<array<int, 2>>> g;
    vector<int> dfn, low;
    vector<bool> bridge;

    void dfs(int u, int pe)
    {
        // u 是当前点，pe 是父边编号；计算 low 并标记桥。
        dfn[u] = low[u] = ++ti;
        for (auto [v, id] : g[u])
        {
            if (id == pe)
            {
                continue;
            }
            if (!dfn[v])
            {
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u])
                {
                    bridge[id] = true;
                }
            }
            else
            {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

  public:
    EdgeBCC(int n) : n(n), g(n), dfn(n), low(n)
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
        // 无参数；重新计算全部桥。
        ti = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        bridge.assign(ed.size(), false);
        for (int u = 0; u < n; u++)
        {
            if (!dfn[u])
            {
                dfs(u, -1);
            }
        }
    }

    const vector<bool> &bridges() const
    {
        // 无参数；返回按边编号排列的桥标记。
        return bridge;
    }

    vector<int> comps() const
    {
        // 无参数；返回删除全部桥后的边双编号。
        vector<int> bel(n, -1);
        int cc = 0;
        for (int s = 0; s < n; s++)
        {
            if (bel[s] != -1)
            {
                continue;
            }
            queue<int> q;
            q.push(s);
            bel[s] = cc++;
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                for (auto [v, id] : g[u])
                {
                    if (!bridge[id] && bel[v] == -1)
                    {
                        bel[v] = bel[u];
                        q.push(v);
                    }
                }
            }
        }
        return bel;
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> e(m);
    EdgeBCC g(n);
    for (auto &[u, v] : e)
    {
        cin >> u >> v;
        --u;
        --v;
        g.addEdge(u, v);
    }
    g.build();
    const auto &bridge = g.bridges();
    for (int i = 0; i < m; i++)
    {
        if (bridge[i])
        {
            cout << e[i].first + 1 << ' ' << e[i].second + 1 << '\n';
        }
    }
}
