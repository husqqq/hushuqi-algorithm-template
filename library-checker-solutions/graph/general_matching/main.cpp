#include <bits/stdc++.h>

using namespace std;

struct Blossom {
    int n;
    vector<vector<int>> adjacent;
    vector<int> match, parent, base;
    vector<char> used, blossom;

    Blossom(int n) : n(n), adjacent(n), match(n, -1), parent(n), base(n), used(n), blossom(n) {}

    int lca(int a, int b) {
        vector<char> visited(n);
        while (true) {
            a = base[a];
            visited[a] = true;
            if (match[a] == -1) {
                break;
            }
            a = parent[match[a]];
        }
        while (!visited[base[b]]) {
            b = parent[match[b]];
        }
        return base[b];
    }

    void markPath(int vertex, int root, int child) {
        while (base[vertex] != root) {
            blossom[base[vertex]] = blossom[base[match[vertex]]] = true;
            parent[vertex] = child;
            child = match[vertex];
            vertex = parent[match[vertex]];
        }
    }

    bool augment(int root) {
        fill(used.begin(), used.end(), false);
        fill(parent.begin(), parent.end(), -1);
        iota(base.begin(), base.end(), 0);
        queue<int> queue;
        queue.push(root);
        used[root] = true;
        while (!queue.empty()) {
            int u = queue.front();
            queue.pop();
            for (int v : adjacent[u]) {
                if (base[u] == base[v] || match[u] == v) {
                    continue;
                }
                if (v == root || (match[v] != -1 && parent[match[v]] != -1)) {
                    int common = lca(u, v);
                    fill(blossom.begin(), blossom.end(), false);
                    markPath(u, common, v);
                    markPath(v, common, u);
                    for (int i = 0; i < n; i++) {
                        if (blossom[base[i]]) {
                            base[i] = common;
                            if (!used[i]) {
                                used[i] = true;
                                queue.push(i);
                            }
                        }
                    }
                } else if (parent[v] == -1) {
                    parent[v] = u;
                    if (match[v] == -1) {
                        while (v != -1) {
                            int previous = parent[v];
                            int next = previous == -1 ? -1 : match[previous];
                            match[v] = previous;
                            if (previous != -1) {
                                match[previous] = v;
                            }
                            v = next;
                        }
                        return true;
                    }
                    v = match[v];
                    used[v] = true;
                    queue.push(v);
                }
            }
        }
        return false;
    }

    void solve() {
        for (int i = 0; i < n; i++) {
            if (match[i] == -1) {
                augment(i);
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    Blossom blossom(n);
    while (m--) {
        int u, v;
        cin >> u >> v;
        blossom.adjacent[u].push_back(v);
        blossom.adjacent[v].push_back(u);
    }
    blossom.solve();
    vector<pair<int, int>> answer;
    for (int i = 0; i < n; i++) {
        if (i < blossom.match[i]) {
            answer.push_back({i, blossom.match[i]});
        }
    }
    cout << answer.size() << '\n';
    for (auto [u, v] : answer) {
        cout << u << ' ' << v << '\n';
    }
    return 0;
}
