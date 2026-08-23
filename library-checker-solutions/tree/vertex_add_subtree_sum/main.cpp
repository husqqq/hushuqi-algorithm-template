#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

struct Fenwick {
    vector<i64> a;

    Fenwick(int n) : a(n + 1) {}

    void add(int x, i64 value) {
        for (x++; x < (int)a.size(); x += x & -x) {
            a[x] += value;
        }
    }

    i64 sum(int x) const {
        i64 answer = 0;
        for (; x > 0; x -= x & -x) {
            answer += a[x];
        }
        return answer;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<i64> value(n);
    for (auto& x : value) {
        cin >> x;
    }
    vector<vector<int>> children(n);
    for (int i = 1; i < n; i++) {
        int parent;
        cin >> parent;
        children[parent].push_back(i);
    }

    vector<int> begin(n), end(n), stack{0}, next(n);
    int timer = 0;
    while (!stack.empty()) {
        int u = stack.back();
        if (next[u] == 0) {
            begin[u] = timer++;
        }
        if (next[u] < (int)children[u].size()) {
            stack.push_back(children[u][next[u]++]);
        } else {
            end[u] = timer;
            stack.pop_back();
        }
    }

    Fenwick fenwick(n);
    for (int i = 0; i < n; i++) {
        fenwick.add(begin[i], value[i]);
    }
    while (q--) {
        int op, u;
        cin >> op >> u;
        if (op == 0) {
            i64 x;
            cin >> x;
            fenwick.add(begin[u], x);
        } else {
            cout << fenwick.sum(end[u]) - fenwick.sum(begin[u]) << '\n';
        }
    }
    return 0;
}
