// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: EX1 Kth 1

#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = numeric_limits<long long>::max() / 4;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    if (b < a)
    {
        a = b;
        return true;
    }
    return false;
}

template <class T> bool chmax(T &a, const T &b)
{
    if (a < b)
    {
        a = b;
        return true;
    }
    return false;
}

long long mulMod(long long a, long long b, long long m)
{
    // a、b 是乘数，m 是正模数；返回 a*b mod m 的最小非负余数。
    assert(m > 0); // 调试检查，可删
    a %= m;
    b %= m;
    if (a < 0)
    {
        a += m;
    }
    if (b < 0)
    {
        b += m;
    }
    constexpr long long limit = 3037000499LL;
    if (m <= limit)
    {
        return a * b % m;
    }
    return (__int128)a * b % m;
}

long long powerMod(long long a, unsigned long long b, long long m)
{
    // a 是可为负的底数，b 是非负指数，m 是正模数；返回 a^b mod m。
    assert(m > 0); // 调试检查，可删
    long long r = 1 % m;
    a %= m;
    if (a < 0)
    {
        a += m;
    }
    while (b)
    {
        if (b & 1)
        {
            r = mulMod(r, a, m);
        }
        a = mulMod(a, a, m);
        b >>= 1;
    }
    return r;
}

unsigned long long magnitude(long long x)
{
    // 返回 x 的无符号幅值，最小负数也可表示。
    auto u = (unsigned long long)x;
    return x < 0 ? 0ULL - u : u;
}

unsigned long long binaryGcd(long long a, long long b)
{
    // 返回 gcd(|a|,|b|)，约定 gcd(0,0)=0。
    auto x = magnitude(a);
    auto y = magnitude(b);
    if (x == 0)
    {
        return y;
    }
    if (y == 0)
    {
        return x;
    }
    int z = countr_zero(x | y);
    x >>= countr_zero(x);
    do
    {
        y >>= countr_zero(y);
        if (x > y)
        {
            swap(x, y);
        }
        y -= x;
    } while (y != 0);
    return x << z;
}

optional<unsigned long long> safeLcm(long long a, long long b)
{
    // 返回非负 lcm(|a|,|b|)，超过无符号 64 位范围时返回空。
    if (a == 0 || b == 0)
    {
        return 0;
    }
    auto x = magnitude(a);
    auto y = magnitude(b);
    auto g = gcd(x, y);
    unsigned __int128 v = (unsigned __int128)(x / g) * y;
    if (v > numeric_limits<unsigned long long>::max())
    {
        return nullopt;
    }
    return (unsigned long long)v;
}

using Exgcd = array<__int128, 3>; // [0]=gcd(a,b)，[1]=x，[2]=y，后二者是一组 Bézout 系数

Exgcd exgcd(long long a, long long b)
{
    // a、b 是原方程系数；返回 {gcd(a,b),x,y}，满足 a*x+b*y=gcd(a,b)。
    __int128 r0 = a;
    __int128 r1 = b;
    __int128 x0 = 1;
    __int128 x1 = 0;
    __int128 y0 = 0;
    __int128 y1 = 1;
    while (r1)
    {
        __int128 q = r0 / r1;
        __int128 nr = r0 - q * r1;
        __int128 nx = x0 - q * x1;
        __int128 ny = y0 - q * y1;
        r0 = r1;
        r1 = nr;
        x0 = x1;
        x1 = nx;
        y0 = y1;
        y1 = ny;
    }
    if (r0 < 0)
    {
        r0 = -r0;
        x0 = -x0;
        y0 = -y0;
    }
    return {r0, x0, y0};
}

optional<long long> invMod(long long a, long long m)
{
    // a 是待求逆整数，m 是大于 1 的模数；返回最小非负逆元，不可逆时返回空。
    assert(m > 1); // 调试检查，可删
    auto [g, x, y] = exgcd(a, m);
    if (g != 1)
    {
        return nullopt;
    }
    x %= m;
    if (x < 0)
    {
        x += m;
    }
    return (long long)x;
}

using Cong = array<int, 2>; // [0]=最小非负余数，[1]=正周期

optional<Cong> congruence(long long a, long long b, long long m)
{
    // 解 ax=b (mod m)，返回 {最小非负解,周期}；无解返回空。
    assert(m > 0); // 调试检查，可删
    long long g = (long long)binaryGcd(a, m);
    if (b % g)
    {
        return nullopt;
    }
    long long q = m / g;
    if (q == 1)
    {
        return Cong{0, 1};
    }
    long long r = mulMod(b / g, *invMod(a / g, q), q);
    return Cong{r, q};
}

optional<Cong> crt(Cong a, Cong b)
{
    // 合并 x≡a[0] (mod a[1]) 与 x≡b[0] (mod b[1])；返回 {最小非负解,lcm 模}，无解或新模越界时返回空。
    assert(a[1] > 0 && b[1] > 0); // 调试检查，可删
    auto [g, x, y] = exgcd(a[1], b[1]);
    __int128 d = (__int128)b[0] - a[0];
    if (d % g)
    {
        return nullopt;
    }
    long long q = (long long)((__int128)b[1] / g);
    __int128 k = 0;
    if (q > 1)
    {
        k = d / g * x % q;
        if (k < 0)
        {
            k += q;
        }
    }
    __int128 m = (__int128)a[1] / g * b[1];
    if (m > numeric_limits<long long>::max())
    {
        return nullopt;
    }
    __int128 r = ((__int128)a[1] * k + a[0]) % m;
    if (r < 0)
    {
        r += m;
    }
    return Cong{(long long)r, (long long)m};
}

int invPrime(int a, int mod)
{
    // 返回非零剩余 a 在素数模 mod 下的逆元。
    assert(0 < a && a < mod); // 调试检查，可删
    int ans = 1;
    int b = mod - 2;
    while (b)
    {
        if (b & 1)
        {
            ans = mulMod(ans, a, mod);
        }
        a = mulMod(a, a, mod);
        b >>= 1;
    }
    return ans;
}

vector<int> invTable(int n, int mod)
{
    // 返回 1..n 在素数模 mod 下的逆元表，位置 0 保持 0。
    assert(0 <= n && n < mod); // 调试检查，可删
    vector<int> inv(n + 1);
    if (n >= 1)
    {
        inv[1] = 1;
    }
    for (int i = 2; i <= n; i++)
    {
        inv[i] = mod - mulMod(mod / i, inv[mod % i], mod);
    }
    return inv;
}

vector<int> batchInv(const vector<int> &a, int mod)
{
    // 返回 a 中每个非零剩余在素数模 mod 下的逆元。
    int n = a.size();
    vector<int> pre(n + 1, 1);
    for (int i = 0; i < n; i++)
    {
        assert(0 < a[i] && a[i] < mod); // 调试检查，可删
        pre[i + 1] = mulMod(pre[i], a[i], mod);
    }
    int cur = invPrime(pre[n], mod);
    vector<int> ans(n);
    for (int i = n - 1; i >= 0; i--)
    {
        ans[i] = mulMod(cur, pre[i], mod);
        cur = mulMod(cur, a[i], mod);
    }
    return ans;
}
