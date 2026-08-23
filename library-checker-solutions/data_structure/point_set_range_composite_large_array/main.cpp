#include <bits/stdc++.h>

using namespace std;

constexpr long long P = 998244353;

struct Function {
    long long a = 1, b = 0;
};

Function compose(Function first, Function second) {
    return {second.a * first.a % P, (second.a * first.b + second.b) % P};
}

struct Node {
    Function value;
    int left = 0, right = 0;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Node> tree(1);
    tree.reserve(1 + 32 * q);
    auto makeNode = [&]() {
        tree.push_back({});
        return int(tree.size()) - 1;
    };
    int root = makeNode();
    auto update = [&](auto&& self, int node, int l, int r, int position, Function function) -> void {
        if (r - l == 1) {
            tree[node].value = function;
            return;
        }
        int mid = l + (r - l) / 2;
        if (position < mid) {
            if (!tree[node].left) {
                tree[node].left = makeNode();
            }
            self(self, tree[node].left, l, mid, position, function);
        } else {
            if (!tree[node].right) {
                tree[node].right = makeNode();
            }
            self(self, tree[node].right, mid, r, position, function);
        }
        Function left = tree[node].left ? tree[tree[node].left].value : Function{};
        Function right = tree[node].right ? tree[tree[node].right].value : Function{};
        tree[node].value = compose(left, right);
    };
    auto query = [&](auto&& self, int node, int l, int r, int ql, int qr) -> Function {
        if (!node || qr <= l || r <= ql) {
            return {};
        }
        if (ql <= l && r <= qr) {
            return tree[node].value;
        }
        int mid = l + (r - l) / 2;
        return compose(self(self, tree[node].left, l, mid, ql, qr), self(self, tree[node].right, mid, r, ql, qr));
    };
    while (q--) {
        int type;
        cin >> type;
        if (type == 0) {
            int position;
            Function function;
            cin >> position >> function.a >> function.b;
            update(update, root, 0, n, position, function);
        } else {
            int l, r;
            long long x;
            cin >> l >> r >> x;
            auto function = query(query, root, 0, n, l, r);
            cout << (function.a * x + function.b) % P << '\n';
        }
    }
    return 0;
}
