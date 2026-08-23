#include <bits/stdc++.h>

using namespace std;

constexpr long long INF = 4'000'000'000'000'000'000LL;

struct Node {
    long long sum = 0, maximum = -INF, secondMaximum = -INF;
    long long minimum = INF, secondMinimum = INF, add = 0;
    int maximumCount = 0, minimumCount = 0, length = 0;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Node> tree(4 * n);
    auto mergeNode = [&](const Node& left, const Node& right) {
        Node result;
        result.sum = left.sum + right.sum;
        result.length = left.length + right.length;
        result.maximum = max(left.maximum, right.maximum);
        result.maximumCount = (left.maximum == result.maximum ? left.maximumCount : 0) +
                              (right.maximum == result.maximum ? right.maximumCount : 0);
        result.secondMaximum = max(left.maximum == result.maximum ? left.secondMaximum : left.maximum,
                                   right.maximum == result.maximum ? right.secondMaximum : right.maximum);
        result.minimum = min(left.minimum, right.minimum);
        result.minimumCount = (left.minimum == result.minimum ? left.minimumCount : 0) +
                              (right.minimum == result.minimum ? right.minimumCount : 0);
        result.secondMinimum = min(left.minimum == result.minimum ? left.secondMinimum : left.minimum,
                                   right.minimum == result.minimum ? right.secondMinimum : right.minimum);
        return result;
    };
    auto build = [&](auto&& self, int node, int l, int r) -> void {
        if (r - l == 1) {
            long long value;
            cin >> value;
            tree[node].sum = tree[node].maximum = tree[node].minimum = value;
            tree[node].maximumCount = tree[node].minimumCount = tree[node].length = 1;
            return;
        }
        int mid = (l + r) / 2;
        self(self, node * 2, l, mid);
        self(self, node * 2 + 1, mid, r);
        tree[node] = mergeNode(tree[node * 2], tree[node * 2 + 1]);
    };
    build(build, 1, 0, n);
    auto applyAdd = [&](int node, long long value) {
        tree[node].sum += value * tree[node].length;
        tree[node].maximum += value;
        if (tree[node].secondMaximum != -INF) tree[node].secondMaximum += value;
        tree[node].minimum += value;
        if (tree[node].secondMinimum != INF) tree[node].secondMinimum += value;
        tree[node].add += value;
    };
    auto applyChmin = [&](int node, long long value) {
        if (tree[node].maximum <= value) return;
        tree[node].sum += (value - tree[node].maximum) * tree[node].maximumCount;
        if (tree[node].minimum == tree[node].maximum) tree[node].minimum = value;
        else if (tree[node].secondMinimum == tree[node].maximum) tree[node].secondMinimum = value;
        tree[node].maximum = value;
    };
    auto applyChmax = [&](int node, long long value) {
        if (tree[node].minimum >= value) return;
        tree[node].sum += (value - tree[node].minimum) * tree[node].minimumCount;
        if (tree[node].maximum == tree[node].minimum) tree[node].maximum = value;
        else if (tree[node].secondMaximum == tree[node].minimum) tree[node].secondMaximum = value;
        tree[node].minimum = value;
    };
    auto push = [&](int node) {
        for (int child : {node * 2, node * 2 + 1}) {
            if (tree[node].add) applyAdd(child, tree[node].add);
            applyChmin(child, tree[node].maximum);
            applyChmax(child, tree[node].minimum);
        }
        tree[node].add = 0;
    };
    auto rangeAdd = [&](auto&& self, int node, int l, int r, int ql, int qr, long long value) -> void {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            applyAdd(node, value);
            return;
        }
        push(node);
        int mid = (l + r) / 2;
        self(self, node * 2, l, mid, ql, qr, value);
        self(self, node * 2 + 1, mid, r, ql, qr, value);
        tree[node] = mergeNode(tree[node * 2], tree[node * 2 + 1]);
    };
    auto rangeChmin = [&](auto&& self, int node, int l, int r, int ql, int qr, long long value) -> void {
        if (qr <= l || r <= ql || tree[node].maximum <= value) return;
        if (ql <= l && r <= qr && tree[node].secondMaximum < value) {
            applyChmin(node, value);
            return;
        }
        push(node);
        int mid = (l + r) / 2;
        self(self, node * 2, l, mid, ql, qr, value);
        self(self, node * 2 + 1, mid, r, ql, qr, value);
        tree[node] = mergeNode(tree[node * 2], tree[node * 2 + 1]);
    };
    auto rangeChmax = [&](auto&& self, int node, int l, int r, int ql, int qr, long long value) -> void {
        if (qr <= l || r <= ql || tree[node].minimum >= value) return;
        if (ql <= l && r <= qr && tree[node].secondMinimum > value) {
            applyChmax(node, value);
            return;
        }
        push(node);
        int mid = (l + r) / 2;
        self(self, node * 2, l, mid, ql, qr, value);
        self(self, node * 2 + 1, mid, r, ql, qr, value);
        tree[node] = mergeNode(tree[node * 2], tree[node * 2 + 1]);
    };
    auto rangeSum = [&](auto&& self, int node, int l, int r, int ql, int qr) -> long long {
        if (qr <= l || r <= ql) return 0;
        if (ql <= l && r <= qr) return tree[node].sum;
        push(node);
        int mid = (l + r) / 2;
        return self(self, node * 2, l, mid, ql, qr) + self(self, node * 2 + 1, mid, r, ql, qr);
    };
    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 3) cout << rangeSum(rangeSum, 1, 0, n, l, r) << '\n';
        else {
            long long value;
            cin >> value;
            if (type == 0) rangeChmin(rangeChmin, 1, 0, n, l, r, value);
            else if (type == 1) rangeChmax(rangeChmax, 1, 0, n, l, r, value);
            else rangeAdd(rangeAdd, 1, 0, n, l, r, value);
        }
    }
    return 0;
}
