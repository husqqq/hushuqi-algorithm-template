#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

template <class T> struct Fenwick {
    int n;
    vector<T> a;

    Fenwick(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        a.assign(n, T{});
    }

    void add(int x, T v) {
        for (int i = x + 1; i <= n; i += i & -i) {
            a[i - 1] += v;
        }
    }

    T sum(int x) const {
        T ans{};
        for (int i = x; i > 0; i -= i & -i) {
            ans += a[i - 1];
        }
        return ans;
    }

    T rangeSum(int l, int r) const { return sum(r) - sum(l); }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    Fenwick<i64> fenwick(n);
    for (int i = 0; i < n; i++) {
        i64 x;
        cin >> x;
        fenwick.add(i, x);
    }

    while (q--) {
        int op, x, y;
        cin >> op >> x >> y;
        if (op == 0) {
            fenwick.add(x, y);
        } else {
            cout << fenwick.rangeSum(x, y) << '\n';
        }
    }
    return 0;
}
