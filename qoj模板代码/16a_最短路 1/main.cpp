

#include <bits/stdc++.h>
using namespace std;
#define int long long

using WEdge = array<int, 2>;

template <class Dist = long long>
vector<optional<Dist>> dijkstraSparse(const vector<vector<WEdge>> &g, int s)
{
    // g 是稀疏图邻接表，s 是源点；返回最短距离，不可达点为空。
    int n = g.size();
    vector<optional<Dist>> d(n);
    priority_queue<pair<Dist, int>, vector<pair<Dist, int>>, greater<>> q;
    d[s] = 0;
    q.push({0, s});
    while (!q.empty())
    {
        auto [du, u] = q.top();
        q.pop();
        if (!d[u].has_value() || du != *d[u])
        {
            continue;
        }
        for (auto [v, w] : g[u])
        {
            assert(w >= 0); // 调试检查，可删：Dijkstra 要求非负边权。
            Dist z = (Dist)w;
            Dist nd = du + z;
            if (!d[v].has_value() || nd < *d[v])
            {
                d[v] = nd;
                q.push({nd, v});
            }
        }
    }
    return d;
}

template <class Dist = long long>
vector<optional<Dist>> dijkstraDense(
    const vector<vector<optional<int>>> &w, int s)
{
    // w 是邻接矩阵，空值表示缺边；返回最短距离，不可达点为空。
    int n = w.size();
    vector<optional<Dist>> d(n);
    vector<bool> vis(n);
    d[s] = 0;
    for (int z = 0; z < n; z++)
    {
        int u = -1;
        for (int i = 0; i < n; i++)
        {
            if (!vis[i] && d[i].has_value() && (u < 0 || *d[i] < *d[u]))
            {
                u = i;
            }
        }
        if (u < 0)
        {
            break;
        }
        vis[u] = true;
        for (int v = 0; v < n; v++)
        {
            if (w[u][v].has_value())
            {
                assert(*w[u][v] >= 0); // 调试检查，可删：Dijkstra 要求非负边权。
                Dist z = (Dist)*w[u][v];
                Dist nd = *d[u] + z;
                if (!d[v].has_value() || nd < *d[v])
                {
                    d[v] = nd;
                }
            }
        }
    }
    return d;
}

template<class Dist = long long>
pair<vector<optional<Dist>>, vector<int>> shortestPathTree(
    const vector<vector<WEdge>> &g, int s)
{
    // g 是非负权有向图；返回最短距离与一棵最短路径树的父节点。
    int n = g.size();
    assert(0 <= s && s < n); // 调试检查，可删。
    auto d = dijkstraSparse<Dist>(g, s);
    vector<int> par(n, -1), vis(n);
    queue<int> q;
    vis[s] = 1;
    q.push(s);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (auto [v, w] : g[u])
        {
            Dist z = (Dist)w;
            if (!vis[v] && d[v].has_value() && *d[u] + z == *d[v])
            {
                vis[v] = 1;
                par[v] = u;
                q.push(v);
            }
        }
    }
    return {d, par};
}

signed main()
{
    int n, m, s; cin >> n >> m >> s;
    --s;
    vector<vector<WEdge>> g(n);
    while (m--)
    {
        int u, v, w; cin >> u >> v >> w;
        --u;
        --v;
        g[u].push_back({v, w});
    }
    auto d = dijkstraSparse(g, s);
    for (int i = 0; i < n; i++)
    {
        if (i) cout << ' ';
        if (d[i].has_value()) cout << *d[i];
        else cout << -1;
    }
    cout << '\n';
}
