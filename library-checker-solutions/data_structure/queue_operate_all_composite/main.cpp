#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;

struct Func {
    int a = 1;
    int b = 0;
};

Func operator+(const Func& left, const Func& right) {
    return {
        int(1LL * right.a * left.a % P),
        int((1LL * right.a * left.b + right.b) % P),
    };
}

struct FoldQueue {
    vector<pair<Func, Func>> left, right;

    void push(Func f) {
        Func sum = right.empty() ? f : right.back().second + f;
        right.emplace_back(f, sum);
    }

    void transfer() {
        while (!right.empty()) {
            Func f = right.back().first;
            right.pop_back();
            Func sum = left.empty() ? f : f + left.back().second;
            left.emplace_back(f, sum);
        }
    }

    void pop() {
        if (left.empty()) {
            transfer();
        }
        left.pop_back();
    }

    Func sum() const {
        Func a = left.empty() ? Func{} : left.back().second;
        Func b = right.empty() ? Func{} : right.back().second;
        return a + b;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    FoldQueue queue;
    while (q--) {
        int op;
        cin >> op;
        if (op == 0) {
            int a, b;
            cin >> a >> b;
            queue.push({a, b});
        } else if (op == 1) {
            queue.pop();
        } else {
            int x;
            cin >> x;
            auto f = queue.sum();
            cout << (1LL * f.a * x + f.b) % P << '\n';
        }
    }
    return 0;
}
