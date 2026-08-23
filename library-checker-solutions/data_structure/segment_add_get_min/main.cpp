#include <bits/stdc++.h>

using namespace std;

using i128 = __int128_t;
constexpr long long INF = 4'000'000'000'000'000'000LL;

struct Line {
    long long a = 0, b = INF;
    i128 operator()(long long x) const { return i128(a) * x + b; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    struct Operation {
        int type;
        long long l, r, a, b;
    };
    vector<Operation> operations;
    vector<long long> xCoordinates;
    for (int i = 0; i < n; i++) {
        Operation operation{0};
        cin >> operation.l >> operation.r >> operation.a >> operation.b;
        operations.push_back(operation);
    }
    for (int i = 0; i < q; i++) {
        Operation operation;
        cin >> operation.type;
        if (operation.type == 0) {
            cin >> operation.l >> operation.r >> operation.a >> operation.b;
        } else {
            cin >> operation.l;
            xCoordinates.push_back(operation.l);
        }
        operations.push_back(operation);
    }
    sort(xCoordinates.begin(), xCoordinates.end());
    xCoordinates.erase(unique(xCoordinates.begin(), xCoordinates.end()), xCoordinates.end());
    int size = xCoordinates.size();
    vector<Line> tree(4 * max(1, size));
    auto insertLine = [&](auto&& self, int node, int l, int r, Line line) -> void {
        int mid = (l + r) / 2;
        bool leftBetter = line(xCoordinates[l]) < tree[node](xCoordinates[l]);
        bool middleBetter = line(xCoordinates[mid]) < tree[node](xCoordinates[mid]);
        if (middleBetter) {
            swap(line, tree[node]);
        }
        if (r - l == 1) {
            return;
        }
        if (leftBetter != middleBetter) {
            self(self, node * 2, l, mid, line);
        } else {
            self(self, node * 2 + 1, mid, r, line);
        }
    };
    auto addSegment = [&](auto&& self, int node, int l, int r, int ql, int qr, Line line) -> void {
        if (qr <= l || r <= ql) {
            return;
        }
        if (ql <= l && r <= qr) {
            insertLine(insertLine, node, l, r, line);
            return;
        }
        int mid = (l + r) / 2;
        self(self, node * 2, l, mid, ql, qr, line);
        self(self, node * 2 + 1, mid, r, ql, qr, line);
    };
    auto query = [&](int position) {
        i128 answer = INF;
        int node = 1, l = 0, r = size;
        while (true) {
            answer = min(answer, tree[node](xCoordinates[position]));
            if (r - l == 1) {
                break;
            }
            int mid = (l + r) / 2;
            if (position < mid) {
                node *= 2;
                r = mid;
            } else {
                node = node * 2 + 1;
                l = mid;
            }
        }
        return answer;
    };
    for (auto operation : operations) {
        if (operation.type == 0) {
            int l = lower_bound(xCoordinates.begin(), xCoordinates.end(), operation.l) - xCoordinates.begin();
            int r = lower_bound(xCoordinates.begin(), xCoordinates.end(), operation.r) - xCoordinates.begin();
            if (l < r) {
                addSegment(addSegment, 1, 0, size, l, r, {operation.a, operation.b});
            }
        } else {
            int position = lower_bound(xCoordinates.begin(), xCoordinates.end(), operation.l) - xCoordinates.begin();
            i128 answer = query(position);
            if (answer >= INF) {
                cout << "INFINITY\n";
            } else {
                cout << (long long)answer << '\n';
            }
        }
    }
    return 0;
}
