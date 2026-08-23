#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

i64 powerMod(i64 a, i64 b, i64 mod) {
    i64 answer = 1;
    for (; b; b >>= 1, a = a * a % mod) {
        if (b & 1) {
            answer = answer * a % mod;
        }
    }
    return answer;
}

int sqrtMod(int y, int p) {
    if (y == 0 || p == 2) {
        return y;
    }
    if (powerMod(y, (p - 1) / 2, p) != 1) {
        return -1;
    }
    if (p % 4 == 3) {
        return powerMod(y, (p + 1) / 4, p);
    }

    int s = countr_zero((unsigned)(p - 1));
    i64 q = (p - 1) >> s;
    int z = 2;
    while (powerMod(z, (p - 1) / 2, p) == 1) {
        z++;
    }

    i64 c = powerMod(z, q, p);
    i64 x = powerMod(y, (q + 1) / 2, p);
    i64 t = powerMod(y, q, p);
    int m = s;
    while (t != 1) {
        int i = 1;
        i64 value = t * t % p;
        while (value != 1) {
            value = value * value % p;
            i++;
        }
        i64 b = powerMod(c, 1LL << (m - i - 1), p);
        x = x * b % p;
        t = t * b % p * b % p;
        c = b * b % p;
        m = i;
    }
    return x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int y, p;
        cin >> y >> p;
        cout << sqrtMod(y, p) << '\n';
    }
    return 0;
}
