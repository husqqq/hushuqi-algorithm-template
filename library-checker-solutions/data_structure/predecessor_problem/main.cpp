#include <bits/stdc++.h>

using namespace std;

using u64 = unsigned long long;

struct FastSet {
    int n;
    vector<vector<u64>> a;

    FastSet(int n) : n(n) {
        do {
            a.push_back(vector<u64>((n + 63) / 64));
            n = (n + 63) / 64;
        } while (n > 1);
    }

    void set(int x) {
        for (auto& level : a) {
            level[x / 64] |= 1ULL << (x % 64);
            x /= 64;
        }
    }

    void reset(int x) {
        for (auto& level : a) {
            level[x / 64] &= ~(1ULL << (x % 64));
            if (level[x / 64]) break;
            x /= 64;
        }
    }

    bool test(int x) const { return a[0][x / 64] >> (x % 64) & 1; }

    int next(int x) const {
        for (int height = 0; height < (int)a.size(); height++) {
            if (x / 64 >= (int)a[height].size()) break;
            u64 value = a[height][x / 64] >> (x % 64);
            if (!value) {
                x = x / 64 + 1;
                continue;
            }
            x += countr_zero(value);
            while (height--) {
                x *= 64;
                x += countr_zero(a[height][x / 64]);
            }
            return x < n ? x : -1;
        }
        return -1;
    }

    int previous(int x) const {
        for (int height = 0; height < (int)a.size() && x >= 0; height++) {
            u64 value = a[height][x / 64] << (63 - x % 64);
            if (!value) {
                x = x / 64 - 1;
                continue;
            }
            x -= countl_zero(value);
            while (height--) {
                x = x * 64 + 63;
                x -= countl_zero(a[height][x / 64]);
            }
            return x;
        }
        return -1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    string s;
    cin >> n >> q >> s;
    FastSet set(n);
    for (int i = 0; i < n; i++) {
        if (s[i] == '1') {
            set.set(i);
        }
    }
    while (q--) {
        int op, x;
        cin >> op >> x;
        if (op == 0) {
            set.set(x);
        } else if (op == 1) {
            set.reset(x);
        } else if (op == 2) {
            cout << set.test(x) << '\n';
        } else if (op == 3) {
            cout << set.next(x) << '\n';
        } else {
            cout << set.previous(x) << '\n';
        }
    }
    return 0;
}
