#include <bits/stdc++.h>

using namespace std;

using u64 = unsigned long long;
using u128 = __uint128_t;

mt19937_64 randomEngine(712367821);

u64 powerMod(u64 a, u64 b, u64 mod) {
    u64 result = 1;
    while (b) {
        if (b & 1) result = u128(result) * a % mod;
        a = u128(a) * a % mod;
        b >>= 1;
    }
    return result;
}

bool isPrime(u64 n) {
    if (n < 2) return false;
    for (u64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
        if (n % p == 0) return n == p;
    }
    int s = countr_zero(n - 1);
    u64 d = (n - 1) >> s;
    for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n == 0) continue;
        u64 x = powerMod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool witness = true;
        for (int i = 1; i < s; i++) {
            x = u128(x) * x % n;
            if (x == n - 1) {
                witness = false;
                break;
            }
        }
        if (witness) return false;
    }
    return true;
}

u64 pollardRho(u64 n) {
    if (n % 2 == 0) return 2;
    while (true) {
        u64 c = randomEngine() % (n - 1) + 1;
        u64 x = randomEngine() % (n - 2) + 2, y = x, divisor = 1;
        auto next = [&](u64 value) { return (u128(value) * value + c) % n; };
        while (divisor == 1) {
            x = next(x);
            y = next(next(y));
            divisor = gcd(x > y ? x - y : y - x, n);
        }
        if (divisor != n) return divisor;
    }
}

void factorize(u64 n, vector<u64>& factors) {
    if (n == 1) return;
    if (isPrime(n)) {
        factors.push_back(n);
        return;
    }
    u64 divisor = pollardRho(n);
    factorize(divisor, factors);
    factorize(n / divisor, factors);
}

u64 modularSqrt(u64 value, u64 prime) {
    if (prime == 2) return value;
    if (powerMod(value, (prime - 1) / 2, prime) != 1) return prime;
    if (prime % 4 == 3) return powerMod(value, (prime + 1) / 4, prime);
    u64 odd = prime - 1;
    int exponent = countr_zero(odd);
    odd >>= exponent;
    u64 nonResidue = 2;
    while (powerMod(nonResidue, (prime - 1) / 2, prime) == 1) nonResidue++;
    u64 c = powerMod(nonResidue, odd, prime);
    u64 x = powerMod(value, (odd + 1) / 2, prime);
    u64 t = powerMod(value, odd, prime);
    int m = exponent;
    while (t != 1) {
        int i = 1;
        u64 current = u128(t) * t % prime;
        while (current != 1) {
            current = u128(current) * current % prime;
            i++;
        }
        u64 b = powerMod(c, 1ULL << (m - i - 1), prime);
        x = u128(x) * b % prime;
        c = u128(b) * b % prime;
        t = u128(t) * c % prime;
        m = i;
    }
    return x;
}

pair<long long, long long> primeAsTwoSquares(u64 prime) {
    u64 root = modularSqrt(prime - 1, prime);
    for (u64 candidate : {root, prime - root}) {
        u64 a = prime, b = candidate;
        while (u128(b) * b > prime) {
            u64 remainder = a % b;
            a = b;
            b = remainder;
        }
        u64 square = prime - b * b;
        u64 c = sqrt((long double)square);
        while ((c + 1) * (c + 1) <= square) c++;
        while (c * c > square) c--;
        if (c * c == square) return {(long long)b, (long long)c};
    }
    return {0, 0};
}

struct Gaussian {
    long long x, y;
};

Gaussian multiply(Gaussian a, Gaussian b) {
    return {a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x};
}

Gaussian power(Gaussian a, int exponent) {
    Gaussian result{1, 0};
    while (exponent) {
        if (exponent & 1) result = multiply(result, a);
        a = multiply(a, a);
        exponent >>= 1;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int queries;
    cin >> queries;
    while (queries--) {
        u64 n;
        cin >> n;
        if (n == 0) {
            cout << "1\n0 0\n";
            continue;
        }
        vector<u64> factors;
        factorize(n, factors);
        sort(factors.begin(), factors.end());
        vector<pair<u64, int>> grouped;
        for (u64 prime : factors) {
            if (grouped.empty() || grouped.back().first != prime) grouped.push_back({prime, 0});
            grouped.back().second++;
        }
        bool possible = true;
        long long scalar = 1;
        Gaussian initial{1, 0};
        vector<pair<pair<long long, long long>, int>> splitPrimes;
        for (auto [prime, exponent] : grouped) {
            if (prime == 2) {
                for (int i = 0; i < exponent / 2; i++) scalar *= 2;
                if (exponent & 1) initial = multiply(initial, {1, 1});
            } else if (prime % 4 == 3) {
                if (exponent & 1) possible = false;
                for (int i = 0; i < exponent / 2; i++) scalar *= prime;
            } else {
                splitPrimes.push_back({primeAsTwoSquares(prime), exponent});
            }
        }
        if (!possible) {
            cout << 0 << '\n';
            continue;
        }
        initial.x *= scalar;
        initial.y *= scalar;
        vector<Gaussian> representations{initial};
        for (auto [parts, exponent] : splitPrimes) {
            Gaussian value{parts.first, parts.second};
            Gaussian conjugate{parts.first, -parts.second};
            vector<Gaussian> next;
            for (auto current : representations) {
                for (int count = 0; count <= exponent; count++) {
                    auto factor = multiply(power(value, count), power(conjugate, exponent - count));
                    next.push_back(multiply(current, factor));
                }
            }
            representations = move(next);
        }
        set<pair<long long, long long>> answer;
        for (auto value : representations) {
            long long x = abs(value.x), y = abs(value.y);
            answer.insert({x, y});
            answer.insert({y, x});
        }
        cout << answer.size() << '\n';
        for (auto [x, y] : answer) cout << x << ' ' << y << '\n';
    }
    return 0;
}
