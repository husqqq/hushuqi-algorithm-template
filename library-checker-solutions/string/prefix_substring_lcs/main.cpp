// Library Checker official reference implementation, adapted to this template's style.
// Problem: prefix_substring_lcs
#include <bits/stdc++.h>
using namespace std;

class fenwick_tree {
    vector<int> v;

  public:
    fenwick_tree(int n) : v(n + 1, 0) {}
    void add(int i) {
        i += 1;
        while (i < int(v.size())) {
            v[i] += 1;
            i += i & (~i + 1);
        }
    }
    int sum(int i) {
        int ret = 0;
        while (i != 0) {
            ret += v[i];
            i &= i - 1;
        }
        return ret;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int Q;
    string S, T;
    cin >> Q >> S >> T;
    const int M = S.size();
    const int N = T.size();

    struct query_t {
        int a;
        int b;
        int c;
        int ans;
    };
    vector<query_t> qs(Q);
    for (auto& q : qs) {
        cin >> q.a >> q.b >> q.c;
    }

    vector<vector<query_t*>> sep_a(M);
    for (query_t& q : qs) {
        if (q.a == 0) {
            q.ans = 0;
        } else {
            sep_a[q.a - 1].push_back(&q);
        }
    }

    vector<int> h(N);
    iota(h.begin(), h.end(), 0);
    for (int a = 0; a < M; a++) {
        int v = -1;
        for (int c = 0; c < N; c++) {
            if (S[a] == T[c] || h[c] < v) {
                swap(h[c], v);
            }
        }
        vector<vector<query_t*>> sep_c(N);
        for (const auto q : sep_a[a]) {
            if (q->c == 0) {
                q->ans = 0;
            } else {
                sep_c[q->c - 1].push_back(q);
            }
        }
        fenwick_tree ft(N);
        int count = 0;
        for (int c = 0; c < N; c++) {
            if (h[c] != -1) {
                ft.add(h[c]);
                count += 1;
            }
            for (const auto q : sep_c[c]) {
                q->ans = (q->c - q->b) - (count - ft.sum(q->b));
            }
        }
    }

    for (const auto& q : qs) {
        cout << q.ans << "\n";
    }

    return 0;
}
