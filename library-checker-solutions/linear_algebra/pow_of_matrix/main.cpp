#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;

struct Matrix {
    int n;
    vector<vector<int>> a;

    Matrix(int n = 0, bool identity = false) : n(n), a(n, vector<int>(n)) {
        if (identity) {
            for (int i = 0; i < n; i++) {
                a[i][i] = 1;
            }
        }
    }

    Matrix operator*(const Matrix& b) const {
        Matrix c(n);
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                if (a[i][k] == 0) {
                    continue;
                }
                for (int j = 0; j < n; j++) {
                    c.a[i][j] = (c.a[i][j] + 1LL * a[i][k] * b.a[k][j]) % P;
                }
            }
        }
        return c;
    }
};

Matrix power(Matrix a, long long b) {
    Matrix ans(a.n, true);
    for (; b; b >>= 1, a = a * a) {
        if (b & 1) {
            ans = ans * a;
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long k;
    cin >> n >> k;
    Matrix a(n);
    for (auto& row : a.a) {
        for (auto& x : row) {
            cin >> x;
        }
    }

    a = power(a, k);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << a.a[i][j] << " \n"[j + 1 == n];
        }
    }
    return 0;
}
