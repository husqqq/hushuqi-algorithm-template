#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, mod;
    cin >> n >> mod;
    vector<vector<int>> a(n, vector<int>(n));
    for (auto& row : a) {
        for (int& value : row) {
            cin >> value;
        }
    }
    if (mod == 1) {
        cout << 0 << '\n';
        return 0;
    }
    long long answer = 1;
    bool negative = false;
    for (int column = 0; column < n; column++) {
        int pivot = column;
        while (pivot < n && a[pivot][column] == 0) {
            pivot++;
        }
        if (pivot == n) {
            cout << 0 << '\n';
            return 0;
        }
        if (pivot != column) {
            swap(a[pivot], a[column]);
            negative = !negative;
        }
        for (int row = column + 1; row < n; row++) {
            int x = a[column][column], y = a[row][column];
            if (!y) {
                continue;
            }
            long long p = 1, q = 0, r = 0, s = 1;
            while (x && y) {
                if (x < y) {
                    int quotient = y / x;
                    y -= quotient * x;
                    r = (r - p * quotient) % mod;
                    s = (s - q * quotient) % mod;
                } else {
                    int quotient = x / y;
                    x -= quotient * y;
                    p = (p - r * quotient) % mod;
                    q = (q - s * quotient) % mod;
                }
            }
            for (int j = column; j < n; j++) {
                int first = a[column][j], second = a[row][j];
                a[column][j] = (p * first + q * second) % mod;
                a[row][j] = (r * first + s * second) % mod;
                if (a[column][j] < 0) a[column][j] += mod;
                if (a[row][j] < 0) a[row][j] += mod;
            }
            if (x == 0) {
                swap(a[column], a[row]);
                negative = !negative;
            }
        }
        answer = answer * a[column][column] % mod;
        if (answer == 0) {
            break;
        }
    }
    if (negative && answer) {
        answer = mod - answer;
    }
    cout << answer << '\n';
    return 0;
}
