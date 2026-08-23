#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;

struct PotentialDSU {
    vector<int> parent, size, potential;

    PotentialDSU(int n) : parent(n), size(n, 1), potential(n) { iota(parent.begin(), parent.end(), 0); }

    int find(int x) {
        if (x == parent[x]) {
            return x;
        }
        int old = parent[x];
        parent[x] = find(parent[x]);
        potential[x] += potential[old];
        if (potential[x] >= P) {
            potential[x] -= P;
        }
        return parent[x];
    }

    int weight(int x) {
        find(x);
        return potential[x];
    }

    bool merge(int u, int v, int x) {
        int rootU = find(u);
        int rootV = find(v);
        int difference = (x - potential[u] + potential[v]) % P;
        if (difference < 0) {
            difference += P;
        }
        if (rootU == rootV) {
            return difference == 0;
        }
        if (size[rootU] < size[rootV]) {
            parent[rootU] = rootV;
            potential[rootU] = difference;
            size[rootV] += size[rootU];
        } else {
            parent[rootV] = rootU;
            potential[rootV] = difference == 0 ? 0 : P - difference;
            size[rootU] += size[rootV];
        }
        return true;
    }

    optional<int> difference(int u, int v) {
        if (find(u) != find(v)) {
            return nullopt;
        }
        int answer = potential[u] - potential[v];
        if (answer < 0) {
            answer += P;
        }
        return answer;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    PotentialDSU dsu(n);
    while (q--) {
        int op, u, v;
        cin >> op >> u >> v;
        if (op == 0) {
            int x;
            cin >> x;
            cout << dsu.merge(u, v, x) << '\n';
        } else {
            auto answer = dsu.difference(u, v);
            cout << (answer ? *answer : -1) << '\n';
        }
    }
    return 0;
}
