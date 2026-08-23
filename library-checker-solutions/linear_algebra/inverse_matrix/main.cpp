#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;

int power(int a, int b) {
    int answer = 1;
    for (; b; b >>= 1, a = 1LL * a * a % P) {
        if (b & 1) {
            answer = 1LL * answer * a % P;
        }
    }
    return answer;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector a(n, vector<int>(2 * n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> a[i][j];
        }
        a[i][n + i] = 1;
    }

    for (int column = 0; column < n; column++) {
        int pivot = column;
        while (pivot < n && a[pivot][column] == 0) {
            pivot++;
        }
        if (pivot == n) {
            cout << -1 << '\n';
            return 0;
        }
        swap(a[pivot], a[column]);
        int inverse = power(a[column][column], P - 2);
        for (auto& x : a[column]) {
            x = 1LL * x * inverse % P;
        }
        for (int row = 0; row < n; row++) {
            if (row == column) {
                continue;
            }
            int factor = a[row][column];
            for (int j = column; j < 2 * n; j++) {
                a[row][j] = (a[row][j] - 1LL * factor * a[column][j]) % P;
                if (a[row][j] < 0) {
                    a[row][j] += P;
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << a[i][n + j] << " \n"[j + 1 == n];
        }
    }
    return 0;
}
