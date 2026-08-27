#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<pair<int,int>> edges(m);
    vector<vector<pair<int,int>>> g(n);
    for (int i = 0; i < m; i++)
    {
        auto &[u, v] = edges[i];
        cin >> u >> v;
        --u; --v;
        g[u].push_back({v, i});
        g[v].push_back({u, i});
    }
    vector<int> dfn(n), low(n);
    vector<char> bridge(m);
    int timer = 0;
    auto dfs = [&](auto &&self, int u, int pe) -> void
    {
        dfn[u] = low[u] = ++timer;
        for (auto [v, id] : g[u])
        {
            if (id == pe) continue;
            if (!dfn[v])
            {
                self(self, v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) bridge[id] = true;
            }
            else low[u] = min(low[u], dfn[v]);
        }
    };
    for (int i = 0; i < n; i++) if (!dfn[i]) dfs(dfs, i, -1);
    for (int i = 0; i < m; i++) if (bridge[i]) cout << edges[i].first + 1 << ' ' << edges[i].second + 1 << '\n';
}
