#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

struct DSU {
    vector<int> parent, size;

    DSU(int n) : parent(n), size(n, 1) { iota(parent.begin(), parent.end(), 0); }

    int find(int x) { return x == parent[x] ? x : parent[x] = find(parent[x]); }

    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            return false;
        }
        if (size[x] < size[y]) {
            swap(x, y);
        }
        parent[y] = x;
        size[x] += size[y];
        return true;
    }
};

struct Edge {
    int u, v, weight, id;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].weight;
        edges[i].id = i;
    }
    sort(edges.begin(), edges.end(),
         [&](const Edge& a, const Edge& b) { return pair(a.weight, a.id) < pair(b.weight, b.id); });

    DSU dsu(n);
    i64 answer = 0;
    vector<int> used;
    for (auto [u, v, weight, id] : edges) {
        if (dsu.merge(u, v)) {
            answer += weight;
            used.push_back(id);
        }
    }

    cout << answer << '\n';
    for (int i = 0; i < (int)used.size(); i++) {
        cout << used[i] << " \n"[i + 1 == (int)used.size()];
    }
    if (used.empty()) {
        cout << '\n';
    }
    return 0;
}
