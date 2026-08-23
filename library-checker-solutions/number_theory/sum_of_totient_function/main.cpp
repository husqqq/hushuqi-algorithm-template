#include <bits/stdc++.h>

using namespace std;

constexpr long long P = 998244353;
constexpr int LIMIT = 5000000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;
    int limit = min<long long>(LIMIT, n);
    vector<int> phi(limit + 1), primes;
    vector<char> composite(limit + 1);
    if (limit >= 1) phi[1] = 1;
    for (int i = 2; i <= limit; i++) {
        if (!composite[i]) {
            primes.push_back(i);
            phi[i] = i - 1;
        }
        for (int prime : primes) {
            if (1LL * i * prime > limit) break;
            composite[i * prime] = true;
            if (i % prime == 0) {
                phi[i * prime] = phi[i] * prime;
                break;
            }
            phi[i * prime] = phi[i] * (prime - 1);
        }
    }
    vector<long long> prefix(limit + 1);
    for (int i = 1; i <= limit; i++) {
        prefix[i] = (prefix[i - 1] + phi[i]) % P;
    }
    unordered_map<long long, long long> memo;
    auto solve = [&](auto&& self, long long x) -> long long {
        if (x <= limit) return prefix[x];
        auto it = memo.find(x);
        if (it != memo.end()) return it->second;
        long long answer = x % P * ((x + 1) % P) % P * ((P + 1) / 2) % P;
        for (long long l = 2, r; l <= x; l = r + 1) {
            long long quotient = x / l;
            r = x / quotient;
            answer = (answer - (r - l + 1) % P * self(self, quotient)) % P;
        }
        if (answer < 0) answer += P;
        return memo[x] = answer;
    };
    cout << solve(solve, n) << '\n';
    return 0;
}
