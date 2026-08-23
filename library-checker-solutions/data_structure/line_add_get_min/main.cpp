#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
using i128 = __int128_t;

constexpr i64 INF = numeric_limits<i64>::max();

struct Line {
    i64 a = 0;
    i64 b = INF;

    i128 operator()(i64 x) const { return i128(a) * x + b; }
};

struct LiChaoTree {
    vector<i64> x;
    vector<Line> line;

    LiChaoTree(vector<i64> x = {}) : x(move(x)) { line.assign(4 * max(1, (int)this->x.size()), {}); }

    void addLine(Line v, int p, int l, int r) {
        int m = (l + r) / 2;
        bool left = v(x[l]) < line[p](x[l]);
        bool middle = v(x[m]) < line[p](x[m]);

        if (middle) {
            swap(v, line[p]);
        }
        if (r - l == 1) {
            return;
        }
        if (left != middle) {
            addLine(v, 2 * p, l, m);
        } else {
            addLine(v, 2 * p + 1, m, r);
        }
    }

    void addLine(Line v) {
        if (!x.empty()) {
            addLine(v, 1, 0, x.size());
        }
    }

    i64 query(int index, int p, int l, int r) const {
        i128 ans = line[p](x[index]);
        if (r - l > 1) {
            int m = (l + r) / 2;
            if (index < m) {
                ans = min(ans, i128(query(index, 2 * p, l, m)));
            } else {
                ans = min(ans, i128(query(index, 2 * p + 1, m, r)));
            }
        }
        return i64(ans);
    }

    i64 queryByIndex(int index) const { return query(index, 1, 0, x.size()); }
};

struct Query {
    int op;
    i64 a, b;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Line> initial(n);
    for (auto& [a, b] : initial) {
        cin >> a >> b;
    }

    vector<Query> queries(q);
    vector<i64> xs;
    for (auto& [op, a, b] : queries) {
        cin >> op >> a;
        b = 0;
        if (op == 0) {
            cin >> b;
        } else {
            xs.push_back(a);
        }
    }

    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    LiChaoTree tree(xs);
    for (auto v : initial) {
        tree.addLine(v);
    }

    for (auto [op, a, b] : queries) {
        if (op == 0) {
            tree.addLine({a, b});
        } else {
            int index = lower_bound(xs.begin(), xs.end(), a) - xs.begin();
            cout << tree.queryByIndex(index) << '\n';
        }
    }
    return 0;
}
