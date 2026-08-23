#include <bits/stdc++.h>

using namespace std;

long long phi(long long n) {
    long long result = n;
    for (long long p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            result = result / p * (p - 1);
            while (n % p == 0) {
                n /= p;
            }
        }
    }
    if (n > 1) {
        result = result / n * (n - 1);
    }
    return result;
}

long long power(long long a, long long b, long long mod, bool& large) {
    if (b == 0 || a == 1) {
        return 1;
    }
    unsigned long long result = 1;
    unsigned long long base = a;
    while (b) {
        if (b & 1) {
            result *= base;
            if (result >= (unsigned long long)mod) {
                large = true;
                result %= mod;
            }
        }
        b >>= 1;
        if (!b) {
            break;
        }
        base *= base;
        if (base >= (unsigned long long)mod) {
            large = true;
            base %= mod;
        }
    }
    return result;
}

long long tetration(long long a, long long height, long long mod, bool& large) {
    if (a == 0) {
        return (~height & 1);
    }
    if (mod == 1) {
        large = true;
        return 1;
    }
    if (height == 0 || a == 1) {
        return 1;
    }
    if (height == 1) {
        large |= a >= mod;
        return a % mod + large * mod;
    }
    long long exponent = tetration(a, height - 1, phi(mod), large);
    long long result = power(a, exponent, mod, large);
    return result + large * mod;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        long long a, height, mod;
        cin >> a >> height >> mod;
        bool large = false;
        cout << tetration(a, height, mod, large) % mod << '\n';
    }
    return 0;
}
