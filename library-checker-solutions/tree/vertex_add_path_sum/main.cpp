#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

struct Fenwick {
    vector<i64> a;

    Fenwick(int n) : a(n + 1) {}

    void add(int x, i64 value) {
        for (x++; x < (int)a.size(); x += x & -x) {
            a[x] += value;
        }
    }

    i64 sum(int x) const {
        i64 answer = 0;
        for (; x > 0; x -= x & -x) {
            answer += a[x];
        }
        return answer;
    }

    i64 rangeSum(int l, int r) const { return sum(r) - sum(l); }
};

struct HLD {
    int n, timer = 0;
    vector<int> parent, depth, size, heavy, head, position;

    HLD(const vector<vector<int>>& adj) {
        n = adj.size();
        parent.assign(n, -1);
        depth.assign(n, 0);
        size.assign(n, 1);
        heavy.assign(n, -1);
        head.resize(n);
        position.resize(n);

        vector<int> order{0};
        for (int i = 0; i < (int)order.size(); i++) {
            int u = order[i];
            for (int v : adj[u]) {
                if (v == parent[u]) {
                    continue;
                }
                parent[v] = u;
                depth[v] = depth[u] + 1;
                order.push_back(v);
            }
        }
        for (int i = n - 1; i > 0; i--) {
            int u = order[i];
            size[parent[u]] += size[u];
            if (heavy[parent[u]] == -1 || size[u] > size[heavy[parent[u]]]) {
                heavy[parent[u]] = u;
            }
        }
        for (int u : order) {
            if (u != 0 && heavy[parent[u]] == u) {
                continue;
            }
            for (int v = u; v != -1; v = heavy[v]) {
                head[v] = u;
                position[v] = timer++;
            }
        }
    }

    template <class F> void path(int u, int v, F&& f) const {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) {
                swap(u, v);
            }
            f(position[head[u]], position[u] + 1);
            u = parent[head[u]];
        }
        if (depth[u] > depth[v]) {
            swap(u, v);
        }
        f(position[u], position[v] + 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<i64> value(n);
    for (auto& x : value) {
        cin >> x;
    }
    vector<vector<int>> adj(n);
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    HLD hld(adj);
    Fenwick fenwick(n);
    for (int i = 0; i < n; i++) {
        fenwick.add(hld.position[i], value[i]);
    }
    while (q--) {
        int op, u;
        cin >> op >> u;
        if (op == 0) {
            i64 x;
            cin >> x;
            fenwick.add(hld.position[u], x);
        } else {
            int v;
            cin >> v;
            i64 answer = 0;
            hld.path(u, v, [&](int l, int r) { answer += fenwick.rangeSum(l, r); });
            cout << answer << '\n';
        }
    }
    return 0;
}
