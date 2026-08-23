#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        int n, m;
        cin >> n >> m;
        vector<vector<pair<int, int>>> adj(n);
        vector<int> degree(n);
        for (int id = 0; id < m; id++) {
            int u, v;
            cin >> u >> v;
            adj[u].emplace_back(v, id);
            adj[v].emplace_back(u, id);
            degree[u]++;
            degree[v]++;
        }

        vector<int> odd;
        int start = m ? -1 : 0;
        for (int i = 0; i < n; i++) {
            if (degree[i] % 2) {
                odd.push_back(i);
            }
            if (start == -1 && degree[i]) {
                start = i;
            }
        }
        bool valid = odd.empty() || odd.size() == 2;
        if (odd.size() == 2) {
            start = odd[0];
        }

        vector<int> vertexPath, edgePath, index(n);
        vector<bool> used(m);
        if (valid) {
            vector<int> vertices{start}, incoming{-1};
            while (!vertices.empty()) {
                int u = vertices.back();
                while (index[u] < (int)adj[u].size() && used[adj[u][index[u]].second]) {
                    index[u]++;
                }
                if (index[u] < (int)adj[u].size()) {
                    auto [v, edge] = adj[u][index[u]++];
                    used[edge] = true;
                    vertices.push_back(v);
                    incoming.push_back(edge);
                } else {
                    vertexPath.push_back(u);
                    if (incoming.back() != -1) {
                        edgePath.push_back(incoming.back());
                    }
                    vertices.pop_back();
                    incoming.pop_back();
                }
            }
            reverse(vertexPath.begin(), vertexPath.end());
            reverse(edgePath.begin(), edgePath.end());
            valid &= (int)edgePath.size() == m;
        }

        if (!valid) {
            cout << "No\n";
            continue;
        }
        cout << "Yes\n";
        for (int i = 0; i < (int)vertexPath.size(); i++) {
            cout << vertexPath[i] << " \n"[i + 1 == (int)vertexPath.size()];
        }
        for (int i = 0; i < (int)edgePath.size(); i++) {
            cout << edgePath[i] << " \n"[i + 1 == (int)edgePath.size()];
        }
        if (edgePath.empty()) {
            cout << '\n';
        }
    }
    return 0;
}
