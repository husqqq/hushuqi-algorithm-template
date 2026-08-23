#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<set<int>> adjacent(n);
    while (m--) {
        int u, v;
        cin >> u >> v;
        adjacent[u].insert(v);
        adjacent[v].insert(u);
    }

    vector<int> weight(n), position(n), order;
    set<pair<int, int>> candidates;
    for (int i = 0; i < n; i++) {
        candidates.insert({0, i});
    }
    while (!candidates.empty()) {
        int u = candidates.rbegin()->second;
        candidates.erase(prev(candidates.end()));
        position[u] = order.size();
        order.push_back(u);
        weight[u] = -1;
        for (int v : adjacent[u]) {
            if (weight[v] == -1) {
                continue;
            }
            candidates.erase({weight[v], v});
            candidates.insert({++weight[v], v});
        }
    }

    for (int u : order) {
        int previous = -1;
        for (int v : adjacent[u]) {
            if (position[v] < position[u]) {
                previous = max(previous, position[v]);
            }
        }
        if (previous == -1) {
            continue;
        }
        int parent = order[previous];
        for (int start : adjacent[u]) {
            if (position[start] >= position[parent] || adjacent[parent].contains(start)) {
                continue;
            }
            vector<int> from(n, -1);
            queue<int> bfs;
            from[start] = start;
            bfs.push(start);
            while (!bfs.empty() && from[parent] == -1) {
                int x = bfs.front();
                bfs.pop();
                for (int v : adjacent[x]) {
                    if (v == u || (v != parent && adjacent[u].contains(v)) || from[v] != -1) {
                        continue;
                    }
                    from[v] = x;
                    bfs.push(v);
                }
            }
            vector<int> cycle;
            for (int x = parent; x != start; x = from[x]) {
                cycle.push_back(x);
            }
            cycle.push_back(start);
            cycle.push_back(u);
            cout << "NO\n" << cycle.size() << '\n';
            for (int i = 0; i < int(cycle.size()); i++) {
                cout << cycle[i] << " \n"[i + 1 == int(cycle.size())];
            }
            return 0;
        }
    }

    cout << "YES\n";
    reverse(order.begin(), order.end());
    for (int i = 0; i < n; i++) {
        cout << order[i] << " \n"[i + 1 == n];
    }
    return 0;
}
