#include <bits/stdc++.h>
using namespace std;
#define int long long

unsigned long long mul64(unsigned long long a, unsigned long long b,
                         unsigned long long mod)
{
    // a、b 是乘数，mod 是正模数；返回 a*b mod mod。
    assert(mod > 0); // 调试检查，可删
    return (unsigned __int128)a * b % mod;
}

unsigned long long pow64(unsigned long long a, unsigned long long b,
                         unsigned long long mod)
{
    // 返回 a^b 对正模数 mod 的最小非负余数。
    assert(mod > 0); // 调试检查，可删
    unsigned long long ans = 1 % mod;
    a %= mod;
    while (b)
    {
        if (b & 1)
        {
            ans = mul64(ans, a, mod);
        }
        a = mul64(a, a, mod);
        b >>= 1;
    }
    return ans;
}

bool isPrime(unsigned long long n)
{
    // 返回 n 是否为质数；本底数集合在完整无符号 64 位范围内确定。
    if (n < 2)
    {
        return false;
    }
    for (auto p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
                   23ULL, 29ULL, 31ULL, 37ULL})
    {
        if (n % p == 0)
        {
            return n == p;
        }
    }
    int s = countr_zero(n - 1);
    auto d = (n - 1) >> s;
    for (auto a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL,
                   1795265022ULL})
    {
        if (a % n == 0)
        {
            continue;
        }
        auto x = pow64(a % n, d, n);
        if (x == 1 || x == n - 1)
        {
            continue;
        }
        bool ok = false;
        for (int r = 1; r < s; r++)
        {
            x = mul64(x, x, n);
            if (x == n - 1)
            {
                ok = true;
                break;
            }
        }
        if (!ok)
        {
            return false;
        }
    }
    return true;
}

unsigned long long pollard(unsigned long long n)
{
    // n 是大于 1 的合数；返回 n 的一个非平凡因子。
    if (n % 2 == 0)
    {
        return 2;
    }
    static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    while (true)
    {
        auto c = rng() % (n - 1) + 1;
        auto x = rng() % n;
        auto y = x;
        auto d = 1ULL;
        auto f = [&](unsigned long long z)
        {
            return (unsigned long long)(((unsigned __int128)mul64(z, z, n) + c) % n);
        };
        while (d == 1)
        {
            x = f(x);
            y = f(f(y));
            d = gcd(x > y ? x - y : y - x, n);
        }
        if (d != n)
        {
            return d;
        }
    }
}

vector<unsigned long long> factor64(unsigned long long n)
{
    // n 是正整数；返回升序质因子表并保留重数，n=1 返回空表。
    assert(n > 0); // 调试检查，可删
    vector<unsigned long long> ans;
    auto dfs = [&](auto &&self, unsigned long long x) -> void
    {
        if (x == 1)
        {
            return;
        }
        if (isPrime(x))
        {
            ans.push_back(x);
            return;
        }
        auto d = pollard(x);
        self(self, d);
        self(self, x / d);
    };
    dfs(dfs, n);
    sort(ans.begin(), ans.end());
    return ans;
}

signed main()
{
    int q; cin >> q;
    while (q--)
    {
        unsigned long long n; cin >> n;
        auto a = factor64(n);
        cout << a.size();
        for (auto p : a) cout << ' ' << p;
        cout << '\n';
    }
}
