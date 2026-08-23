#include <bits/stdc++.h>

using namespace std;

template <class T> struct SparseTable {
    int n;
    vector<vector<T>> a;

    SparseTable(const vector<T>& v = {}) {
        if (!v.empty()) {
            init(v);
        }
    }

    void init(const vector<T>& v) {
        n = v.size();
        int log = bit_width((unsigned)n);
        a.assign(log, vector<T>(n));
        a[0] = v;
        for (int k = 1; k < log; k++) {
            for (int i = 0; i + (1 << k) <= n; i++) {
                a[k][i] = min(a[k - 1][i], a[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    T rangeMin(int l, int r) const {
        int k = bit_width((unsigned)(r - l)) - 1;
        return min(a[k][l], a[k][r - (1 << k)]);
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

    SparseTable<int> st(a);
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << st.rangeMin(l, r) << '\n';
    }
    return 0;
}
