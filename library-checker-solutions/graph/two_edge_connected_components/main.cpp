#include <bits/stdc++.h>

using namespace std;

struct EBCC {
    int n, timer = 0;
    vector<pair<int, int>> edges;
    vector<vector<pair<int, int>>> adj;
    vector<int> dfn, low, belong;
    vector<bool> bridge;

    EBCC(int n = 0) : n(n), adj(n), dfn(n, -1), low(n), belong(n, -1) {}

    void addEdge(int u, int v) {
        int id = edges.size();
        edges.emplace_back(u, v);
        adj[u].emplace_back(v, id);
        adj[v].emplace_back(u, id);
    }

    void dfsBridge(int u, int parentEdge) {
        dfn[u] = low[u] = timer++;
        for (auto [v, id] : adj[u]) {
            if (id == parentEdge) {
                continue;
            }
            if (dfn[v] == -1) {
                dfsBridge(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) {
                    bridge[id] = true;
                }
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    void dfsComponent(int start, int id, vector<int>& component) {
        vector<int> stack{start};
        belong[start] = id;
        while (!stack.empty()) {
            int u = stack.back();
            stack.pop_back();
            component.push_back(u);
            for (auto [v, edge] : adj[u]) {
                if (!bridge[edge] && belong[v] == -1) {
                    belong[v] = id;
                    stack.push_back(v);
                }
            }
        }
    }

    vector<vector<int>> work() {
        bridge.assign(edges.size(), false);
        for (int i = 0; i < n; i++) {
            if (dfn[i] == -1) {
                dfsBridge(i, -1);
            }
        }

        vector<vector<int>> components;
        for (int i = 0; i < n; i++) {
            if (belong[i] == -1) {
                components.emplace_back();
                dfsComponent(i, components.size() - 1, components.back());
            }
        }
        return components;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    EBCC ebcc(n);
    while (m--) {
        int u, v;
        cin >> u >> v;
        ebcc.addEdge(u, v);
    }

    auto components = ebcc.work();
    cout << components.size() << '\n';
    for (auto& component : components) {
        cout << component.size();
        for (int u : component) {
            cout << ' ' << u;
        }
        cout << '\n';
    }
    return 0;
}
