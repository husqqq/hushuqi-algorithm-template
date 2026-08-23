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

int matrixRank(vector<vector<int>> a) {
    int n = a.size();
    int m = n ? a[0].size() : 0;
    int rank = 0;
    for (int column = 0; column < m && rank < n; column++) {
        int pivot = rank;
        while (pivot < n && a[pivot][column] == 0) {
            pivot++;
        }
        if (pivot == n) {
            continue;
        }
        swap(a[pivot], a[rank]);
        int inverse = power(a[rank][column], P - 2);
        for (int row = rank + 1; row < n; row++) {
            int factor = 1LL * a[row][column] * inverse % P;
            for (int j = column; j < m; j++) {
                a[row][j] = (a[row][j] - 1LL * factor * a[rank][j]) % P;
                if (a[row][j] < 0) {
                    a[row][j] += P;
                }
            }
        }
        rank++;
    }
    return rank;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector a(n, vector<int>(m));
    for (auto& row : a) {
        for (auto& x : row) {
            cin >> x;
        }
    }
    cout << matrixRank(move(a)) << '\n';
    return 0;
}
