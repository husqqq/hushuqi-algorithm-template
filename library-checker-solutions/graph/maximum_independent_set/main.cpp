#include <bits/stdc++.h>

using namespace std;

struct MaximumClique {
    int n;
    vector<unsigned long long> adjacent;
    vector<int> best, current;

    void search(vector<int> candidates) {
        vector<int> order, color;
        unsigned long long remaining = 0;
        for (int v : candidates) {
            remaining |= 1ULL << v;
        }
        int colors = 0;
        while (remaining) {
            colors++;
            unsigned long long available = remaining;
            while (available) {
                int v = countr_zero(available);
                order.push_back(v);
                color.push_back(colors);
                remaining ^= 1ULL << v;
                available &= ~(1ULL << v);
                available &= ~adjacent[v];
                available &= remaining;
            }
        }
        for (int i = int(order.size()) - 1; i >= 0; i--) {
            if (current.size() + color[i] <= best.size()) {
                return;
            }
            int v = order[i];
            current.push_back(v);
            vector<int> next;
            for (int j = 0; j < i; j++) {
                if (adjacent[v] >> order[j] & 1) {
                    next.push_back(order[j]);
                }
            }
            if (next.empty()) {
                if (current.size() > best.size()) {
                    best = current;
                }
            } else {
                search(move(next));
            }
            current.pop_back();
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    MaximumClique solver{n, vector<unsigned long long>(n)};
    for (int i = 0; i < n; i++) {
        solver.adjacent[i] = ((1ULL << n) - 1) ^ (1ULL << i);
    }
    while (m--) {
        int u, v;
        cin >> u >> v;
        solver.adjacent[u] &= ~(1ULL << v);
        solver.adjacent[v] &= ~(1ULL << u);
    }
    vector<int> vertices(n);
    iota(vertices.begin(), vertices.end(), 0);
    solver.search(move(vertices));
    cout << solver.best.size() << '\n';
    for (int i = 0; i < int(solver.best.size()); i++) {
        cout << solver.best[i] << " \n"[i + 1 == int(solver.best.size())];
    }
    return 0;
}
