#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

using OrderedSet = tree<long long, null_type, less<long long>, rb_tree_tag, tree_order_statistics_node_update>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    OrderedSet set;
    while (n--) {
        long long x;
        cin >> x;
        set.insert(x);
    }

    while (q--) {
        int op;
        long long x;
        cin >> op >> x;
        if (op == 0) {
            set.insert(x);
        } else if (op == 1) {
            set.erase(x);
        } else if (op == 2) {
            if (x <= 0 || x > (long long)set.size()) {
                cout << -1 << '\n';
            } else {
                cout << *set.find_by_order(x - 1) << '\n';
            }
        } else if (op == 3) {
            cout << set.order_of_key(x + 1) << '\n';
        } else if (op == 4) {
            auto it = set.upper_bound(x);
            cout << (it == set.begin() ? -1 : *prev(it)) << '\n';
        } else {
            auto it = set.lower_bound(x);
            cout << (it == set.end() ? -1 : *it) << '\n';
        }
    }
    return 0;
}
