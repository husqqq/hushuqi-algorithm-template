#include <bits/stdc++.h>

using namespace std;

using u64 = unsigned long long;
using u128 = __uint128_t;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

u64 powerMod(u64 a, u64 b, u64 mod) {
    u64 ans = 1;
    for (; b; b >>= 1, a = u128(a) * a % mod) {
        if (b & 1) {
            ans = u128(ans) * a % mod;
        }
    }
    return ans;
}

bool isPrime(u64 n) {
    if (n < 2) {
        return false;
    }
    for (u64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
        if (n % p == 0) {
            return n == p;
        }
    }

    int s = countr_zero(n - 1);
    u64 d = (n - 1) >> s;
    for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n == 0) {
            continue;
        }
        u64 x = powerMod(a % n, d, n);
        if (x == 1 || x == n - 1) {
            continue;
        }
        bool composite = true;
        for (int r = 1; r < s; r++) {
            x = u128(x) * x % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) {
            return false;
        }
    }
    return true;
}

u64 pollardRho(u64 n) {
    if (n % 2 == 0) {
        return 2;
    }
    while (true) {
        u64 c = rng() % (n - 1) + 1;
        u64 x = rng() % (n - 2) + 2;
        u64 y = x;
        u64 d = 1;
        auto f = [&](u64 v) { return (u128(v) * v + c) % n; };
        while (d == 1) {
            x = f(x);
            y = f(f(y));
            u64 delta = x > y ? x - y : y - x;
            d = gcd(delta, n);
        }
        if (d != n) {
            return d;
        }
    }
}

void factorize(u64 n, vector<u64>& factors) {
    if (n == 1) {
        return;
    }
    if (isPrime(n)) {
        factors.push_back(n);
        return;
    }
    u64 d = pollardRho(n);
    factorize(d, factors);
    factorize(n / d, factors);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    while (q--) {
        u64 n;
        cin >> n;
        vector<u64> factors;
        factorize(n, factors);
        sort(factors.begin(), factors.end());
        cout << factors.size();
        for (u64 p : factors) {
            cout << ' ' << p;
        }
        cout << '\n';
    }
    return 0;
}
