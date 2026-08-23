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
            answer = P - answer;
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

    int n, m;
    cin >> n >> m;
    vector laplacian(max(0, n - 1), vector<int>(max(0, n - 1)));
    while (m--) {
        int u, v;
        cin >> u >> v;
        if (u == v) {
            continue;
        }
        if (u < n - 1) {
            if (++laplacian[u][u] == P) {
                laplacian[u][u] = 0;
            }
        }
        if (v < n - 1) {
            if (++laplacian[v][v] == P) {
                laplacian[v][v] = 0;
            }
        }
        if (u < n - 1 && v < n - 1) {
            laplacian[u][v] = laplacian[u][v] == 0 ? P - 1 : laplacian[u][v] - 1;
            laplacian[v][u] = laplacian[v][u] == 0 ? P - 1 : laplacian[v][u] - 1;
        }
    }
    cout << determinant(move(laplacian)) << '\n';
    return 0;
}
