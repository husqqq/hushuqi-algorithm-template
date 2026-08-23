#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;
    vector a(n, vector<int>(m));
    vector b(m, vector<int>(k));
    vector c(n, vector<int>(k));
    for (auto& row : a) {
        for (auto& x : row) {
            cin >> x;
        }
    }
    for (auto& row : b) {
        for (auto& x : row) {
            cin >> x;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int x = 0; x < m; x++) {
            if (a[i][x] == 0) {
                continue;
            }
            for (int j = 0; j < k; j++) {
                c[i][j] = (c[i][j] + 1LL * a[i][x] * b[x][j]) % P;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            cout << c[i][j] << " \n"[j + 1 == k];
        }
    }
    return 0;
}
