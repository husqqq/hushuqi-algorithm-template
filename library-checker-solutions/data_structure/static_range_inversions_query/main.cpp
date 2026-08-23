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

struct Query {
    int l, r, id;
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
    vector<Query> queries(q);
    for (int i = 0; i < q; i++) {
        cin >> queries[i].l >> queries[i].r;
        queries[i].id = i;
    }
    int block = max(1, int(n / max(1.0, sqrt(q))));
    sort(queries.begin(), queries.end(), [&](const Query& x, const Query& y) {
        int bx = x.l / block, by = y.l / block;
        if (bx != by) {
            return bx < by;
        }
        return bx & 1 ? x.r > y.r : x.r < y.r;
    });

    Fenwick fenwick(values.size());
    vector<long long> answer(q);
    long long inversions = 0;
    int left = 0, right = 0, size = 0;
    for (auto query : queries) {
        while (right < query.r) {
            int x = a[right++];
            inversions += size - fenwick.sum(x + 1);
            fenwick.add(x, 1);
            size++;
        }
        while (right > query.r) {
            int x = a[--right];
            fenwick.add(x, -1);
            size--;
            inversions -= size - fenwick.sum(x + 1);
        }
        while (left > query.l) {
            int x = a[--left];
            inversions += fenwick.sum(x);
            fenwick.add(x, 1);
            size++;
        }
        while (left < query.l) {
            int x = a[left++];
            fenwick.add(x, -1);
            size--;
            inversions -= fenwick.sum(x);
        }
        answer[query.id] = inversions;
    }
    for (auto value : answer) {
        cout << value << '\n';
    }
    return 0;
}
