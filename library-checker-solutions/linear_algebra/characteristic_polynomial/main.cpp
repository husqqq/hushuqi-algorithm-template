#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;

long long power(long long a, int b) {
    long long result = 1;
    while (b) {
        if (b & 1) {
            result = result * a % P;
        }
        a = a * a % P;
        b >>= 1;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<vector<long long>> a(n, vector<long long>(n));
    for (auto& row : a) {
        for (auto& value : row) {
            cin >> value;
        }
    }

    for (int column = 0; column + 2 < n; column++) {
        int pivot = column + 1;
        while (pivot < n && a[pivot][column] == 0) {
            pivot++;
        }
        if (pivot == n) {
            continue;
        }
        if (pivot != column + 1) {
            swap(a[pivot], a[column + 1]);
            for (int row = 0; row < n; row++) {
                swap(a[row][pivot], a[row][column + 1]);
            }
        }
        long long inverse = power(a[column + 1][column], P - 2);
        for (int row = column + 2; row < n; row++) {
            if (!a[row][column]) {
                continue;
            }
            long long factor = a[row][column] * inverse % P;
            for (int j = column; j < n; j++) {
                a[row][j] = (a[row][j] - factor * a[column + 1][j]) % P;
                if (a[row][j] < 0) {
                    a[row][j] += P;
                }
            }
            for (int i = 0; i < n; i++) {
                a[i][column + 1] = (a[i][column + 1] + factor * a[i][row]) % P;
            }
        }
    }

    vector<vector<long long>> characteristic(n + 1);
    characteristic[0] = {1};
    for (int size = 1; size <= n; size++) {
        characteristic[size].assign(size + 1, 0);
        for (int degree = 0; degree < size; degree++) {
            characteristic[size][degree + 1] += characteristic[size - 1][degree];
            characteristic[size][degree] =
                (characteristic[size][degree] - a[size - 1][size - 1] * characteristic[size - 1][degree]) % P;
        }
        long long product = 1;
        for (int distance = 1; distance < size; distance++) {
            product = product * a[size - distance][size - distance - 1] % P;
            long long coefficient = product * a[size - distance - 1][size - 1] % P;
            for (int degree = 0; degree <= size - distance - 1; degree++) {
                characteristic[size][degree] =
                    (characteristic[size][degree] - coefficient * characteristic[size - distance - 1][degree]) % P;
            }
        }
        for (auto& value : characteristic[size]) {
            if (value < 0) {
                value += P;
            }
        }
    }
    for (int i = 0; i <= n; i++) {
        cout << characteristic[n][i] << " \n"[i == n];
    }
    return 0;
}
