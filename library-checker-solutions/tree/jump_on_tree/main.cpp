#include <bits/stdc++.h>

using namespace std;

struct LCA {
    int n, log;
    vector<int> depth;
    vector<vector<int>> parent;

    LCA(const vector<vector<int>>& adj) {
        n = adj.size();
        log = bit_width((unsigned)n);
        depth.assign(n, 0);
        parent.assign(log, vector<int>(n));
        vector<int> order{0};
        parent[0][0] = 0;
        for (int i = 0; i < (int)order.size(); i++) {
            int u = order[i];
            for (int v : adj[u]) {
                if (v == parent[0][u]) {
                    continue;
                }
                parent[0][v] = u;
                depth[v] = depth[u] + 1;
                order.push_back(v);
            }
        }
        for (int k = 1; k < log; k++) {
            for (int i = 0; i < n; i++) {
                parent[k][i] = parent[k - 1][parent[k - 1][i]];
            }
        }
    }

    int jump(int u, int distance) const {
        for (int k = 0; distance; k++, distance >>= 1) {
            if (distance & 1) {
                u = parent[k][u];
            }
        }
        return u;
    }

    int lca(int u, int v) const {
        if (depth[u] < depth[v]) {
            swap(u, v);
        }
        u = jump(u, depth[u] - depth[v]);
        if (u == v) {
            return u;
        }
        for (int k = log - 1; k >= 0; k--) {
            if (parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }
        return parent[0][u];
    }

    int pathJump(int s, int t, int k) const {
        int p = lca(s, t);
        int up = depth[s] - depth[p];
        int down = depth[t] - depth[p];
        if (k > up + down) {
            return -1;
        }
        if (k <= up) {
            return jump(s, k);
        }
        return jump(t, up + down - k);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<vector<int>> adj(n);
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    LCA lca(adj);
    while (q--) {
        int s, t, k;
        cin >> s >> t >> k;
        cout << lca.pathJump(s, t, k) << '\n';
    }
    return 0;
}
