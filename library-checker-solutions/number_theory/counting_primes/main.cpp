#include <bits/stdc++.h>

using namespace std;

constexpr int SIEVE_LIMIT = 5000000;
constexpr int PHI_X = 100000;
constexpr int PHI_S = 100;

vector<int> primes;
vector<int> primeCount(SIEVE_LIMIT + 1);
array<array<int, PHI_S>, PHI_X> phiTable{};
map<pair<long long, int>, long long> phiCache;
map<long long, long long> lehmerCache;

long long phi(long long x, int s) {
    if (s == 0) {
        return x;
    }
    if (s < PHI_S && x < PHI_X) {
        return phiTable[x][s];
    }
    auto key = pair{x, s};
    if (s <= 200) {
        auto it = phiCache.find(key);
        if (it != phiCache.end()) {
            return it->second;
        }
    }
    long long result = phi(x, s - 1) - phi(x / primes[s - 1], s - 1);
    if (s <= 200) {
        phiCache[key] = result;
    }
    return result;
}

long long primePi(long long x) {
    if (x < SIEVE_LIMIT) {
        return primeCount[x];
    }
    auto it = lehmerCache.find(x);
    if (it != lehmerCache.end()) {
        return it->second;
    }
    long long a = primePi(sqrt(sqrt((long double)x)));
    long long b = primePi(sqrt((long double)x));
    long long c = primePi(cbrt((long double)x));
    long long result = phi(x, a) + (b + a - 2) * (b - a + 1) / 2;
    for (long long i = a; i < b; i++) {
        long long value = x / primes[i];
        result -= primePi(value);
        if (i < c) {
            long long limit = primePi(sqrt((long double)value));
            for (long long j = i; j < limit; j++) {
                result -= primePi(value / primes[j]) - j;
            }
        }
    }
    return lehmerCache[x] = result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<bool> composite(SIEVE_LIMIT + 1);
    for (int i = 2; i <= SIEVE_LIMIT; i++) {
        if (!composite[i]) {
            primes.push_back(i);
        }
        for (int p : primes) {
            if (1LL * i * p > SIEVE_LIMIT) {
                break;
            }
            composite[i * p] = true;
            if (i % p == 0) {
                break;
            }
        }
        primeCount[i] = primes.size();
    }
    for (int x = 0; x < PHI_X; x++) {
        phiTable[x][0] = x;
    }
    for (int s = 1; s < PHI_S; s++) {
        for (int x = 0; x < PHI_X; x++) {
            phiTable[x][s] = phiTable[x][s - 1] - phiTable[x / primes[s - 1]][s - 1];
        }
    }

    long long n;
    cin >> n;
    cout << primePi(n) << '\n';
    return 0;
}
