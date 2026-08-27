#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int,int>>> g(n);
    for (int i = 0; i < m; i++)
    {
        int u, v; cin >> u >> v; --u; --v;
        g[u].push_back({v, i}); g[v].push_back({u, i});
    }
    vector<int> dfn(n), low(n);
    vector<char> cut(n);
    int timer = 0;
    auto dfs = [&](auto &&self, int u, int pe) -> void
    {
        dfn[u] = low[u] = ++timer;
        int children = 0;
        for (auto [v, id] : g[u])
        {
            if (id == pe) continue;
            if (!dfn[v])
            {
                children++;
                self(self, v, id);
                low[u] = min(low[u], low[v]);
                if (pe != -1 && low[v] >= dfn[u]) cut[u] = true;
            }
            else low[u] = min(low[u], dfn[v]);
        }
        if (pe == -1 && children > 1) cut[u] = true;
    };
    for (int i = 0; i < n; i++) if (!dfn[i]) dfs(dfs, i, -1);
    cout << count(cut.begin(), cut.end(), 1) << '\n';
    bool first = true;
    for (int i = 0; i < n; i++) if (cut[i]) { if (!first) cout << ' '; first = false; cout << i + 1; }
    cout << '\n';
}
