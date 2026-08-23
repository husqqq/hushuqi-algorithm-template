#include <bits/stdc++.h>

using namespace std;

constexpr long long P = 998244353;

struct Edge {
    int to;
    long long multiply, add;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> value(n);
    for (auto& x : value) {
        cin >> x;
    }
    vector<vector<Edge>> adjacent(n);
    for (int i = 1; i < n; i++) {
        int u, v;
        long long multiply, add;
        cin >> u >> v >> multiply >> add;
        adjacent[u].push_back({v, multiply, add});
        adjacent[v].push_back({u, multiply, add});
    }
    vector<int> parent(n, -1), subtree(n, 1), order{0};
    vector<long long> parentMultiply(n), parentAdd(n), down = value;
    for (int i = 0; i < int(order.size()); i++) {
        int u = order[i];
        for (auto edge : adjacent[u]) {
            if (edge.to != parent[u]) {
                parent[edge.to] = u;
                parentMultiply[edge.to] = edge.multiply;
                parentAdd[edge.to] = edge.add;
                order.push_back(edge.to);
            }
        }
    }
    for (int i = n - 1; i > 0; i--) {
        int u = order[i], p = parent[u];
        long long contribution = (parentMultiply[u] * down[u] + parentAdd[u] * subtree[u]) % P;
        down[p] += contribution;
        down[p] %= P;
        subtree[p] += subtree[u];
    }
    vector<long long> answer(n);
    answer[0] = down[0];
    for (int u : order) {
        for (auto edge : adjacent[u]) {
            int v = edge.to;
            if (parent[v] != u) {
                continue;
            }
            long long childContribution = (edge.multiply * down[v] + edge.add * subtree[v]) % P;
            long long outside = (answer[u] - childContribution + P) % P;
            long long outsideContribution = (edge.multiply * outside + edge.add * (n - subtree[v])) % P;
            answer[v] = (down[v] + outsideContribution) % P;
        }
    }
    for (int i = 0; i < n; i++) {
        cout << answer[i] << " \n"[i + 1 == n];
    }
    return 0;
}
