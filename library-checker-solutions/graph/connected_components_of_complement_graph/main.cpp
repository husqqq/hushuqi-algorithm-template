#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> graph(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    vector<int> remaining(n), marked(n), component(n, -1);
    iota(remaining.begin(), remaining.end(), 0);
    vector<vector<int>> components;
    while (!remaining.empty()) {
        int start = remaining.back();
        remaining.pop_back();
        components.push_back({});
        queue<int> queue;
        queue.push(start);
        component[start] = components.size() - 1;
        while (!queue.empty()) {
            int u = queue.front();
            queue.pop();
            components.back().push_back(u);
            for (int v : graph[u]) marked[v] = true;
            vector<int> nextRemaining;
            for (int v : remaining) {
                if (marked[v]) nextRemaining.push_back(v);
                else component[v] = component[start], queue.push(v);
            }
            for (int v : graph[u]) marked[v] = false;
            remaining = move(nextRemaining);
        }
    }
    cout << components.size() << '\n';
    for (const auto& vertices : components) {
        cout << vertices.size();
        for (int vertex : vertices) cout << ' ' << vertex;
        cout << '\n';
    }
}
