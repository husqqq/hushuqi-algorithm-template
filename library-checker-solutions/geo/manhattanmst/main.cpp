#include <bits/stdc++.h>

using namespace std;

struct Point {
    long long x, y;
};

struct Edge {
    long long weight;
    int u, v;
};

struct DSU {
    vector<int> parent;
    DSU(int n) : parent(n, -1) {}
    int find(int u) { return parent[u] < 0 ? u : parent[u] = find(parent[u]); }
    bool unite(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) return false;
        if (parent[u] > parent[v]) swap(u, v);
        parent[u] += parent[v];
        parent[v] = u;
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Point> original(n), transformed(n);
    for (auto& point : original) cin >> point.x >> point.y;
    transformed = original;
    vector<Edge> edges;
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    for (int direction = 0; direction < 4; direction++) {
        sort(order.begin(), order.end(),
             [&](int a, int b) { return transformed[a].x + transformed[a].y < transformed[b].x + transformed[b].y; });
        map<long long, int> sweep;
        for (int i : order) {
            for (auto it = sweep.lower_bound(-transformed[i].y); it != sweep.end();) {
                int j = it->second;
                if (transformed[i].x - transformed[j].x < transformed[i].y - transformed[j].y) break;
                long long weight = abs(original[i].x - original[j].x) + abs(original[i].y - original[j].y);
                edges.push_back({weight, i, j});
                it = sweep.erase(it);
            }
            sweep[-transformed[i].y] = i;
        }
        for (auto& point : transformed) {
            if (direction & 1) point.x = -point.x;
            else swap(point.x, point.y);
        }
    }
    sort(edges.begin(), edges.end(), [&](const Edge& a, const Edge& b) { return a.weight < b.weight; });
    DSU dsu(n);
    long long total = 0;
    vector<pair<int, int>> answer;
    for (auto edge : edges) {
        if (dsu.unite(edge.u, edge.v)) {
            total += edge.weight;
            answer.push_back({edge.u, edge.v});
        }
    }
    cout << total << '\n';
    for (auto [u, v] : answer) cout << u << ' ' << v << '\n';
    return 0;
}
