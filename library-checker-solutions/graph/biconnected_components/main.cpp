#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> edges(m);
    vector<vector<pair<int, int>>> adj(n);
    for (int i = 0; i < m; i++) {
        auto& [u, v] = edges[i];
        cin >> u >> v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }

    vector<int> dfn(n), low(n), stk;
    vector<vector<int>> components;
    int timer = 0;

    auto dfs = [&](auto&& self, int u, int parentEdge) -> void {
        dfn[u] = low[u] = ++timer;
        bool hasEdge = false;
        for (auto [v, id] : adj[u]) {
            if (id == parentEdge) {
                continue;
            }
            hasEdge = true;
            if (!dfn[v]) {
                stk.push_back(id);
                self(self, v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u]) {
                    vector<int> vertices;
                    while (true) {
                        int e = stk.back();
                        stk.pop_back();
                        vertices.push_back(edges[e].first);
                        vertices.push_back(edges[e].second);
                        if (e == id) {
                            break;
                        }
                    }
                    sort(vertices.begin(), vertices.end());
                    vertices.erase(unique(vertices.begin(), vertices.end()), vertices.end());
                    components.push_back(move(vertices));
                }
            } else if (dfn[v] < dfn[u]) {
                stk.push_back(id);
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (parentEdge == -1 && !hasEdge) {
            components.push_back({u});
        }
    };

    for (int i = 0; i < n; i++) {
        if (!dfn[i]) {
            dfs(dfs, i, -1);
        }
    }

    cout << components.size() << '\n';
    for (auto& component : components) {
        cout << component.size();
        for (int v : component) {
            cout << ' ' << v;
        }
        cout << '\n';
    }
    return 0;
}
