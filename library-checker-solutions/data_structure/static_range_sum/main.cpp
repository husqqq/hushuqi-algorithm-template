#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<i64> sum(n + 1);
    for (int i = 0; i < n; i++) {
        i64 x;
        cin >> x;
        sum[i + 1] = sum[i] + x;
    }
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << sum[r] - sum[l] << '\n';
    }
    return 0;
}
