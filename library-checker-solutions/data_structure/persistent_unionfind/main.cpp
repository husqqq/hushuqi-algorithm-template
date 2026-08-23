#include <bits/stdc++.h>

using namespace std;

struct Node {
    int left = 0, right = 0, value = -1;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Node> tree(1);
    tree.reserve(1 + 50 * q);
    auto clone = [&](int node) {
        tree.push_back(tree[node]);
        return int(tree.size()) - 1;
    };
    auto get = [&](auto&& self, int node, int l, int r, int position) -> int {
        if (!node) {
            return -1;
        }
        if (r - l == 1) {
            return tree[node].value;
        }
        int mid = (l + r) / 2;
        return position < mid ? self(self, tree[node].left, l, mid, position)
                              : self(self, tree[node].right, mid, r, position);
    };
    auto set = [&](auto&& self, int node, int l, int r, int position, int value) -> int {
        int result = clone(node);
        if (r - l == 1) {
            tree[result].value = value;
            return result;
        }
        int mid = (l + r) / 2;
        if (position < mid) {
            tree[result].left = self(self, tree[result].left, l, mid, position, value);
        } else {
            tree[result].right = self(self, tree[result].right, mid, r, position, value);
        }
        return result;
    };
    auto find = [&](int root, int u) {
        while (true) {
            int parent = get(get, root, 0, n, u);
            if (parent < 0) {
                return pair{u, parent};
            }
            u = parent;
        }
    };

    vector<int> roots(q + 1);
    for (int i = 0; i < q; i++) {
        int type, version, u, v;
        cin >> type >> version >> u >> v;
        int root = roots[version + 1];
        auto [leaderU, sizeU] = find(root, u);
        auto [leaderV, sizeV] = find(root, v);
        if (type == 0) {
            if (leaderU != leaderV) {
                if (sizeU > sizeV) {
                    swap(leaderU, leaderV);
                    swap(sizeU, sizeV);
                }
                root = set(set, root, 0, n, leaderU, sizeU + sizeV);
                root = set(set, root, 0, n, leaderV, leaderU);
            }
        } else {
            cout << (leaderU == leaderV) << '\n';
        }
        roots[i + 1] = root;
    }
    return 0;
}
