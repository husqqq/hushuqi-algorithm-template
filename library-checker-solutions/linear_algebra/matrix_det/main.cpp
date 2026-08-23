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

int determinant(vector<vector<int>> a) {
    int n = a.size();
    int answer = 1;
    for (int column = 0; column < n; column++) {
        int pivot = column;
        while (pivot < n && a[pivot][column] == 0) {
            pivot++;
        }
        if (pivot == n) {
            return 0;
        }
        if (pivot != column) {
            swap(a[pivot], a[column]);
            answer = answer == 0 ? 0 : P - answer;
        }
        answer = 1LL * answer * a[column][column] % P;
        int inverse = power(a[column][column], P - 2);
        for (int row = column + 1; row < n; row++) {
            int factor = 1LL * a[row][column] * inverse % P;
            for (int j = column; j < n; j++) {
                a[row][j] = (a[row][j] - 1LL * factor * a[column][j]) % P;
                if (a[row][j] < 0) {
                    a[row][j] += P;
                }
            }
        }
    }
    return answer;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector a(n, vector<int>(n));
    for (auto& row : a) {
        for (auto& x : row) {
            cin >> x;
        }
    }
    cout << determinant(move(a)) << '\n';
    return 0;
}
