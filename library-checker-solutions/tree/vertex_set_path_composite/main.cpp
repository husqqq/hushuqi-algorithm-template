#include <bits/stdc++.h>

using namespace std;

constexpr long long P = 998244353;

struct Function {
    long long a = 1, b = 0;
};

Function compose(Function first, Function second) {
    return {second.a * first.a % P, (second.a * first.b + second.b) % P};
}

struct SegmentTree {
    int size = 1;
    vector<Function> forward, backward;

    SegmentTree(int n) {
        while (size < n) {
            size *= 2;
        }
        forward.resize(2 * size);
        backward.resize(2 * size);
    }

    void set(int position, Function function) {
        int node = position + size;
        forward[node] = backward[node] = function;
        while (node /= 2) {
            forward[node] = compose(forward[node * 2], forward[node * 2 + 1]);
            backward[node] = compose(backward[node * 2 + 1], backward[node * 2]);
        }
    }

    pair<Function, Function> query(int l, int r) const {
        Function leftForward, rightForward, leftBackward, rightBackward;
        for (l += size, r += size; l < r; l /= 2, r /= 2) {
            if (l & 1) {
                leftForward = compose(leftForward, forward[l]);
                leftBackward = compose(backward[l], leftBackward);
                l++;
            }
            if (r & 1) {
                --r;
                rightForward = compose(forward[r], rightForward);
                rightBackward = compose(rightBackward, backward[r]);
            }
        }
        return {compose(leftForward, rightForward), compose(rightBackward, leftBackward)};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Function> function(n);
    for (auto& [a, b] : function) {
        cin >> a >> b;
    }
    vector<vector<int>> adjacent(n);
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adjacent[u].push_back(v);
        adjacent[v].push_back(u);
    }
    vector<int> parent(n, -1), depth(n), subtree(n, 1), heavy(n, -1), order{0};
    for (int i = 0; i < int(order.size()); i++) {
        int u = order[i];
        for (int v : adjacent[u]) {
            if (v != parent[u]) {
                parent[v] = u;
                depth[v] = depth[u] + 1;
                order.push_back(v);
            }
        }
    }
    for (int i = n - 1; i > 0; i--) {
        int u = order[i], p = parent[u];
        subtree[p] += subtree[u];
        if (heavy[p] == -1 || subtree[u] > subtree[heavy[p]]) {
            heavy[p] = u;
        }
    }
    vector<int> head(n), position(n);
    int timer = 0;
    for (int u : order) {
        if (u && heavy[parent[u]] == u) {
            continue;
        }
        for (int v = u; v != -1; v = heavy[v]) {
            head[v] = u;
            position[v] = timer++;
        }
    }
    SegmentTree segmentTree(n);
    for (int i = 0; i < n; i++) {
        segmentTree.set(position[i], function[i]);
    }

    auto path = [&](int u, int v) {
        Function upward, downward;
        while (head[u] != head[v]) {
            if (depth[head[u]] >= depth[head[v]]) {
                auto [forward, backward] = segmentTree.query(position[head[u]], position[u] + 1);
                upward = compose(upward, backward);
                u = parent[head[u]];
            } else {
                auto [forward, backward] = segmentTree.query(position[head[v]], position[v] + 1);
                downward = compose(forward, downward);
                v = parent[head[v]];
            }
        }
        if (depth[u] >= depth[v]) {
            auto [forward, backward] = segmentTree.query(position[v], position[u] + 1);
            upward = compose(upward, backward);
        } else {
            auto [forward, backward] = segmentTree.query(position[u], position[v] + 1);
            downward = compose(forward, downward);
        }
        return compose(upward, downward);
    };

    while (q--) {
        int type;
        cin >> type;
        if (type == 0) {
            int vertex;
            Function value;
            cin >> vertex >> value.a >> value.b;
            segmentTree.set(position[vertex], value);
        } else {
            int u, v;
            long long x;
            cin >> u >> v >> x;
            auto value = path(u, v);
            cout << (value.a * x + value.b) % P << '\n';
        }
    }
    return 0;
}
