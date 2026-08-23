#include <bits/stdc++.h>

using namespace std;

struct Fenwick {
    vector<int> tree;
    Fenwick(int n) : tree(n + 1) {}
    void add(int index, int value) {
        for (index++; index < int(tree.size()); index += index & -index) {
            tree[index] += value;
        }
    }
    int sum(int index) const {
        int result = 0;
        for (; index; index -= index & -index) {
            result += tree[index];
        }
        return result;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n), values;
    for (int& x : a) {
        cin >> x;
    }
    values = a;
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    for (int& x : a) {
        x = lower_bound(values.begin(), values.end(), x) - values.begin();
    }
    vector<vector<pair<int, int>>> queries(n + 1);
    for (int id = 0; id < q; id++) {
        int l, r;
        cin >> l >> r;
        queries[r].push_back({l, id});
    }
    vector<int> last(values.size(), -1), answer(q);
    Fenwick fenwick(n);
    for (int r = 1; r <= n; r++) {
        int value = a[r - 1];
        if (last[value] != -1) {
            fenwick.add(last[value], -1);
        }
        last[value] = r - 1;
        fenwick.add(r - 1, 1);
        for (auto [l, id] : queries[r]) {
            answer[id] = fenwick.sum(r) - fenwick.sum(l);
        }
    }
    for (int value : answer) {
        cout << value << '\n';
    }
    return 0;
}
