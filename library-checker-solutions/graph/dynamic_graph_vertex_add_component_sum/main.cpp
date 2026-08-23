#include <bits/stdc++.h>

using namespace std;

struct Operation {
    int type, u = 0, v = 0;
    long long value = 0;
};

struct Event {
    int type, u, v;
    long long value;
};

struct RollbackDSU {
    vector<int> parent;
    vector<long long> sum;
    struct Change {
        int type, a, b, sizeA, sizeB;
        long long sumA, sumB;
    };
    vector<Change> history;

    RollbackDSU(vector<long long> value) : parent(value.size(), -1), sum(move(value)) {}

    int find(int u) const {
        while (parent[u] >= 0) {
            u = parent[u];
        }
        return u;
    }

    void unite(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) {
            history.push_back({0});
            return;
        }
        if (parent[u] > parent[v]) {
            swap(u, v);
        }
        history.push_back({1, u, v, parent[u], parent[v], sum[u], sum[v]});
        parent[u] += parent[v];
        parent[v] = u;
        sum[u] += sum[v];
    }

    void add(int vertex, long long value) {
        int root = find(vertex);
        history.push_back({2, root, 0, 0, 0, sum[root], 0});
        sum[root] += value;
    }

    void rollback(int checkpoint) {
        while (int(history.size()) > checkpoint) {
            auto change = history.back();
            history.pop_back();
            if (change.type == 1) {
                parent[change.a] = change.sizeA;
                parent[change.b] = change.sizeB;
                sum[change.a] = change.sumA;
                sum[change.b] = change.sumB;
            } else if (change.type == 2) {
                sum[change.a] = change.sumA;
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> initial(n);
    for (auto& value : initial) {
        cin >> value;
    }
    vector<Operation> operations(q);
    vector<vector<Event>> segmentTree(4 * q);
    auto addInterval = [&](auto&& self, int node, int l, int r, int ql, int qr, Event event) -> void {
        if (qr <= l || r <= ql) {
            return;
        }
        if (ql <= l && r <= qr) {
            segmentTree[node].push_back(event);
            return;
        }
        int mid = (l + r) / 2;
        self(self, node * 2, l, mid, ql, qr, event);
        self(self, node * 2 + 1, mid, r, ql, qr, event);
    };
    map<pair<int, int>, int> addedAt;
    for (int time = 0; time < q; time++) {
        auto& operation = operations[time];
        cin >> operation.type >> operation.u;
        if (operation.type <= 1) {
            cin >> operation.v;
            if (operation.u > operation.v) {
                swap(operation.u, operation.v);
            }
            auto edge = pair{operation.u, operation.v};
            if (operation.type == 0) {
                addedAt[edge] = time;
            } else {
                addInterval(addInterval, 1, 0, q, addedAt[edge], time, {0, operation.u, operation.v, 0});
                addedAt.erase(edge);
            }
        } else if (operation.type == 2) {
            cin >> operation.value;
            addInterval(addInterval, 1, 0, q, time, q, {1, operation.u, 0, operation.value});
        }
    }
    for (auto [edge, start] : addedAt) {
        addInterval(addInterval, 1, 0, q, start, q, {0, edge.first, edge.second, 0});
    }

    RollbackDSU dsu(move(initial));
    auto solve = [&](auto&& self, int node, int l, int r) -> void {
        int checkpoint = dsu.history.size();
        for (auto event : segmentTree[node]) {
            if (event.type == 0) {
                dsu.unite(event.u, event.v);
            } else {
                dsu.add(event.u, event.value);
            }
        }
        if (r - l == 1) {
            if (operations[l].type == 3) {
                int root = dsu.find(operations[l].u);
                cout << dsu.sum[root] << '\n';
            }
        } else {
            int mid = (l + r) / 2;
            self(self, node * 2, l, mid);
            self(self, node * 2 + 1, mid, r);
        }
        dsu.rollback(checkpoint);
    };
    solve(solve, 1, 0, q);
    return 0;
}
