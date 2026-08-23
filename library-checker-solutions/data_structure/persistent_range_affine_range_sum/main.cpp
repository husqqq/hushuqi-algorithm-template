#include <bits/stdc++.h>

using namespace std;

constexpr long long P = 998244353;

struct Node {
    int left = 0, right = 0;
    long long sum = 0, multiply = 1, add = 0;
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
    vector<Node> tree(1);
    tree.reserve(8 * n + 150 * q);
    auto clone = [&](int node) {
        tree.push_back(tree[node]);
        return int(tree.size()) - 1;
    };
    auto build = [&](auto&& self, int l, int r) -> int {
        int node = clone(0);
        if (r - l == 1) {
            tree[node].sum = initial[l];
            return node;
        }
        int mid = (l + r) / 2;
        tree[node].left = self(self, l, mid);
        tree[node].right = self(self, mid, r);
        tree[node].sum = (tree[tree[node].left].sum + tree[tree[node].right].sum) % P;
        return node;
    };
    auto apply = [&](int node, int l, int r, long long multiply, long long add) {
        tree[node].sum = (tree[node].sum * multiply + add * (r - l)) % P;
        tree[node].multiply = tree[node].multiply * multiply % P;
        tree[node].add = (tree[node].add * multiply + add) % P;
    };
    auto materialize = [&](int child, int l, int r, long long multiply, long long add) {
        int result = clone(child);
        apply(result, l, r, multiply, add);
        return result;
    };
    auto push = [&](int node, int l, int r) {
        if (r - l == 1 || (tree[node].multiply == 1 && tree[node].add == 0)) {
            return;
        }
        int mid = (l + r) / 2;
        tree[node].left = materialize(tree[node].left, l, mid, tree[node].multiply, tree[node].add);
        tree[node].right = materialize(tree[node].right, mid, r, tree[node].multiply, tree[node].add);
        tree[node].multiply = 1;
        tree[node].add = 0;
    };
    auto update = [&](auto&& self, int node, int l, int r, int ql, int qr, long long multiply, long long add) -> int {
        if (qr <= l || r <= ql) {
            return node;
        }
        int result = clone(node);
        if (ql <= l && r <= qr) {
            apply(result, l, r, multiply, add);
            return result;
        }
        push(result, l, r);
        int mid = (l + r) / 2;
        tree[result].left = self(self, tree[result].left, l, mid, ql, qr, multiply, add);
        tree[result].right = self(self, tree[result].right, mid, r, ql, qr, multiply, add);
        tree[result].sum = (tree[tree[result].left].sum + tree[tree[result].right].sum) % P;
        return result;
    };
    auto copyRange = [&](auto&& self, int target, int source, int l, int r, int ql, int qr) -> int {
        if (qr <= l || r <= ql) {
            return target;
        }
        if (ql <= l && r <= qr) {
            return source;
        }
        int result = clone(target);
        push(result, l, r);
        int mid = (l + r) / 2;
        int sourceLeft = tree[source].left;
        int sourceRight = tree[source].right;
        if (tree[source].multiply != 1 || tree[source].add != 0) {
            sourceLeft = materialize(sourceLeft, l, mid, tree[source].multiply, tree[source].add);
            sourceRight = materialize(sourceRight, mid, r, tree[source].multiply, tree[source].add);
        }
        tree[result].left = self(self, tree[result].left, sourceLeft, l, mid, ql, qr);
        tree[result].right = self(self, tree[result].right, sourceRight, mid, r, ql, qr);
        tree[result].sum = (tree[tree[result].left].sum + tree[tree[result].right].sum) % P;
        return result;
    };
    auto query = [&](auto&& self, int node, int l, int r, int ql, int qr, long long multiply,
                     long long add) -> long long {
        if (qr <= l || r <= ql) {
            return 0;
        }
        if (ql <= l && r <= qr) {
            return (tree[node].sum * multiply + add * (r - l)) % P;
        }
        long long nextMultiply = tree[node].multiply * multiply % P;
        long long nextAdd = (tree[node].add * multiply + add) % P;
        int mid = (l + r) / 2;
        return (self(self, tree[node].left, l, mid, ql, qr, nextMultiply, nextAdd) +
                self(self, tree[node].right, mid, r, ql, qr, nextMultiply, nextAdd)) %
               P;
    };

    vector<int> roots(q + 1);
    roots[0] = build(build, 0, n);
    for (int i = 0; i < q; i++) {
        int type, version, l, r;
        cin >> type >> version;
        int root = roots[version + 1];
        if (type == 0) {
            long long multiply, add;
            cin >> l >> r >> multiply >> add;
            root = update(update, root, 0, n, l, r, multiply, add);
        } else if (type == 1) {
            int sourceVersion;
            cin >> sourceVersion >> l >> r;
            root = copyRange(copyRange, root, roots[sourceVersion + 1], 0, n, l, r);
        } else {
            cin >> l >> r;
            cout << query(query, root, 0, n, l, r, 1, 0) << '\n';
        }
        roots[i + 1] = root;
    }
    return 0;
}
