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
        adj[v].emplace_back(u, id);
    }

    vector<int> color(n), parent(n, -1), parentEdge(n, -1);
    vector<int> cycleVertices, cycleEdges;
    for (int start = 0; start < n && cycleEdges.empty(); start++) {
        if (color[start]) {
            continue;
        }
        color[start] = 1;
        vector<pair<int, int>> stack{{start, 0}};
        while (!stack.empty() && cycleEdges.empty()) {
            int u = stack.back().first;
            int& index = stack.back().second;
            if (index == (int)adj[u].size()) {
                color[u] = 2;
                stack.pop_back();
                continue;
            }
            auto [v, edge] = adj[u][index++];
            if (edge == parentEdge[u]) {
                continue;
            }
            if (color[v] == 0) {
                color[v] = 1;
                parent[v] = u;
                parentEdge[v] = edge;
                stack.emplace_back(v, 0);
            } else if (color[v] == 1) {
                vector<int> vertices, edges;
                for (int x = u; x != v; x = parent[x]) {
                    vertices.push_back(x);
                    edges.push_back(parentEdge[x]);
                }
                vertices.push_back(v);
                reverse(vertices.begin(), vertices.end());
                reverse(edges.begin(), edges.end());
                edges.push_back(edge);
                cycleVertices = move(vertices);
                cycleEdges = move(edges);
            }
        }
    }

    if (cycleEdges.empty()) {
        cout << -1 << '\n';
        return 0;
    }
    cout << cycleEdges.size() << '\n';
    for (int i = 0; i < (int)cycleVertices.size(); i++) {
        cout << cycleVertices[i] << " \n"[i + 1 == (int)cycleVertices.size()];
    }
    for (int i = 0; i < (int)cycleEdges.size(); i++) {
        cout << cycleEdges[i] << " \n"[i + 1 == (int)cycleEdges.size()];
    }
    return 0;
}
