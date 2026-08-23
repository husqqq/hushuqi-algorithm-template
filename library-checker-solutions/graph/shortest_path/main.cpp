#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, source, target;
    cin >> n >> m >> source >> target;
    vector<vector<pair<int, i64>>> adj(n);
    while (m--) {
        int u, v;
        i64 w;
        cin >> u >> v >> w;
        adj[u].emplace_back(v, w);
    }

    constexpr i64 INF = numeric_limits<i64>::max() / 2;
    vector<i64> distance(n, INF);
    vector<int> parent(n, -1);
    priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<>> queue;
    distance[source] = 0;
    queue.emplace(0, source);

    while (!queue.empty()) {
        auto [d, u] = queue.top();
        queue.pop();
        if (d != distance[u]) {
            continue;
        }
        for (auto [v, w] : adj[u]) {
            if (distance[v] > d + w) {
                distance[v] = d + w;
                parent[v] = u;
                queue.emplace(distance[v], v);
            }
        }
    }

    if (distance[target] == INF) {
        cout << -1 << '\n';
        return 0;
    }

    vector<int> path;
    for (int u = target; u != -1; u = parent[u]) {
        path.push_back(u);
    }
    reverse(path.begin(), path.end());

    cout << distance[target] << ' ' << path.size() - 1 << '\n';
    for (int i = 1; i < (int)path.size(); i++) {
        cout << path[i - 1] << ' ' << path[i] << '\n';
    }
    return 0;
}
