#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> adjacent(n);
    while (m--) {
        int u, v;
        cin >> u >> v;
        adjacent[u].push_back(v);
        adjacent[v].push_back(u);
    }

    vector<int> in(n), out(n), low(n, n), degree(n), path(n, n), next(n);
    iota(next.begin(), next.end(), 0);
    vector<char> visited(n);
    auto unite = [&](int u, int v) {
        swap(next[u], next[v]);
        degree[u] += degree[v];
    };
    int timer = 0;
    auto dfs = [&](auto&& self, int u, int parent) -> void {
        visited[u] = true;
        in[u] = timer++;
        for (int v : adjacent[u]) {
            if (v == u) {
                continue;
            }
            if (v == parent) {
                parent = n;
                continue;
            }
            if (visited[v]) {
                if (in[v] < in[u]) {
                    degree[u]++;
                    low[u] = min(low[u], in[v]);
                } else {
                    degree[u]--;
                    int x = path[u];
                    while (x != n && in[x] <= in[v] && in[v] < out[x]) {
                        unite(u, x);
                        x = path[x];
                    }
                    path[u] = x;
                }
                continue;
            }
            self(self, v, u);
            if (path[v] == n && degree[v] <= 1) {
                degree[u] += degree[v];
                low[u] = min(low[u], low[v]);
                continue;
            }
            if (degree[v] == 0) {
                v = path[v];
            }
            if (low[v] < low[u]) {
                low[u] = low[v];
                swap(v, path[u]);
            }
            while (v != n) {
                unite(u, v);
                v = path[v];
            }
        }
        out[u] = timer;
    };
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs(dfs, i, n);
        }
    }

    vector<char> emitted(n);
    vector<vector<int>> components;
    for (int i = 0; i < n; i++) {
        if (emitted[i]) {
            continue;
        }
        vector<int> component;
        int u = i;
        do {
            component.push_back(u);
            emitted[u] = true;
            u = next[u];
        } while (u != i);
        components.push_back(move(component));
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
