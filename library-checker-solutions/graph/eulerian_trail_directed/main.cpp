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
        vector<int> in(n), out(n);
        for (int id = 0; id < m; id++) {
            int u, v;
            cin >> u >> v;
            adj[u].emplace_back(v, id);
            out[u]++;
            in[v]++;
        }

        int start = m ? -1 : 0;
        int plus = 0, minus = 0;
        bool valid = true;
        for (int i = 0; i < n; i++) {
            if (out[i] - in[i] == 1) {
                start = i;
                plus++;
            } else if (in[i] - out[i] == 1) {
                minus++;
            } else if (in[i] != out[i]) {
                valid = false;
            } else if (start == -1 && out[i]) {
                start = i;
            }
        }
        valid &= (plus == 0 && minus == 0) || (plus == 1 && minus == 1);

        vector<int> vertexPath, edgePath, index(n);
        if (valid) {
            vector<int> vertices{start}, incoming{-1};
            while (!vertices.empty()) {
                int u = vertices.back();
                if (index[u] < (int)adj[u].size()) {
                    auto [v, edge] = adj[u][index[u]++];
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
