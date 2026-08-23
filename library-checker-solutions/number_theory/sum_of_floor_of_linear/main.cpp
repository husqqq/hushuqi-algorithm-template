#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
using i128 = __int128_t;

i64 floorSum(i64 n, i64 m, i64 a, i64 b) {
    i128 answer = 0;
    while (true) {
        if (a >= m) {
            answer += i128(n - 1) * n * (a / m) / 2;
            a %= m;
        }
        if (b >= m) {
            answer += i128(n) * (b / m);
            b %= m;
        }
        i64 y = a * n + b;
        if (y < m) {
            return i64(answer);
        }
        n = y / m;
        b = y % m;
        swap(a, m);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        i64 n, m, a, b;
        cin >> n >> m >> a >> b;
        cout << floorSum(n, m, a, b) << '\n';
    }
    return 0;
}
