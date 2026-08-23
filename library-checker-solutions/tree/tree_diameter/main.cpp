#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

struct SearchResult {
    int vertex;
    vector<int> parent;
    vector<i64> distance;
};

SearchResult farthest(const vector<vector<pair<int, int>>>& adj, int start) {
    int n = adj.size();
    vector<int> parent(n, -1), stack{start};
    vector<i64> distance(n, -1);
    distance[start] = 0;
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        for (auto [v, w] : adj[u]) {
            if (v == parent[u]) {
                continue;
            }
            parent[v] = u;
            distance[v] = distance[u] + w;
            stack.push_back(v);
        }
    }
    int vertex = max_element(distance.begin(), distance.end()) - distance.begin();
    return {vertex, move(parent), move(distance)};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<vector<pair<int, int>>> adj(n);
    for (int i = 1; i < n; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }

    int start = farthest(adj, 0).vertex;
    auto result = farthest(adj, start);
    int target = result.vertex;
    vector<int> path;
    for (int u = target; u != -1; u = result.parent[u]) {
        path.push_back(u);
    }
    reverse(path.begin(), path.end());

    cout << result.distance[target] << ' ' << path.size() << '\n';
    for (int i = 0; i < (int)path.size(); i++) {
        cout << path[i] << " \n"[i + 1 == (int)path.size()];
    }
    return 0;
}
