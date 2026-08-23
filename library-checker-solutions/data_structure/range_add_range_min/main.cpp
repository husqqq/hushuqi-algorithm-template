#ifndef TEMPLATE_INLINE_7900F2F40E1A16337052
#define TEMPLATE_INLINE_7900F2F40E1A16337052

#include <bits/stdc++.h>

using namespace std;

class RangeAddMinTree {
    int size;
    vector<long long> minimum, lazy;

    void build(int node, int left, int right, const vector<long long>& values) {
        if (right - left == 1) {
            minimum[node] = values[left];
            return;
        }
        int middle = (left + right) / 2;
        build(node * 2, left, middle, values);
        build(node * 2 + 1, middle, right, values);
        pull(node);
    }

    void apply(int node, long long value) {
        minimum[node] += value;
        lazy[node] += value;
    }

    void push(int node) {
        if (lazy[node] == 0) return;
        apply(node * 2, lazy[node]);
        apply(node * 2 + 1, lazy[node]);
        lazy[node] = 0;
    }

    void pull(int node) { minimum[node] = min(minimum[node * 2], minimum[node * 2 + 1]); }

    void add(int node, int left, int right, int queryLeft, int queryRight, long long value) {
        if (queryRight <= left || right <= queryLeft) return;
        if (queryLeft <= left && right <= queryRight) {
            apply(node, value);
            return;
        }
        push(node);
        int middle = (left + right) / 2;
        add(node * 2, left, middle, queryLeft, queryRight, value);
        add(node * 2 + 1, middle, right, queryLeft, queryRight, value);
        pull(node);
    }

    long long rangeMin(int node, int left, int right, int queryLeft, int queryRight) {
        if (queryRight <= left || right <= queryLeft) return numeric_limits<long long>::max();
        if (queryLeft <= left && right <= queryRight) return minimum[node];
        push(node);
        int middle = (left + right) / 2;
        return min(rangeMin(node * 2, left, middle, queryLeft, queryRight),
                   rangeMin(node * 2 + 1, middle, right, queryLeft, queryRight));
    }

  public:
    explicit RangeAddMinTree(const vector<long long>& values) : size(values.size()), minimum(size * 4), lazy(size * 4) {
        build(1, 0, size, values);
    }

    void add(int left, int right, long long value) { add(1, 0, size, left, right, value); }

    long long rangeMin(int left, int right) { return rangeMin(1, 0, size, left, right); }
};
#endif


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> values(n);
    for (auto& value : values) cin >> value;
    RangeAddMinTree tree(values);
    while (q--) {
        int type, left, right;
        cin >> type >> left >> right;
        if (type == 0) {
            long long value;
            cin >> value;
            tree.add(left, right, value);
        } else cout << tree.rangeMin(left, right) << '\n';
    }
}
