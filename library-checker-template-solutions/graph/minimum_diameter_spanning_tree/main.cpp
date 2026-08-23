
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct MinimumDiameterSpanningTree
{
    struct Edge
    {
        int to;
        long long w;
    };

    struct State
    {
        long long d;
        int u, fa;

        bool operator>(const State &o) const
        {
            return tie(d, u, fa) > tie(o.d, o.u, o.fa);
        }
    };

    int n;
    vector<vector<Edge>> g;

    MinimumDiameterSpanningTree(int n_) : n(n_), g(n_)
    {
        assert(n > 0);
    }

    void addEdge(int u, int v, long long w)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && w >= 0);
        g[u].push_back({v, w * 2});
        g[v].push_back({u, w * 2});
    }

    pair<vector<long long>, vector<int>> dijkstra(int s, const vector<vector<Edge>> &a) const
    {
        int m = a.size();
        const long long INF = numeric_limits<long long>::max() / 4;
        vector<long long> dis(m, INF);
        vector<int> fa(m, -1);
        vector<char> vis(m);
        priority_queue<State, vector<State>, greater<State>> q;
        q.push({0, s, s});
        while (!q.empty())
        {
            auto [d, u, p] = q.top();
            q.pop();
            if (vis[u])
            {
                continue;
            }
            vis[u] = true;
            dis[u] = d;
            fa[u] = p;
            for (auto [v, w] : a[u])
            {
                if (!vis[v])
                {
                    assert(d <= INF - w);
                    q.push({d + w, v, u});
                }
            }
        }
        return {dis, fa};
    }

    pair<long long, vector<pair<int, int>>> solve() const
    {
        const long long INF = numeric_limits<long long>::max() / 4;
        vector<vector<long long>> dis(n);
        vector<vector<int>> ord(n, vector<int>(n));
        long long bestPoint = INF;
        int center = 0;
        for (int s = 0; s < n; s++)
        {
            dis[s] = dijkstra(s, g).first;
            iota(ord[s].begin(), ord[s].end(), 0);
            stable_sort(ord[s].begin(), ord[s].end(), [&](int x, int y)
            {
                return dis[s][x] > dis[s][y];
            });
            assert(dis[s][ord[s][0]] < INF);
            long long cur = dis[s][ord[s][0]] * 2;
            if (cur < bestPoint)
            {
                bestPoint = cur;
                center = s;
            }
        }

        long long bestEdge = INF, left = 0, right = 0;
        int cu = 0, cv = 0;
        for (int u = 0; u < n; u++)
        {
            for (auto [v, w] : g[u])
            {
                if (v <= u)
                {
                    continue;
                }
                int y = ord[u][0];
                for (int x : ord[u])
                {
                    if (dis[v][x] > dis[v][y])
                    {
                        long long cur = dis[v][y] + dis[u][x] + w;
                        if (cur < bestEdge)
                        {
                            bestEdge = cur;
                            cu = u;
                            cv = v;
                            left = cur / 2 - dis[u][x];
                            right = w - left;
                        }
                        y = x;
                    }
                }
            }
        }

        vector<vector<Edge>> a = g;
        int root = center;
        bool inside = bestEdge < bestPoint;
        if (inside)
        {
            root = n;
            a.push_back({});
            a[root].push_back({cu, left});
            a[root].push_back({cv, right});
        }
        auto fa = dijkstra(root, a).second;
        vector<pair<int, int>> tree;
        if (inside)
        {
            for (int u = 0; u < n; u++)
            {
                if (u != cu && u != cv)
                {
                    tree.push_back({u, fa[u]});
                }
            }
            tree.push_back({cu, cv});
        }
        else
        {
            for (int u = 0; u < n; u++)
            {
                if (u != root)
                {
                    tree.push_back({u, fa[u]});
                }
            }
        }
        return {min(bestPoint, bestEdge) / 2, tree};
    }
};

signed main()
{
    int n, m; cin >> n >> m;
    MinimumDiameterSpanningTree tree(n);
    map<pair<int, int>, pair<int, int>> best;
    for (int i = 0; i < m; i++)
    {
        int u, v, w; cin >> u >> v >> w;
        tree.addEdge(u, v, w);
        if (u > v) swap(u, v);
        if (!best.count({u, v}) || w < best[{u, v}].first) best[{u, v}] = {w, i};
    }
    auto [diameter, e] = tree.solve();
    cout << diameter << '\n';
    for (int i = 0; i < (int)e.size(); i++)
    {
        auto [u, v] = e[i];
        if (u > v) swap(u, v);
        cout << best[{u, v}].second << " \n"[i + 1 == (int)e.size()];
    }
}
