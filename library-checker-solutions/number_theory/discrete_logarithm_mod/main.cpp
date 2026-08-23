#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

i64 extendedGcd(i64 a, i64 b, i64& x, i64& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    i64 x1, y1;
    i64 g = extendedGcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - a / b * y1;
    return g;
}

i64 inverse(i64 a, i64 mod) {
    i64 x, y;
    extendedGcd(a, mod, x, y);
    x %= mod;
    return x < 0 ? x + mod : x;
}

i64 powerMod(i64 a, i64 b, i64 mod) {
    i64 answer = 1 % mod;
    for (; b; b >>= 1, a = a * a % mod) {
        if (b & 1) {
            answer = answer * a % mod;
        }
    }
    return answer;
}

i64 bsgs(i64 x, i64 y, i64 mod) {
    int block = sqrt((long double)mod) + 1;
    unordered_map<i64, int> baby;
    baby.reserve(2 * block + 1);
    i64 value = 1 % mod;
    for (int j = 0; j < block; j++) {
        baby.emplace(value, j);
        value = value * x % mod;
    }
    i64 step = inverse(powerMod(x, block, mod), mod);
    value = y;
    for (int i = 0; i <= block; i++) {
        auto it = baby.find(value);
        if (it != baby.end()) {
            return 1LL * i * block + it->second;
        }
        value = value * step % mod;
    }
    return -1;
}

i64 discreteLog(i64 x, i64 y, i64 mod) {
    x %= mod;
    y %= mod;
    if (mod == 1 || y == 1 % mod) {
        return 0;
    }

    i64 factor = 1;
    int offset = 0;
    while (true) {
        i64 g = gcd(x, mod);
        if (g == 1) {
            break;
        }
        if (y % g != 0) {
            return -1;
        }
        mod /= g;
        y /= g;
        factor = factor * (x / g) % mod;
        offset++;
        if (factor == y) {
            return offset;
        }
    }
    i64 target = y * inverse(factor, mod) % mod;
    i64 answer = bsgs(x % mod, target, mod);
    return answer == -1 ? -1 : answer + offset;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        i64 x, y, mod;
        cin >> x >> y >> mod;
        cout << discreteLog(x, y, mod) << '\n';
    }
    return 0;
}
