#include <bits/stdc++.h>

using namespace std;

long long power(long long a, long long b, long long mod) {
    long long result = 1;
    while (b) {
        if (b & 1) result = result * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return result;
}

long long inverse(long long a, long long mod) {
    if (mod == 1) return 0;
    long long b = mod, x = 1, y = 0;
    while (b) {
        long long q = a / b;
        a %= b;
        swap(a, b);
        x = (x - q * y) % mod;
        swap(x, y);
    }
    return (x + mod) % mod;
}

long long primePowerRoot(long long a, long long prime, int exponent, long long mod) {
    long long rest = mod - 1;
    int valuation = 0;
    while (rest % prime == 0) {
        rest /= prime;
        valuation++;
    }
    long long primePower = power(prime, exponent, mod);
    long long answer =
        power(a, ((primePower - 1) * inverse(rest, primePower) % primePower * rest + 1) / primePower, mod);
    long long generator = 2;
    while (power(generator, (mod - 1) / prime, mod) == 1) generator++;
    generator = power(generator, rest, mod);
    int step = sqrt(double((valuation - exponent) * prime)) + 1;
    map<long long, int> baby;
    long long value = 1;
    long long jump = power(generator, step * power(prime, valuation - 1, mod - 1) % (mod - 1), mod);
    for (int i = 0; i <= step; i++) {
        baby[value] = i;
        value = value * jump % mod;
    }
    long long backward = inverse(power(generator, power(prime, valuation - 1, mod - 1), mod), mod);
    for (int level = exponent; level < valuation; level++) {
        long long error = inverse(power(answer, primePower, mod), mod) * a % mod;
        long long target = power(error, power(prime, valuation - 1 - level, mod - 1), mod);
        for (int j = 0; j <= step; j++) {
            auto it = baby.find(target);
            if (it != baby.end()) {
                long long digit = j + 1LL * step * it->second;
                answer =
                    answer * power(generator, digit * power(prime, level - exponent, mod - 1) % (mod - 1), mod) % mod;
                break;
            }
            target = target * backward % mod;
        }
    }
    return answer;
}

long long kthRoot(long long a, long long k, long long primeMod) {
    if (a == 0 && k > 0) return 0;
    k %= primeMod - 1;
    long long divisor = gcd(k, primeMod - 1);
    if (power(a, (primeMod - 1) / divisor, primeMod) != 1) return -1;
    a = power(a, inverse(k / divisor, (primeMod - 1) / divisor), primeMod);
    for (long long p = 2; p * p <= divisor; p++) {
        int exponent = 0;
        while (divisor % p == 0) {
            divisor /= p;
            exponent++;
        }
        if (exponent) a = primePowerRoot(a, p, exponent, primeMod);
    }
    if (divisor > 1) a = primePowerRoot(a, divisor, 1, primeMod);
    return a;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int tests;
    cin >> tests;
    while (tests--) {
        long long k, a, prime;
        cin >> k >> a >> prime;
        cout << kthRoot(a, k, prime) << '\n';
    }
    return 0;
}
