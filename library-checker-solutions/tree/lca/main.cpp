#include <bits/stdc++.h>

using namespace std;

struct LCA {
    int n, log;
    vector<int> depth;
    vector<vector<int>> parent;

    LCA(const vector<int>& p = {}) {
        if (!p.empty()) {
            init(p);
        }
    }

    void init(const vector<int>& p) {
        n = p.size();
        log = bit_width((unsigned)n);
        depth.assign(n, 0);
        parent.assign(log, vector<int>(n, 0));
        parent[0] = p;

        for (int i = 1; i < n; i++) {
            depth[i] = depth[parent[0][i]] + 1;
        }
        for (int k = 1; k < log; k++) {
            for (int i = 0; i < n; i++) {
                parent[k][i] = parent[k - 1][parent[k - 1][i]];
            }
        }
    }

    int jump(int u, int d) const {
        for (int k = 0; d; k++, d >>= 1) {
            if (d & 1) {
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
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> parent(n);
    parent[0] = 0;
    for (int i = 1; i < n; i++) {
        cin >> parent[i];
    }

    LCA lca(parent);
    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << lca.lca(u, v) << '\n';
    }
    return 0;
}
