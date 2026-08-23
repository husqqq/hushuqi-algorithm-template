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

    int n, m;
    cin >> n >> m;
    vector<vector<int>> a(n, vector<int>(m + 1));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> a[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        cin >> a[i][m];
    }

    vector<int> pivotColumn;
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
        long long inverse = power(a[rank][column], P - 2);
        for (int j = column; j <= m; j++) {
            a[rank][j] = a[rank][j] * inverse % P;
        }
        for (int i = 0; i < n; i++) {
            if (i == rank || a[i][column] == 0) {
                continue;
            }
            long long factor = a[i][column];
            for (int j = column; j <= m; j++) {
                a[i][j] = (a[i][j] - factor * a[rank][j]) % P;
                if (a[i][j] < 0) {
                    a[i][j] += P;
                }
            }
        }
        pivotColumn.push_back(column);
        rank++;
    }

    for (int i = rank; i < n; i++) {
        if (a[i][m]) {
            cout << -1 << '\n';
            return 0;
        }
    }

    vector<int> isPivot(m);
    for (int column : pivotColumn) {
        isPivot[column] = 1;
    }
    vector<vector<int>> answer(1, vector<int>(m));
    for (int i = 0; i < rank; i++) {
        answer[0][pivotColumn[i]] = a[i][m];
    }
    for (int freeColumn = 0; freeColumn < m; freeColumn++) {
        if (isPivot[freeColumn]) {
            continue;
        }
        vector<int> basis(m);
        basis[freeColumn] = 1;
        for (int i = 0; i < rank; i++) {
            if (a[i][freeColumn]) {
                basis[pivotColumn[i]] = P - a[i][freeColumn];
            }
        }
        answer.push_back(move(basis));
    }

    cout << answer.size() - 1 << '\n';
    for (auto& row : answer) {
        for (int i = 0; i < m; i++) {
            cout << row[i] << " \n"[i + 1 == m];
        }
    }
    return 0;
}
