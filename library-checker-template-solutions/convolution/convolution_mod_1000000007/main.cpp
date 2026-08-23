

#include <bits/stdc++.h>
using namespace std;
#define int long long

using C = complex<long double>;

void fft(vector<C> &a, bool inv = false)
{
    // a 是待变换序列，inv 表示是否做逆变换；原地完成 FFT，逆变换已除以长度。
    int n = a.size();
    assert(n > 0 && has_single_bit((unsigned long long)n)); // 调试检查，可删。
    int j = 0;
    for (int i = 1; i < n; i++)
    {
        int k = n >> 1;
        while (j & k)
        {
            j ^= k;
            k >>= 1;
        }
        j ^= k;
        if (i < j)
        {
            swap(a[i], a[j]);
        }
    }
    const long double pi = acosl(-1);
    for (int len = 2; len <= n; len <<= 1)
    {
        long double ang = 2 * pi / len * (inv ? -1 : 1);
        C wlen(cosl(ang), sinl(ang));
        for (int l = 0; l < n; l += len)
        {
            C w{1.0L, 0.0L};
            for (int j = 0; j < len / 2; j++)
            {
                C x = a[l + j];
                C y = a[l + j + len / 2] * w;
                a[l + j] = x + y;
                a[l + j + len / 2] = x - y;
                w *= wlen;
            }
        }
    }
    if (inv)
    {
        for (C &x : a)
        {
            x /= n;
        }
    }
}

vector<long double> convolution(const vector<long double> &a, const vector<long double> &b)
{
    // a、b 是实系数序列；返回二者的线性卷积，任一输入为空时返回空。
    if (a.empty() || b.empty())
    {
        return {};
    }
    int sz = a.size() + b.size() - 1;
    int n = bit_ceil((unsigned long long)sz);
    vector<C> x(n);
    vector<C> y(n);
    copy(a.begin(), a.end(), x.begin());
    copy(b.begin(), b.end(), y.begin());
    fft(x);
    fft(y);
    for (int i = 0; i < n; i++)
    {
        x[i] *= y[i];
    }
    fft(x, true);
    vector<long double> c(sz);
    for (int i = 0; i < sz; i++)
    {
        c[i] = x[i].real();
    }
    return c;
}

namespace arbitrary_mod_convolution
{
struct NTT
{
    uint32_t mod, root; // mod 是素数模数，root 是其原根。

    uint32_t pow(uint32_t a, uint32_t n) const
    {
        // a 是模 mod 剩余，n 是非负指数；返回 a^n mod mod。
        uint64_t ans = 1;
        while (n)
        {
            if (n & 1)
            {
                ans = ans * a % mod;
            }
            a = (uint64_t)a * a % mod;
            n >>= 1;
        }
        return ans;
    }

    void dft(vector<uint32_t> &a, bool inv) const
    {
        // a 长度是支持范围内的 2 的幂；inv 为 false/true 时原地正/逆 NTT。
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++)
        {
            int b = n >> 1;
            for (; j & b; b >>= 1)
            {
                j ^= b;
            }
            j ^= b;
            if (i < j)
            {
                swap(a[i], a[j]);
            }
        }
        for (int len = 2; len <= n; len <<= 1)
        {
            uint32_t w = pow(root, (mod - 1) / len);
            if (inv)
            {
                w = pow(w, mod - 2);
            }
            for (int l = 0; l < n; l += len)
            {
                uint64_t cur = 1;
                for (int i = 0; i < len / 2; i++)
                {
                    uint32_t x = a[l + i];
                    uint32_t y = cur * a[l + i + len / 2] % mod;
                    a[l + i] = x + y < mod ? x + y : x + y - mod;
                    a[l + i + len / 2] = x >= y ? x - y : x + mod - y;
                    cur = cur * w % mod;
                }
            }
        }
        if (inv)
        {
            uint32_t iv = pow(n, mod - 2);
            for (auto &x : a)
            {
                x = (uint64_t)x * iv % mod;
            }
        }
    }

    vector<uint32_t> conv(const vector<unsigned long long> &x,
                          const vector<unsigned long long> &y) const
    {
        // x、y 是普通整数系数；返回它们在 mod 下的线性卷积。
        int need = x.size() + y.size() - 1;
        int n = bit_ceil((unsigned long long)need);
        vector<uint32_t> a(n), b(n);
        for (int i = 0; i < (int)x.size(); i++)
        {
            a[i] = x[i] % mod;
        }
        for (int i = 0; i < (int)y.size(); i++)
        {
            b[i] = y[i] % mod;
        }
        dft(a, false);
        dft(b, false);
        for (int i = 0; i < n; i++)
        {
            a[i] = (uint64_t)a[i] * b[i] % mod;
        }
        dft(a, true);
        a.resize(need);
        return a;
    }
};


inline uint32_t inv(uint32_t a, uint32_t mod)
{
    // a 与素数 mod 互素；返回 a 在模 mod 下的逆元。
    uint64_t ans = 1, b = a;
    uint32_t e = mod - 2;
    while (e)
    {
        if (e & 1)
        {
            ans = ans * b % mod;
        }
        b = b * b % mod;
        e >>= 1;
    }
    return ans;
}
}

long long normConv(long long x, long long mod)
{
    // mod 是正模数；返回 x 在 [0,mod) 内的代表元。
    x %= mod;
    if (x < 0)
    {
        x += mod;
    }
    return x;
}

vector<long long> convNaiveMod(const vector<long long> &a, const vector<long long> &b,
                               long long mod)
{
    // a、b 已规范到 [0,mod)，mod 是正模数；用 128 位乘加返回精确模卷积，任一输入为空时返回空。
    assert(mod > 0); // 调试检查，可删。
    if (a.empty() || b.empty())
    {
        return {};
    }
    vector<long long> c(a.size() + b.size() - 1);
    for (int i = 0; i < (int)a.size(); i++)
    {
        for (int j = 0; j < (int)b.size(); j++)
        {
            c[i + j] = ((__int128)c[i + j] + (__int128)a[i] * b[j]) % mod;
        }
    }
    return c;
}

vector<long long> convNTTMod(const vector<long long> &a, const vector<long long> &b,
                             long long mod)
{
    // a、b 已规范到 [0,mod)，mod 不超过 1E9+7；用三个 NTT 素数返回精确模卷积。
    assert(0 < mod && mod <= 1000000007); // 调试检查，可删。
    if (a.empty() || b.empty())
    {
        return {};
    }
    int need = a.size() + b.size() - 1;
    int n = bit_ceil((unsigned long long)need);
    assert(n <= (1 << 20)); // 调试检查，可删；所选三个素数都支持该长度。
    constexpr array<uint32_t, 3> prime{998244353, 985661441, 943718401};
    constexpr array<uint32_t, 3> root{3, 3, 7};
    vector<unsigned long long> x(a.begin(), a.end()), y(b.begin(), b.end());
    array<vector<uint32_t>, 3> rem;
    for (int k = 0; k < 3; k++)
    {
        rem[k] = arbitrary_mod_convolution::NTT{prime[k], root[k]}.conv(x, y);
    }
    uint64_t inv01 = arbitrary_mod_convolution::inv(prime[0] % prime[1], prime[1]);
    uint64_t p01mod2 = (uint64_t)prime[0] * prime[1] % prime[2];
    uint64_t inv012 = arbitrary_mod_convolution::inv(p01mod2, prime[2]);
    uint64_t p0mod = prime[0] % mod;
    uint64_t p01mod = p0mod * (prime[1] % mod) % mod;
    vector<long long> ans(need);
    for (int i = 0; i < need; i++)
    {
        uint64_t c0 = rem[0][i];
        uint64_t c1 = (rem[1][i] + prime[1] - c0 % prime[1]) % prime[1] * inv01 % prime[1];
        uint64_t seen2 = (c0 + (uint64_t)prime[0] * c1) % prime[2];
        uint64_t c2 = (rem[2][i] + prime[2] - seen2) % prime[2] * inv012 % prime[2];
        ans[i] = (c0 % mod + p0mod * (c1 % mod) + p01mod * (c2 % mod)) % mod;
    }
    return ans;
}

vector<long long> convMod(vector<long long> a, vector<long long> b, long long mod)
{
    // a、b 是整数系数，mod 是正模数；返回最小非负代表元组成的线性卷积。
    assert(mod > 0); // 调试检查，可删。
    if (a.empty() || b.empty())
    {
        return {};
    }
    for (auto &x : a)
    {
        x = normConv(x, mod);
    }
    for (auto &x : b)
    {
        x = normConv(x, mod);
    }
    if (min(a.size(), b.size()) <= 64)
    {
        return convNaiveMod(a, b, mod);
    }
    int s = a.size() + b.size() - 1;
    int n = bit_ceil((unsigned long long)s);
    if (mod <= 1000000007 && n <= (1 << 20))
    {
        return convNTTMod(a, b, mod);
    }
    const long long B = 1 << 15;
    unsigned __int128 t = min(a.size(), b.size());
    unsigned __int128 hi = (mod - 1) / B;
    unsigned __int128 lim = (unsigned __int128)1 << 48;
    auto bounded = [&](unsigned __int128 x, unsigned __int128 y, unsigned __int128 z)
    {
        if (x && y > lim / x)
        {
            return false;
        }
        x *= y;
        return !x || z <= lim / x;
    };
    bool safe = numeric_limits<long double>::digits >= 64 && n <= (1 << 20) &&
                bounded(t, B - 1, B - 1) && bounded(t, 2 * (B - 1), hi) &&
                bounded(t, hi, hi);
    if (!safe)
    {
        return convNaiveMod(a, b, mod);
    }
    vector<long double> a0(a.size());
    vector<long double> a1(a.size());
    vector<long double> b0(b.size());
    vector<long double> b1(b.size());
    for (int i = 0; i < (int)a.size(); i++)
    {
        a0[i] = a[i] % B;
        a1[i] = a[i] / B;
    }
    for (int i = 0; i < (int)b.size(); i++)
    {
        b0[i] = b[i] % B;
        b1[i] = b[i] / B;
    }
    auto c0 = convolution(a0, b0);
    auto c1 = convolution(a0, b1);
    auto q = convolution(a1, b0);
    auto c2 = convolution(a1, b1);
    vector<long long> c(c0.size());
    for (int i = 0; i < (int)c.size(); i++)
    {
        long long x = llroundl(c0[i]) % mod;
        long long y = (llroundl(c1[i]) + llroundl(q[i])) % mod;
        long long z = llroundl(c2[i]) % mod;
        __int128 v = x;
        v += (__int128)y * B % mod;
        v += (__int128)z * B % mod * B % mod;
        c[i] = v % mod;
    }
    return c;
}

signed main(){ int n,m; cin>>n>>m; vector<long long>a(n),b(m); for(auto&x:a)cin>>x;for(auto&x:b)cin>>x;auto c=convMod(a,b,1000000007);for(int i=0;i<(int)c.size();i++)cout<<c[i]<<" \n"[i+1==(int)c.size()]; }
