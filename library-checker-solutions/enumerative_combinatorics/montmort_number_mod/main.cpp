#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    i64 mod;
    cin >> n >> mod;
    vector<i64> derangement(n + 1);
    derangement[0] = 1 % mod;
    if (n >= 1) {
        derangement[1] = 0;
    }
    for (int i = 2; i <= n; i++) {
        derangement[i] = (i - 1) * (derangement[i - 1] + derangement[i - 2]) % mod;
    }
    for (int i = 1; i <= n; i++) {
        cout << derangement[i] << " \n"[i == n];
    }
    return 0;
}
