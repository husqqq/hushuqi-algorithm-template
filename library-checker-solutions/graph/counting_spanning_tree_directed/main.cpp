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

    int n, m, root;
    cin >> n >> m >> root;
    vector<vector<int>> laplacian(n, vector<int>(n));
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        if (++laplacian[v][v] == P) {
            laplacian[v][v] = 0;
        }
        if (--laplacian[v][u] < 0) {
            laplacian[v][u] += P;
        }
    }

    vector<vector<int>> a(n - 1, vector<int>(n - 1));
    for (int i = 0, x = 0; i < n; i++) {
        if (i == root) {
            continue;
        }
        for (int j = 0, y = 0; j < n; j++) {
            if (j != root) {
                a[x][y++] = laplacian[i][j];
            }
        }
        x++;
    }

    long long answer = 1;
    for (int i = 0; i + 1 <= n - 1; i++) {
        int pivot = i;
        while (pivot < n - 1 && a[pivot][i] == 0) {
            pivot++;
        }
        if (pivot == n - 1) {
            answer = 0;
            break;
        }
        if (pivot != i) {
            swap(a[pivot], a[i]);
            answer = P - answer;
        }
        answer = answer * a[i][i] % P;
        long long inverse = power(a[i][i], P - 2);
        for (int j = i + 1; j < n - 1; j++) {
            long long factor = a[j][i] * inverse % P;
            for (int k = i; k < n - 1; k++) {
                a[j][k] = (a[j][k] - factor * a[i][k]) % P;
                if (a[j][k] < 0) {
                    a[j][k] += P;
                }
            }
        }
    }
    cout << answer << '\n';
    return 0;
}
