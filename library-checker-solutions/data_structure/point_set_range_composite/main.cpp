#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;

struct Info {
    int a = 1;
    int b = 0;
};

Info operator+(const Info& left, const Info& right) {
    return {
        int(1LL * right.a * left.a % P),
        int((1LL * right.a * left.b + right.b) % P),
    };
}

struct SegmentTree {
    int n;
    vector<Info> info;

    SegmentTree(const vector<Info>& a = {}) {
        if (!a.empty()) {
            init(a);
        }
    }

    void init(const vector<Info>& a) {
        n = a.size();
        info.assign(2 * n, {});
        copy(a.begin(), a.end(), info.begin() + n);
        for (int i = n - 1; i > 0; i--) {
            info[i] = info[2 * i] + info[2 * i + 1];
        }
    }

    void modify(int p, Info v) {
        for (info[p += n] = v; p > 1; p >>= 1) {
            info[p >> 1] = info[p & ~1] + info[p | 1];
        }
    }

    Info rangeQuery(int l, int r) const {
        Info left, right;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) {
                left = left + info[l++];
            }
            if (r & 1) {
                right = info[--r] + right;
            }
        }
        return left + right;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Info> a(n);
    for (auto& [x, y] : a) {
        cin >> x >> y;
    }

    SegmentTree seg(a);
    while (q--) {
        int op, x, y, z;
        cin >> op >> x >> y >> z;
        if (op == 0) {
            seg.modify(x, {y, z});
        } else {
            auto f = seg.rangeQuery(x, y);
            cout << (1LL * f.a * z + f.b) % P << '\n';
        }
    }
    return 0;
}
