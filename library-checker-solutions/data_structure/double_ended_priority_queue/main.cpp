#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    multiset<long long> set;
    while (n--) {
        long long x;
        cin >> x;
        set.insert(x);
    }
    while (q--) {
        int op;
        cin >> op;
        if (op == 0) {
            long long x;
            cin >> x;
            set.insert(x);
        } else if (op == 1) {
            auto it = set.begin();
            cout << *it << '\n';
            set.erase(it);
        } else {
            auto it = prev(set.end());
            cout << *it << '\n';
            set.erase(it);
        }
    }
    return 0;
}
