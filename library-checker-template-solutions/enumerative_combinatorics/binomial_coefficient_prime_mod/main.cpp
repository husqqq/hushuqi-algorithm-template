
#include <bits/stdc++.h>

inline long long combMulMod(long long a, long long b, long long mod)
{
    // mod>0 且 a、b 均为 [0,mod)；小模数直接乘，大模数才使用局部宽乘法。
    constexpr long long limit = 3037000499LL;
    if (mod <= limit)
    {
        return a * b % mod;
    }
    return (long long)((__int128)a * b % mod);
}
using namespace std;
#define int long long

int powPrime(int a, int b, int p)
{
    // a 是底数，b 是非负指数，p 是素数；返回 a^b mod p。
    assert(b >= 0 && p >= 2); // 调试检查，可删。
    a %= p;
    if (a < 0)
    {
        a += p;
    }
    int ans = 1 % p;
    while (b)
    {
        if (b & 1)
        {
            ans = combMulMod(ans, a, p);
        }
        a = combMulMod(a, a, p);
        b >>= 1;
    }
    return ans;
}

int lucas(int n, int k, int p)
{
    // n、k 是组合数参数，p 是素数；返回 C(n,k) mod p。
    assert(n >= 0 && p >= 2); // 调试检查，可删。
    if (k < 0 || k > n)
    {
        return 0;
    }
    vector<int> fac(p, 1), ifac(p, 1);
    for (int i = 1; i < p; i++)
    {
        fac[i] = combMulMod(fac[i - 1], i, p);
    }
    ifac[p - 1] = powPrime(fac[p - 1], p - 2, p);
    for (int i = p - 1; i; i--)
    {
        ifac[i - 1] = combMulMod(ifac[i], i, p);
    }
    int ans = 1;
    while (n || k)
    {
        int a = n % p;
        int b = k % p;
        if (b > a)
        {
            return 0;
        }
        ans = combMulMod(combMulMod(ans, fac[a], p), ifac[b], p);
        ans = combMulMod(ans, ifac[a - b], p);
        n /= p;
        k /= p;
    }
    return ans;
}

class PrimeBinom
{
    int p;
    vector<int> fac, ifac;

  public:
    PrimeBinom(int p, int maxN) : p(p)
    {
        // p 是素数，maxN 是本批查询最大 n；预处理 Lucas 所需低位阶乘。
        assert(p >= 2 && maxN >= 0);
        int n = min(p - 1, maxN);
        fac.assign(n + 1, 1);
        ifac.assign(n + 1, 1);
        for (int i = 1; i <= n; i++) fac[i] = combMulMod(fac[i - 1], i, p);
        ifac[n] = powPrime(fac[n], p - 2, p);
        for (int i = n; i; i--) ifac[i - 1] = combMulMod(ifac[i], i, p);
    }

    int C(int n, int k) const
    {
        // n、k 是组合数参数；返回 C(n,k) mod p。
        if (k < 0 || k > n) return 0;
        int ans = 1;
        while (n || k)
        {
            int a = n % p, b = k % p;
            if (b > a) return 0;
            assert(a < (int)fac.size());
            ans = combMulMod(ans, combMulMod(fac[a], combMulMod(ifac[b], ifac[a - b], p), p), p);
            n /= p;
            k /= p;
        }
        return ans;
    }
};

signed main()
{
    int t, p; cin >> t >> p;
    vector<pair<int, int>> q(t);
    int mx = 0;
    for (auto &[n, k] : q)
    {
        cin >> n >> k;
        mx = max(mx, n);
    }
    PrimeBinom comb(p, mx);
    for (auto [n, k] : q) cout << comb.C(n, k) << '\n';
}
