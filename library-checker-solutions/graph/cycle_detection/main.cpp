#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n);
    for (int id = 0; id < m; id++) {
        int u, v;
        cin >> u >> v;
        adj[u].emplace_back(v, id);
    }

    vector<int> color(n), parent(n, -1), parentEdge(n, -1), cycle;
    for (int start = 0; start < n && cycle.empty(); start++) {
        if (color[start]) {
            continue;
        }
        vector<pair<int, int>> stack{{start, 0}};
        color[start] = 1;
        while (!stack.empty() && cycle.empty()) {
            int u = stack.back().first;
            int& index = stack.back().second;
            if (index == (int)adj[u].size()) {
                color[u] = 2;
                stack.pop_back();
                continue;
            }
            auto [v, edge] = adj[u][index++];
            if (color[v] == 0) {
                color[v] = 1;
                parent[v] = u;
                parentEdge[v] = edge;
                stack.emplace_back(v, 0);
            } else if (color[v] == 1) {
                vector<int> path;
                for (int x = u; x != v; x = parent[x]) {
                    path.push_back(parentEdge[x]);
                }
                reverse(path.begin(), path.end());
                cycle = move(path);
                cycle.push_back(edge);
            }
        }
    }

    if (cycle.empty()) {
        cout << -1 << '\n';
    } else {
        cout << cycle.size() << '\n';
        for (int edge : cycle) {
            cout << edge << '\n';
        }
    }
    return 0;
}
