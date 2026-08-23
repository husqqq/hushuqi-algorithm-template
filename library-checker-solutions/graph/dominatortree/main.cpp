#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, source;
    cin >> n >> m >> source;
    vector<vector<int>> graph(n), reverseGraph(n);
    while (m--) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        reverseGraph[v].push_back(u);
    }

    vector<int> order(n, -1), vertex, parent(n, -1);
    auto dfs = [&](auto&& self, int u) -> void {
        order[u] = vertex.size();
        vertex.push_back(u);
        for (int v : graph[u]) {
            if (order[v] == -1) {
                parent[v] = u;
                self(self, v);
            }
        }
    };
    dfs(dfs, source);

    vector<int> semi(n);
    iota(semi.begin(), semi.end(), 0);
    vector<int> dsu(n, -1), label(n);
    iota(label.begin(), label.end(), 0);
    auto find = [&](auto&& self, int u) -> int {
        if (dsu[u] == -1) {
            return label[u];
        }
        int ancestor = dsu[u];
        self(self, ancestor);
        if (order[semi[label[u]]] > order[semi[label[ancestor]]]) {
            label[u] = label[ancestor];
        }
        if (dsu[ancestor] != -1) {
            dsu[u] = dsu[ancestor];
        }
        return label[u];
    };

    vector<vector<int>> bucket(n);
    vector<int> bestAncestor(n), immediate(n, -1);
    for (int index = int(vertex.size()) - 1; index >= 1; index--) {
        int u = vertex[index];
        for (int v : reverseGraph[u]) {
            if (order[v] != -1) {
                int candidate = semi[find(find, v)];
                if (order[candidate] < order[semi[u]]) {
                    semi[u] = candidate;
                }
            }
        }
        bucket[semi[u]].push_back(u);
        for (int v : bucket[parent[u]]) {
            bestAncestor[v] = find(find, v);
        }
        bucket[parent[u]].clear();
        dsu[u] = parent[u];
    }
    for (int index = 1; index < int(vertex.size()); index++) {
        int u = vertex[index];
        int v = bestAncestor[u];
        immediate[u] = semi[u] == semi[v] ? semi[u] : immediate[v];
    }
    immediate[source] = source;
    for (int i = 0; i < n; i++) {
        cout << immediate[i] << " \n"[i + 1 == n];
    }
    return 0;
}
