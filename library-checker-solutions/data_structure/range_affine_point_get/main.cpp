#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;

struct SegmentTree {
    int n;
    vector<int> mul, add;

    SegmentTree(int n) : n(n), mul(4 * n, 1), add(4 * n) {}

    void apply(int p, int b, int c) {
        mul[p] = 1LL * mul[p] * b % P;
        add[p] = (1LL * add[p] * b + c) % P;
    }

    void push(int p) {
        if (mul[p] == 1 && add[p] == 0) {
            return;
        }
        apply(2 * p, mul[p], add[p]);
        apply(2 * p + 1, mul[p], add[p]);
        mul[p] = 1;
        add[p] = 0;
    }

    void rangeApply(int ql, int qr, int b, int c, int p, int l, int r) {
        if (qr <= l || r <= ql) {
            return;
        }
        if (ql <= l && r <= qr) {
            apply(p, b, c);
            return;
        }
        push(p);
        int m = (l + r) / 2;
        rangeApply(ql, qr, b, c, 2 * p, l, m);
        rangeApply(ql, qr, b, c, 2 * p + 1, m, r);
    }

    void rangeApply(int l, int r, int b, int c) { rangeApply(l, r, b, c, 1, 0, n); }

    int pointGet(int x, int value, int p, int l, int r) const {
        if (r - l == 1) {
            return (1LL * value * mul[p] + add[p]) % P;
        }
        int m = (l + r) / 2;
        int answer;
        if (x < m) {
            answer = pointGet(x, value, 2 * p, l, m);
        } else {
            answer = pointGet(x, value, 2 * p + 1, m, r);
        }
        return (1LL * answer * mul[p] + add[p]) % P;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto& x : a) {
        cin >> x;
    }
    SegmentTree tree(n);
    while (q--) {
        int op;
        cin >> op;
        if (op == 0) {
            int l, r, b, c;
            cin >> l >> r >> b >> c;
            tree.rangeApply(l, r, b, c);
        } else {
            int i;
            cin >> i;
            cout << tree.pointGet(i, a[i], 1, 0, n) << '\n';
        }
    }
    return 0;
}
