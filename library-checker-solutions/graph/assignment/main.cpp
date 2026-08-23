#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector a(n, vector<i64>(n));
    for (auto& row : a) {
        for (auto& x : row) {
            cin >> x;
        }
    }

    vector<i64> u(n + 1), v(n + 1);
    vector<int> matching(n + 1), way(n + 1);
    for (int row = 1; row <= n; row++) {
        matching[0] = row;
        int column0 = 0;
        vector<i64> minimum(n + 1, numeric_limits<i64>::max());
        vector<bool> used(n + 1);
        do {
            used[column0] = true;
            int row0 = matching[column0];
            i64 delta = numeric_limits<i64>::max();
            int column1 = 0;
            for (int column = 1; column <= n; column++) {
                if (used[column]) {
                    continue;
                }
                i64 current = a[row0 - 1][column - 1] - u[row0] - v[column];
                if (current < minimum[column]) {
                    minimum[column] = current;
                    way[column] = column0;
                }
                if (minimum[column] < delta) {
                    delta = minimum[column];
                    column1 = column;
                }
            }
            for (int column = 0; column <= n; column++) {
                if (used[column]) {
                    u[matching[column]] += delta;
                    v[column] -= delta;
                } else {
                    minimum[column] -= delta;
                }
            }
            column0 = column1;
        } while (matching[column0] != 0);

        do {
            int previous = way[column0];
            matching[column0] = matching[previous];
            column0 = previous;
        } while (column0 != 0);
    }

    vector<int> answer(n);
    for (int column = 1; column <= n; column++) {
        answer[matching[column] - 1] = column - 1;
    }
    i64 cost = 0;
    for (int i = 0; i < n; i++) {
        cost += a[i][answer[i]];
    }
    cout << cost << '\n';
    for (int i = 0; i < n; i++) {
        cout << answer[i] << " \n"[i + 1 == n];
    }
    return 0;
}
