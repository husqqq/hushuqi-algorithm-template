#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;

int add(int a, int b) {
    a += b;
    return a >= P ? a - P : a;
}

int mul(int a, int b) {
    return 1LL * a * b % P;
}

struct LazySegmentTree {
    int n;
    vector<int> sum, tagMul, tagAdd;

    LazySegmentTree(const vector<int>& a = {}) {
        if (!a.empty()) {
            init(a);
        }
    }

    void init(const vector<int>& a) {
        n = a.size();
        sum.assign(4 * n, 0);
        tagMul.assign(4 * n, 1);
        tagAdd.assign(4 * n, 0);
        build(1, 0, n, a);
    }

    void build(int p, int l, int r, const vector<int>& a) {
        if (r - l == 1) {
            sum[p] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(2 * p, l, m, a);
        build(2 * p + 1, m, r, a);
        pull(p);
    }

    void pull(int p) { sum[p] = add(sum[2 * p], sum[2 * p + 1]); }

    void apply(int p, int l, int r, int b, int c) {
        sum[p] = add(mul(sum[p], b), mul(r - l, c));
        tagMul[p] = mul(tagMul[p], b);
        tagAdd[p] = add(mul(tagAdd[p], b), c);
    }

    void push(int p, int l, int r) {
        if (tagMul[p] == 1 && tagAdd[p] == 0) {
            return;
        }
        int m = (l + r) / 2;
        apply(2 * p, l, m, tagMul[p], tagAdd[p]);
        apply(2 * p + 1, m, r, tagMul[p], tagAdd[p]);
        tagMul[p] = 1;
        tagAdd[p] = 0;
    }

    void rangeApply(int ql, int qr, int b, int c, int p, int l, int r) {
        if (qr <= l || r <= ql) {
            return;
        }
        if (ql <= l && r <= qr) {
            apply(p, l, r, b, c);
            return;
        }
        push(p, l, r);
        int m = (l + r) / 2;
        rangeApply(ql, qr, b, c, 2 * p, l, m);
        rangeApply(ql, qr, b, c, 2 * p + 1, m, r);
        pull(p);
    }

    void rangeApply(int l, int r, int b, int c) { rangeApply(l, r, b, c, 1, 0, n); }

    int rangeSum(int ql, int qr, int p, int l, int r) {
        if (qr <= l || r <= ql) {
            return 0;
        }
        if (ql <= l && r <= qr) {
            return sum[p];
        }
        push(p, l, r);
        int m = (l + r) / 2;
        return add(rangeSum(ql, qr, 2 * p, l, m), rangeSum(ql, qr, 2 * p + 1, m, r));
    }

    int rangeSum(int l, int r) { return rangeSum(l, r, 1, 0, n); }
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

    LazySegmentTree seg(a);
    while (q--) {
        int op, l, r;
        cin >> op >> l >> r;
        if (op == 0) {
            int b, c;
            cin >> b >> c;
            seg.rangeApply(l, r, b, c);
        } else {
            cout << seg.rangeSum(l, r) << '\n';
        }
    }
    return 0;
}
