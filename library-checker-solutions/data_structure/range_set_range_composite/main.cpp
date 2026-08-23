#include <bits/stdc++.h>

using namespace std;

constexpr long long P = 998244353;

struct Function {
    long long a = 1, b = 0;
};

Function compose(Function first, Function second) {
    return {second.a * first.a % P, (second.a * first.b + second.b) % P};
}

long long power(long long a, int b) {
    long long result = 1;
    while (b) {
        if (b & 1) {
            result = result * a % P;
        }
        a = a * a % P;
        b >>= 1;
    }
    return result;
}

Function repeat(Function function, int count) {
    Function result;
    while (count) {
        if (count & 1) {
            result = compose(result, function);
        }
        function = compose(function, function);
        count >>= 1;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Function> tree(4 * n), lazy(4 * n);
    vector<char> assigned(4 * n);
    auto build = [&](auto&& self, int node, int l, int r) -> void {
        if (r - l == 1) {
            cin >> tree[node].a >> tree[node].b;
            return;
        }
        int mid = (l + r) / 2;
        self(self, node * 2, l, mid);
        self(self, node * 2 + 1, mid, r);
        tree[node] = compose(tree[node * 2], tree[node * 2 + 1]);
    };
    build(build, 1, 0, n);
    auto apply = [&](int node, int l, int r, Function function) {
        tree[node] = repeat(function, r - l);
        lazy[node] = function;
        assigned[node] = true;
    };
    auto push = [&](int node, int l, int r) {
        if (!assigned[node] || r - l == 1) {
            return;
        }
        int mid = (l + r) / 2;
        apply(node * 2, l, mid, lazy[node]);
        apply(node * 2 + 1, mid, r, lazy[node]);
        assigned[node] = false;
    };
    auto update = [&](auto&& self, int node, int l, int r, int ql, int qr, Function function) -> void {
        if (qr <= l || r <= ql) {
            return;
        }
        if (ql <= l && r <= qr) {
            apply(node, l, r, function);
            return;
        }
        push(node, l, r);
        int mid = (l + r) / 2;
        self(self, node * 2, l, mid, ql, qr, function);
        self(self, node * 2 + 1, mid, r, ql, qr, function);
        tree[node] = compose(tree[node * 2], tree[node * 2 + 1]);
    };
    auto query = [&](auto&& self, int node, int l, int r, int ql, int qr) -> Function {
        if (qr <= l || r <= ql) {
            return {};
        }
        if (ql <= l && r <= qr) {
            return tree[node];
        }
        push(node, l, r);
        int mid = (l + r) / 2;
        return compose(self(self, node * 2, l, mid, ql, qr), self(self, node * 2 + 1, mid, r, ql, qr));
    };
    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 0) {
            Function function;
            cin >> function.a >> function.b;
            update(update, 1, 0, n, l, r, function);
        } else {
            long long x;
            cin >> x;
            auto function = query(query, 1, 0, n, l, r);
            cout << (function.a * x + function.b) % P << '\n';
        }
    }
    return 0;
}
