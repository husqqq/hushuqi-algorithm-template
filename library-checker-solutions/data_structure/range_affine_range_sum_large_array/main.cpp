#include <bits/stdc++.h>

using namespace std;

constexpr long long P = 998244353;

struct Node {
    long long sum = 0, multiply = 1, add = 0;
    int left = 0, right = 0;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Node> tree(2);
    tree.reserve(2 + 64 * q);
    auto makeNode = [&]() {
        tree.push_back({});
        return int(tree.size()) - 1;
    };
    auto apply = [&](int node, int l, int r, long long multiply, long long add) {
        tree[node].sum = (tree[node].sum * multiply + add * (r - l)) % P;
        tree[node].multiply = tree[node].multiply * multiply % P;
        tree[node].add = (tree[node].add * multiply + add) % P;
    };
    auto push = [&](int node, int l, int r) {
        if (r - l == 1) {
            return;
        }
        if (!tree[node].left) {
            tree[node].left = makeNode();
        }
        if (!tree[node].right) {
            tree[node].right = makeNode();
        }
        int mid = l + (r - l) / 2;
        apply(tree[node].left, l, mid, tree[node].multiply, tree[node].add);
        apply(tree[node].right, mid, r, tree[node].multiply, tree[node].add);
        tree[node].multiply = 1;
        tree[node].add = 0;
    };
    auto update = [&](auto&& self, int node, int l, int r, int ql, int qr, long long multiply, long long add) -> void {
        if (qr <= l || r <= ql) {
            return;
        }
        if (ql <= l && r <= qr) {
            apply(node, l, r, multiply, add);
            return;
        }
        push(node, l, r);
        int mid = l + (r - l) / 2;
        self(self, tree[node].left, l, mid, ql, qr, multiply, add);
        self(self, tree[node].right, mid, r, ql, qr, multiply, add);
        tree[node].sum = (tree[tree[node].left].sum + tree[tree[node].right].sum) % P;
    };
    auto query = [&](auto&& self, int node, int l, int r, int ql, int qr) -> long long {
        if (qr <= l || r <= ql) {
            return 0;
        }
        if (ql <= l && r <= qr) {
            return tree[node].sum;
        }
        push(node, l, r);
        int mid = l + (r - l) / 2;
        return (self(self, tree[node].left, l, mid, ql, qr) + self(self, tree[node].right, mid, r, ql, qr)) % P;
    };
    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 0) {
            long long multiply, add;
            cin >> multiply >> add;
            update(update, 1, 0, n, l, r, multiply, add);
        } else {
            cout << query(query, 1, 0, n, l, r) << '\n';
        }
    }
    return 0;
}
