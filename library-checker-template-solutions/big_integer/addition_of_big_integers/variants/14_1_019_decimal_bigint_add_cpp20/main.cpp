


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

namespace arbModConv
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

// LC_GENERATOR_CH14_ONLY_BEGIN
#undef int
template <uint32_t mod, uint32_t primitive> struct StaticNTT
{
    static constexpr int level = countr_zero(mod - 1);
    static constexpr uint32_t montInv()
    {
        uint32_t x = mod;
        for (int i = 0; i < 4; i++) x *= 2 - mod * x;
        return x;
    }
    static constexpr uint32_t inv32 = montInv();
    static constexpr uint32_t r2 = -uint64_t(mod) % mod;
    static inline array<uint32_t, 32> root, iroot, rate2, irate2, rate3, irate3, dw, dy;
    static inline bool ready = false;

    static uint32_t reduce(uint64_t x)
    {
        // x 小于 mod^2；返回其 Montgomery 约减结果。
        uint64_t t = (uint32_t)x * (uint32_t)(-inv32);
        return (x + t * mod) >> 32;
    }
    static uint32_t toMont(uint32_t x) { return reduce((uint64_t)x * r2); }
    static uint32_t fromMont(uint32_t x)
    {
        uint32_t v = reduce(x);
        return v >= mod ? v - mod : v;
    }
    static uint32_t mul(uint32_t x, uint32_t y) { return reduce((uint64_t)x * y); }
    static uint32_t add(uint32_t x, uint32_t y)
    {
        uint32_t v = x + y - 2 * mod;
        return (int32_t)v < 0 ? v + 2 * mod : v;
    }
    static uint32_t sub(uint32_t x, uint32_t y)
    {
        uint32_t v = x - y;
        return (int32_t)v < 0 ? v + 2 * mod : v;
    }

    static uint32_t power(uint32_t a, uint32_t e)
    {
        // a 是模 mod 剩余，e 是非负指数；返回 a^e mod mod。
        uint64_t r = 1;
        while (e)
        {
            if (e & 1) r = r * a % mod;
            a = (uint64_t)a * a % mod;
            e >>= 1;
        }
        return r;
    }

    static void init()
    {
        // 无参数；首次调用时建立 radix-2/radix-4 跨块旋转因子。
        if (ready) return;
        ready = true;
        root[level] = toMont(power(primitive, (mod - 1) >> level));
        iroot[level] = toMont(power(fromMont(root[level]), mod - 2));
        for (int i = level - 1; i >= 0; i--)
        {
            root[i] = mul(root[i + 1], root[i + 1]);
            iroot[i] = mul(iroot[i + 1], iroot[i + 1]);
        }
        uint32_t x = toMont(1), y = toMont(1);
        for (int i = 0; i + 2 <= level; i++)
        {
            rate2[i] = mul(root[i + 2], x);
            irate2[i] = mul(iroot[i + 2], y);
            x = mul(x, iroot[i + 2]);
            y = mul(y, root[i + 2]);
        }
        x = y = toMont(1);
        for (int i = 0; i + 3 <= level; i++)
        {
            rate3[i] = mul(root[i + 3], x);
            irate3[i] = mul(iroot[i + 3], y);
            x = mul(x, iroot[i + 3]);
            y = mul(y, root[i + 3]);
        }
        dw[1] = root[2], dy[1] = iroot[2];
        dw[2] = root[3], dy[2] = iroot[3];
        for (int i = 3; i < level; i++)
        {
            dw[i] = mul(mul(dw[i - 1], iroot[i]), root[i + 1]);
            dy[i] = mul(mul(dy[i - 1], root[i]), iroot[i + 1]);
        }
    }

    static void dft(vector<uint32_t> &a)
    {
        // a 的长度是 mod-1 所支持的 2 的幂；原地完成 radix-4 正变换。
        init();
        int n = a.size(), h = countr_zero((uint32_t)n), len = 0;
        assert(n > 0 && has_single_bit((unsigned long long)n) && h <= level);
        while (len < h)
        {
            if (h - len == 1)
            {
                int p = 1 << (h - len - 1);
                uint32_t rot = toMont(1);
                for (int s = 0; s < (1 << len); s++)
                {
                    int off = s << (h - len);
                    for (int i = 0; i < p; i++)
                    {
                        uint32_t l = a[off + i], r = mul(a[off + i + p], rot);
                        a[off + i] = add(l, r);
                        a[off + i + p] = sub(l, r);
                    }
                    if (s + 1 != (1 << len)) rot = mul(rot, rate2[countr_zero((uint32_t)~s)]);
                }
                len++;
            }
            else
            {
                int p = 1 << (h - len - 2);
                uint32_t rot = toMont(1), imag = root[2];
                for (int s = 0; s < (1 << len); s++)
                {
                    uint32_t rot2 = mul(rot, rot);
                    uint32_t rot3 = mul(rot2, rot);
                    int off = s << (h - len);
                    for (int i = 0; i < p; i++)
                    {
                        uint32_t a0 = a[off + i];
                        uint32_t a1 = mul(a[off + i + p], rot);
                        uint32_t a2 = mul(a[off + i + 2 * p], rot2);
                        uint32_t a3 = mul(a[off + i + 3 * p], rot3);
                        uint32_t t0 = add(a0, a2), t1 = add(a1, a3);
                        uint32_t x = mul(sub(a1, a3), imag), y = sub(a0, a2);
                        a[off + i] = add(t0, t1);
                        a[off + i + p] = sub(t0, t1);
                        a[off + i + 2 * p] = add(y, x);
                        a[off + i + 3 * p] = sub(y, x);
                    }
                    if (s + 1 != (1 << len)) rot = mul(rot, rate3[countr_zero((uint32_t)~s)]);
                }
                len += 2;
            }
        }
    }

    static void idft(vector<uint32_t> &a)
    {
        // a 来自 dft；原地完成逆变换并除以序列长度。
        init();
        int n = a.size(), h = countr_zero((uint32_t)n), len = h;
        assert(n > 0 && has_single_bit((unsigned long long)n) && h <= level);
        while (len)
        {
            if (len == 1)
            {
                int p = 1 << (h - len);
                uint32_t rot = toMont(1);
                for (int s = 0; s < (1 << (len - 1)); s++)
                {
                    int off = s << (h - len + 1);
                    for (int i = 0; i < p; i++)
                    {
                        uint32_t l = a[off + i], r = a[off + i + p];
                        a[off + i] = add(l, r);
                        a[off + i + p] = mul(sub(l, r), rot);
                    }
                    if (s + 1 != (1 << (len - 1))) rot = mul(rot, irate2[countr_zero((uint32_t)~s)]);
                }
                len--;
            }
            else
            {
                int p = 1 << (h - len);
                uint32_t rot = toMont(1), imag = iroot[2];
                for (int s = 0; s < (1 << (len - 2)); s++)
                {
                    uint32_t rot2 = mul(rot, rot);
                    uint32_t rot3 = mul(rot2, rot);
                    int off = s << (h - len + 2);
                    for (int i = 0; i < p; i++)
                    {
                        uint32_t a0 = a[off + i], a1 = a[off + i + p];
                        uint32_t a2 = a[off + i + 2 * p], a3 = a[off + i + 3 * p];
                        uint32_t t0 = add(a0, a1), t1 = add(a2, a3);
                        uint32_t x = mul(sub(a2, a3), imag), y = sub(a0, a1);
                        a[off + i] = add(t0, t1);
                        a[off + i + p] = mul(add(y, x), rot);
                        a[off + i + 2 * p] = mul(sub(t0, t1), rot2);
                        a[off + i + 3 * p] = mul(sub(y, x), rot3);
                    }
                    if (s + 1 != (1 << (len - 2))) rot = mul(rot, irate3[countr_zero((uint32_t)~s)]);
                }
                len -= 2;
            }
        }
        uint32_t iv = toMont(power(n, mod - 2));
        for (auto &x : a) x = fromMont(mul(x, iv));
    }

    static void dftCompact(vector<uint32_t> &a)
    {
        // a 以 Montgomery 形式保存；按连续 radix-4 块原地完成正变换。
        init();
        int n = a.size(), k = countr_zero((uint32_t)n);
        if (n <= 1) return;
        if (k == 1)
        {
            uint32_t x = a[1];
            a[1] = sub(a[0], x);
            a[0] = add(a[0], x);
            return;
        }
        if (k & 1)
        {
            int v = 1 << (k - 1);
            for (int i = 0; i < v; i++)
            {
                uint32_t x = a[i + v];
                a[i + v] = sub(a[i], x);
                a[i] = add(a[i], x);
            }
        }
        int u = 1 << (2 + (k & 1)), v = 1 << (k - 2 - (k & 1));
        uint32_t one = toMont(1), imag = dw[1];
        while (v)
        {
            for (int i = 0; i < v; i++)
            {
                uint32_t a0 = a[i], a1 = a[i + v], a2 = a[i + 2 * v], a3 = a[i + 3 * v];
                uint32_t t0 = add(a0, a2), t1 = add(a1, a3);
                uint32_t x = mul(sub(a1, a3), imag), y = sub(a0, a2);
                a[i] = add(t0, t1);
                a[i + v] = sub(t0, t1);
                a[i + 2 * v] = add(y, x);
                a[i + 3 * v] = sub(y, x);
            }
            uint32_t xx = mul(one, dw[2]);
            for (int jh = 4; jh < u;)
            {
                uint32_t ww = mul(xx, xx), wx = mul(ww, xx);
                int l = jh * v, r = l + v;
                for (int i = l; i < r; i++)
                {
                    uint32_t a0 = a[i], a1 = mul(a[i + v], xx);
                    uint32_t a2 = mul(a[i + 2 * v], ww), a3 = mul(a[i + 3 * v], wx);
                    uint32_t t0 = add(a0, a2), t1 = add(a1, a3);
                    uint32_t x = mul(sub(a1, a3), imag), y = sub(a0, a2);
                    a[i] = add(t0, t1);
                    a[i + v] = sub(t0, t1);
                    a[i + 2 * v] = add(y, x);
                    a[i + 3 * v] = sub(y, x);
                }
                xx = mul(xx, dw[countr_zero((uint32_t)(jh += 4))]);
            }
            u <<= 2;
            v >>= 2;
        }
    }

    static void idftCompact(vector<uint32_t> &a)
    {
        // a 来自 dftCompact；原地逆变换、除以长度并转回普通剩余。
        init();
        int n = a.size(), k = countr_zero((uint32_t)n);
        if (n <= 1)
        {
            if (n == 1) a[0] = fromMont(a[0]);
            return;
        }
        if (k == 1)
        {
            uint32_t x = a[1];
            a[1] = sub(a[0], x);
            a[0] = add(a[0], x);
        }
        else
        {
            int u = 1 << (k - 2), v = 1;
            uint32_t one = toMont(1), imag = dy[1];
            while (u)
            {
                for (int i = 0; i < v; i++)
                {
                    uint32_t a0 = a[i], a1 = a[i + v], a2 = a[i + 2 * v], a3 = a[i + 3 * v];
                    uint32_t t0 = add(a0, a1), t1 = add(a2, a3);
                    uint32_t x = mul(sub(a2, a3), imag), y = sub(a0, a1);
                    a[i] = add(t0, t1);
                    a[i + v] = add(y, x);
                    a[i + 2 * v] = sub(t0, t1);
                    a[i + 3 * v] = sub(y, x);
                }
                u <<= 2;
                uint32_t xx = mul(one, dy[2]);
                for (int jh = 4; jh < u;)
                {
                    uint32_t ww = mul(xx, xx), yy = mul(xx, imag);
                    int l = jh * v, r = l + v;
                    for (int i = l; i < r; i++)
                    {
                        uint32_t a0 = a[i], a1 = a[i + v], a2 = a[i + 2 * v], a3 = a[i + 3 * v];
                        uint32_t t0 = add(a0, a1), t1 = add(a2, a3);
                        uint32_t x = mul(sub(a2, a3), yy), y = mul(sub(a0, a1), xx);
                        a[i] = add(t0, t1);
                        a[i + v] = add(y, x);
                        a[i + 2 * v] = mul(sub(t0, t1), ww);
                        a[i + 3 * v] = mul(sub(y, x), ww);
                    }
                    xx = mul(xx, dy[countr_zero((uint32_t)(jh += 4))]);
                }
                u >>= 4;
                v <<= 2;
            }
            if (k & 1)
            {
                int u2 = 1 << (k - 1);
                for (int i = 0; i < u2; i++)
                {
                    uint32_t x = a[i + u2];
                    a[i + u2] = sub(a[i], x);
                    a[i] = add(a[i], x);
                }
            }
        }
        uint32_t iv = toMont(power(n, mod - 2));
        for (auto &x : a) x = fromMont(mul(x, iv));
    }

    static vector<uint32_t> conv(const vector<unsigned long long> &x,
                                 const vector<unsigned long long> &y)
    {
        // x、y 是普通整数系数；返回二者在 mod 下的线性卷积。
        if (x.empty() || y.empty()) return {};
        int need = x.size() + y.size() - 1;
        int n = bit_ceil((unsigned long long)need);
        vector<uint32_t> a(n), b(n);
        for (int i = 0; i < (int)x.size(); i++) a[i] = toMont(x[i] % mod);
        for (int i = 0; i < (int)y.size(); i++) b[i] = toMont(y[i] % mod);
        dft(a);
        dft(b);
        for (int i = 0; i < n; i++) a[i] = mul(a[i], b[i]);
        idft(a);
        a.resize(need);
        return a;
    }
};
#define int long long

// LC_GENERATOR_CH14_ONLY_END
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
        rem[k] = arbModConv::NTT{prime[k], root[k]}.conv(x, y);
    }
    uint64_t inv01 = arbModConv::inv(prime[0] % prime[1], prime[1]);
    uint64_t p01mod2 = (uint64_t)prime[0] * prime[1] % prime[2];
    uint64_t inv012 = arbModConv::inv(p01mod2, prime[2]);
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
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct HexBigInt
{
    using It = vector<uint32_t>::iterator;
    using CIt = vector<uint32_t>::const_iterator;

    bool neg = false;
    vector<uint32_t> a;

    HexBigInt() = default;

    HexBigInt(long long x)
    {
        neg = x < 0;
        uint64_t y = neg ? 0 - (uint64_t)x : (uint64_t)x;
        while (y)
        {
            a.push_back((uint32_t)y);
            y >>= 32;
        }
    }

    HexBigInt(const string &s)
    {
        assign(s);
    }

    HexBigInt(bool sgn, vector<uint32_t> v) : neg(sgn), a(move(v))
    {
        trim(this->a);
        if (this->a.empty())
        {
            this->neg = false;
        }
    }

    HexBigInt &assign(const string &s)
    {
        assert(!s.empty());
        int p = 0;
        neg = s[0] == '-';
        if (s[0] == '-' || s[0] == '+')
        {
            p++;
        }
        assert(p < (int)s.size());
        a.clear();
        for (int r = s.size(); r > p; r -= 8)
        {
            int l = max(p, r - 8);
            uint32_t x = 0;
            for (int i = l; i < r; i++)
            {
                x <<= 4;
                if ('0' <= s[i] && s[i] <= '9')
                {
                    x |= s[i] - '0';
                }
                else if ('A' <= s[i] && s[i] <= 'F')
                {
                    x |= s[i] - 'A' + 10;
                }
                else
                {
                    assert('a' <= s[i] && s[i] <= 'f');
                    x |= s[i] - 'a' + 10;
                }
            }
            a.push_back(x);
        }
        trim(a);
        if (a.empty())
        {
            neg = false;
        }
        return *this;
    }

    string toHex() const
    {
        if (a.empty())
        {
            return "0";
        }
        const char *digit = "0123456789ABCDEF";
        string s = neg ? "-" : "";
        for (int i = a.size() - 1; i >= 0; i--)
        {
            string t(8, '0');
            uint32_t x = a[i];
            for (int j = 7; j >= 0; j--)
            {
                t[j] = digit[x & 15];
                x >>= 4;
            }
            if (i == (int)a.size() - 1)
            {
                t.erase(0, t.find_first_not_of('0'));
            }
            s += t;
        }
        return s;
    }

    bool isZero() const
    {
        return a.empty();
    }

    HexBigInt operator-() const
    {
        HexBigInt x = *this;
        if (!x.a.empty())
        {
            x.neg = !x.neg;
        }
        return x;
    }

    friend bool operator==(const HexBigInt &x, const HexBigInt &y)
    {
        return x.neg == y.neg && x.a == y.a;
    }

    friend bool operator<(const HexBigInt &x, const HexBigInt &y)
    {
        if (x.neg != y.neg)
        {
            return x.neg;
        }
        int c = cmp(x.a, y.a);
        return x.neg ? c > 0 : c < 0;
    }

    friend bool operator!=(const HexBigInt &x, const HexBigInt &y)
    {
        return !(x == y);
    }

    friend bool operator>(const HexBigInt &x, const HexBigInt &y)
    {
        return y < x;
    }

    friend bool operator<=(const HexBigInt &x, const HexBigInt &y)
    {
        return !(y < x);
    }

    friend bool operator>=(const HexBigInt &x, const HexBigInt &y)
    {
        return !(x < y);
    }

    friend HexBigInt operator+(const HexBigInt &x, const HexBigInt &y)
    {
        if (x.neg == y.neg)
        {
            return {x.neg, addVec(x.a, y.a)};
        }
        int c = cmp(x.a, y.a);
        if (c <= 0)
        {
            return {c ? y.neg : false, subVec(y.a, x.a)};
        }
        return {x.neg, subVec(x.a, y.a)};
    }

    friend HexBigInt operator-(const HexBigInt &x, const HexBigInt &y)
    {
        return x + (-y);
    }

    friend HexBigInt operator*(const HexBigInt &x, const HexBigInt &y)
    {
        auto z = mulVec(x.a, y.a);
        return {z.empty() ? false : x.neg != y.neg, move(z)};
    }

    friend pair<HexBigInt, HexBigInt> divmod(const HexBigInt &x, const HexBigInt &y)
    {
        assert(!y.a.empty());
        auto [q, r] = divVec(x.a, y.a);
        return {{q.empty() ? false : x.neg != y.neg, move(q)},
                {r.empty() ? false : x.neg, move(r)}};
    }

    friend HexBigInt operator/(const HexBigInt &x, const HexBigInt &y)
    {
        return divmod(x, y).first;
    }

    friend HexBigInt operator%(const HexBigInt &x, const HexBigInt &y)
    {
        return divmod(x, y).second;
    }

    HexBigInt &operator+=(const HexBigInt &x)
    {
        return *this = *this + x;
    }

    HexBigInt &operator-=(const HexBigInt &x)
    {
        return *this = *this - x;
    }

    HexBigInt &operator*=(const HexBigInt &x)
    {
        return *this = *this * x;
    }

    HexBigInt &operator/=(const HexBigInt &x)
    {
        return *this = *this / x;
    }

    HexBigInt &operator%=(const HexBigInt &x)
    {
        return *this = *this % x;
    }

    friend ostream &operator<<(ostream &out, const HexBigInt &x)
    {
        return out << x.toHex();
    }

    static HexBigInt fromDecimal(const string &s)
    {
        // s 是带可选正负号的十进制整数串；按 10^9 分组并分治合并到二进制 limb。
        assert(!s.empty());
        int p = 0;
        bool sign = s[0] == '-';
        if (s[0] == '-' || s[0] == '+') p++;
        assert(p < (int)s.size());
        vector<uint32_t> group;
        for (int r = s.size(); r > p; r -= 9)
        {
            int l = max<int>(p, r - 9);
            uint32_t x = 0;
            for (int i = l; i < r; i++)
            {
                assert('0' <= s[i] && s[i] <= '9');
                x = x * 10 + s[i] - '0';
            }
            group.push_back(x);
        }
        int z = bit_ceil((unsigned long long)max<int>(1, group.size()));
        group.resize(z);
        vector<HexBigInt> power;
        power.emplace_back(1000000000LL);
        for (int n = 2; n < z; n <<= 1) power.push_back(power.back() * power.back());
        auto build = [&](auto &&self, int l, int len, int level) -> HexBigInt
        {
            if (len == 1) return HexBigInt((long long)group[l]);
            int half = len / 2;
            HexBigInt low = self(self, l, half, level - 1);
            HexBigInt high = self(self, l + half, half, level - 1);
            return low + high * power[level - 1];
        };
        HexBigInt ans = build(build, 0, z, countr_zero((unsigned long long)z));
        if (!ans.a.empty()) ans.neg = sign;
        return ans;
    }

    string toDecimal() const
    {
        // 无参数；按 10^9 幂递归分裂为十进制分组，返回规范字符串。
        if (a.empty()) return "0";
        HexBigInt x = *this;
        x.neg = false;
        vector<HexBigInt> power{HexBigInt(1000000000LL)};
        int level = 0;
        while (!(x < power[level]))
        {
            power.push_back(power.back() * power.back());
            level++;
        }
        vector<uint32_t> group(1LL << level);
        auto split = [&](auto &&self, const HexBigInt &v, int l, int lv) -> void
        {
            if (lv == 0)
            {
                group[l] = v.a.empty() ? 0 : v.a[0];
                return;
            }
            auto [high, low] = divmod(v, power[lv - 1]);
            self(self, low, l, lv - 1);
            self(self, high, l + (1LL << (lv - 1)), lv - 1);
        };
        split(split, x, 0, level);
        while (group.size() > 1 && group.back() == 0) group.pop_back();
        string s = neg ? "-" : "";
        s += to_string(group.back());
        for (int i = group.size() - 2; i >= 0; i--)
        {
            string t = to_string(group[i]);
            s += string(9 - t.size(), '0') + t;
        }
        return s;
    }

  private:
    static constexpr int naiveDiv = 64;

    static void trim(vector<uint32_t> &x)
    {
        while (!x.empty() && x.back() == 0)
        {
            x.pop_back();
        }
    }

    static bool less(CIt x, CIt xe, CIt y, CIt ye)
    {
        int nx = xe - x, ny = ye - y;
        while (nx && x[nx - 1] == 0)
        {
            nx--;
        }
        while (ny && y[ny - 1] == 0)
        {
            ny--;
        }
        if (nx != ny)
        {
            return nx < ny;
        }
        for (int i = nx - 1; i >= 0; i--)
        {
            if (x[i] != y[i])
            {
                return x[i] < y[i];
            }
        }
        return false;
    }

    static int cmp(const vector<uint32_t> &x, const vector<uint32_t> &y)
    {
        if (x == y)
        {
            return 0;
        }
        return less(x.begin(), x.end(), y.begin(), y.end()) ? -1 : 1;
    }

    static void add(CIt x, CIt xe, CIt y, CIt ye, It z, It ze)
    {
        if (xe - x < ye - y)
        {
            swap(x, y);
            swap(xe, ye);
        }
        int nx = xe - x, ny = ye - y;
        assert(nx <= ze - z);
        uint32_t carry = 0;
        for (int i = 0; i < ny; i++)
        {
            uint64_t v = (uint64_t)x[i] + y[i] + carry;
            z[i] = v;
            carry = v >> 32;
        }
        for (int i = ny; i < nx; i++)
        {
            uint64_t v = (uint64_t)x[i] + carry;
            z[i] = v;
            carry = v >> 32;
        }
        if (carry)
        {
            assert(z + nx < ze);
            z[nx] = carry;
        }
    }

    static void addTo(It x, It xe, CIt y, CIt ye)
    {
        int nx = xe - x, ny = ye - y;
        uint32_t carry = 0;
        for (int i = 0; i < ny; i++)
        {
            uint64_t v = (uint64_t)x[i] + y[i] + carry;
            x[i] = v;
            carry = v >> 32;
        }
        for (int i = ny; carry && i < nx; i++)
        {
            uint64_t v = (uint64_t)x[i] + carry;
            x[i] = v;
            carry = v >> 32;
        }
        assert(!carry);
    }

    static void subFrom(It x, It xe, CIt y, CIt ye)
    {
        int nx = xe - x, ny = ye - y;
        int32_t borrow = 0;
        for (int i = 0; i < ny; i++)
        {
            int64_t v = (int64_t)x[i] - y[i] + borrow;
            x[i] = v;
            borrow = v >> 32;
        }
        for (int i = ny; borrow && i < nx; i++)
        {
            int64_t v = (int64_t)x[i] + borrow;
            x[i] = v;
            borrow = v >> 32;
        }
        assert(!borrow);
    }

    static vector<uint32_t> addVec(const vector<uint32_t> &x, const vector<uint32_t> &y)
    {
        vector<uint32_t> z(max(x.size(), y.size()) + 1);
        add(x.begin(), x.end(), y.begin(), y.end(), z.begin(), z.end());
        trim(z);
        return z;
    }

    static vector<uint32_t> subVec(const vector<uint32_t> &x, const vector<uint32_t> &y)
    {
        assert(cmp(x, y) >= 0);
        vector<uint32_t> z = x;
        subFrom(z.begin(), z.end(), y.begin(), y.end());
        trim(z);
        return z;
    }

    static void mulNaive(CIt x, CIt xe, CIt y, CIt ye, It z, It ze)
    {
        int nx = xe - x, ny = ye - y;
        assert(ze - z == nx + ny);
        for (int i = 0; i < nx; i++)
        {
            uint32_t carry = 0;
            for (int j = 0; j < ny; j++)
            {
                uint64_t v = (uint64_t)x[i] * y[j] + carry + z[i + j];
                z[i + j] = v;
                carry = v >> 32;
            }
            z[i + ny] = carry;
        }
    }

    static void mul(CIt x, CIt xe, CIt y, CIt ye, It z, It ze)
    {
        if (xe - x < ye - y)
        {
            swap(x, y);
            swap(xe, ye);
        }
        int nx = xe - x, ny = ye - y;
        assert(ze - z == nx + ny);
        if (ny <= 128)
        {
            mulNaive(x, xe, y, ye, z, ze);
            return;
        }
        int n = (nx + 1) / 2;
        if (ny <= n)
        {
            mul(x, x + n, y, ye, z, z + n + ny);
            vector<uint32_t> carry(z + n, z + n + ny);
            fill(z + n, z + n + ny, 0);
            mul(x + n, xe, y, ye, z + n, ze);
            addTo(z + n, ze, carry.begin(), carry.end());
            return;
        }
        mul(x, x + n, y, y + n, z, z + 2 * n);
        mul(x + n, xe, y + n, ye, z + 2 * n, ze);
        vector<uint32_t> x1(n + 1), y1(n + 1), mid(2 * n + 2);
        add(x, x + n, x + n, xe, x1.begin(), x1.end());
        add(y, y + n, y + n, ye, y1.begin(), y1.end());
        mul(x1.begin(), x1.end(), y1.begin(), y1.end(), mid.begin(), mid.end());
        subFrom(mid.begin(), mid.end(), z, z + 2 * n);
        subFrom(mid.begin(), mid.end(), z + 2 * n, ze);
        trim(mid);
        addTo(z + n, ze, mid.begin(), mid.end());
    }

    static vector<uint32_t> mulVec(const vector<uint32_t> &x, const vector<uint32_t> &y)
    {
        if (x.empty() || y.empty())
        {
            return {};
        }
        vector<uint32_t> z(x.size() + y.size());
        mul(x.begin(), x.end(), y.begin(), y.end(), z.begin(), z.end());
        trim(z);
        return z;
    }

    static void leftShift(It x, It xe, int s)
    {
        assert(0 <= s && s < 32);
        if (!s)
        {
            return;
        }
        int back = 32 - s;
        uint32_t carry = 0;
        for (auto it = x; it != xe; ++it)
        {
            uint32_t v = (*it << s) | carry;
            carry = *it >> back;
            *it = v;
        }
        assert(!carry);
    }

    static void shiftR(It x, It xe, int s)
    {
        assert(0 <= s && s < 32);
        if (!s)
        {
            return;
        }
        int back = 32 - s;
        uint32_t carry = 0;
        for (auto it = xe; it != x;)
        {
            --it;
            uint32_t v = (*it >> s) | carry;
            carry = *it << back;
            *it = v;
        }
        assert(!carry);
    }

    static void divNaive(CIt x, CIt xe, CIt y, CIt ye, It q, It qe, It r, It re)
    {
        int nx = xe - x, ny = ye - y;
        if (ny == 1)
        {
            uint64_t carry = 0;
            for (int i = nx - 1; i >= 0; i--)
            {
                uint64_t v = (carry << 32) | x[i];
                q[i] = v / y[0];
                carry = v - (uint64_t)q[i] * y[0];
            }
            r[0] = carry;
            return;
        }
        if (max(nx, ny) <= 2)
        {
            uint64_t xv = x[0] | (nx == 2 ? (uint64_t)x[1] << 32 : 0);
            uint64_t yv = y[0] | (ny == 2 ? (uint64_t)y[1] << 32 : 0);
            uint64_t qv = xv / yv, rv = xv % yv;
            if (q != qe)
            {
                q[0] = qv;
                if (q + 1 != qe)
                {
                    q[1] = qv >> 32;
                }
            }
            r[0] = rv;
            if (r + 1 != re)
            {
                r[1] = rv >> 32;
            }
            return;
        }
        if (less(x, xe, y, ye))
        {
            copy(x, min(xe, x + (ye - y)), r);
            return;
        }
        int shift = countl_zero(y[ny - 1]);
        vector<uint32_t> a(nx + (countl_zero(x[nx - 1]) < shift));
        vector<uint32_t> b(y, ye);
        copy(x, xe, a.begin());
        leftShift(a.begin(), a.end(), shift);
        leftShift(b.begin(), b.end(), shift);
        uint32_t top = b.back();
        vector<uint32_t> qv(a.size() - b.size() + 1);
        vector<uint32_t> rv(a.end() - b.size(), a.end());
        for (int i = qv.size() - 1; i >= 0; i--)
        {
            if (rv.size() == b.size())
            {
                if (!less(rv.begin(), rv.end(), b.begin(), b.end()))
                {
                    qv[i] = 1;
                    rv = subVec(rv, b);
                }
            }
            else if (rv.size() > b.size())
            {
                uint64_t top2 = ((uint64_t)rv.back() << 32) | rv[rv.size() - 2];
                uint64_t z = min<uint64_t>(UINT32_MAX, top2 / top);
                vector<uint32_t> bz = mulVec(b, vector<uint32_t>{(uint32_t)z});
                while (less(rv.begin(), rv.end(), bz.begin(), bz.end()))
                {
                    z--;
                    bz = subVec(bz, b);
                }
                rv = subVec(rv, bz);
                while (!less(rv.begin(), rv.end(), b.begin(), b.end()))
                {
                    z++;
                    rv = subVec(rv, b);
                }
                qv[i] = z;
            }
            if (i)
            {
                rv.insert(rv.begin(), a[i - 1]);
            }
            trim(rv);
        }
        shiftR(rv.begin(), rv.end(), shift);
        trim(qv);
        trim(rv);
        copy(qv.begin(), qv.end(), q);
        copy(rv.begin(), rv.end(), r);
    }

    static void div2n1n(CIt x, CIt xe, CIt y, CIt ye, It q, It qe, It r, It re)
    {
        int n = ye - y;
        assert(xe - x == 2 * n && qe - q == n && re - r == n + 1);
        assert(less(x + n, xe, y, ye));
        if (n % 2 || n <= naiveDiv)
        {
            divNaive(x, xe, y, ye, q, qe, r, re);
            return;
        }
        int h = n / 2;
        vector<uint32_t> r1(n + h + 1);
        copy(x, x + h, r1.begin());
        div3n2n(x + h, xe, y, ye, q + h, qe, r1.begin() + h, r1.end());
        div3n2n(r1.begin(), r1.end() - 1, y, ye, q, q + h, r, re);
    }

    static void div3n2n(CIt x, CIt xe, CIt y, CIt ye, It q, It qe, It r, It re)
    {
        int n = (ye - y) / 2;
        assert(xe - x == 3 * n && qe - q == n && re - r == 2 * n + 1);
        assert(less(x + n, xe, y, ye));
        vector<uint32_t> d(2 * n);
        if (less(x + 2 * n, xe, y + n, ye))
        {
            div2n1n(x + n, xe, y + n, ye, q, qe, r + n, re);
            mul(q, qe, y, y + n, d.begin(), d.end());
        }
        else
        {
            fill(q, qe, UINT32_MAX);
            add(x + n, x + 2 * n, y + n, ye, r + n, re);
            copy(y, y + n, d.begin() + n);
            subFrom(d.begin(), d.end(), y, y + n);
        }
        copy(x, x + n, r);
        while (less(r, re, d.begin(), d.end()))
        {
            addTo(r, re, y, ye);
            vector<uint32_t> one{1};
            subFrom(q, qe, one.begin(), one.end());
        }
        subFrom(r, re, d.begin(), d.end());
    }

    static void divide(CIt x, CIt xe, CIt y, CIt ye, It q, It qe, It r, It re)
    {
        int nx = xe - x, ny = ye - y;
        assert(ny > 0 && qe - q == nx - ny + 1 && re - r == ny);
        if (min(nx - ny, ny) <= naiveDiv)
        {
            divNaive(x, xe, y, ye, q, qe, r, re);
            return;
        }
        int blocks = (ny + naiveDiv - 1) / naiveDiv;
        if (blocks > 1)
        {
            blocks = bit_ceil((unsigned long long)blocks);
        }
        int n = (ny + blocks - 1) / blocks * blocks;
        int digit = n - ny;
        int shift = countl_zero(y[ny - 1]);
        vector<uint32_t> a(nx + digit + (countl_zero(x[nx - 1]) <= shift));
        vector<uint32_t> b(n), rem(n + 1), z(2 * n);
        copy(x, xe, a.begin() + digit);
        copy(y, ye, b.begin() + digit);
        leftShift(a.begin() + digit, a.end(), shift);
        leftShift(b.begin() + digit, b.end(), shift);
        int t = max<long long>(2, (a.size() + n - 1) / n);
        copy(a.begin() + (t - 2) * n, a.end(), z.begin());
        int upper = qe - (q + (t - 2) * n);
        if (upper < n)
        {
            vector<uint32_t> qq(n);
            div2n1n(z.begin(), z.end(), b.begin(), b.end(), qq.begin(), qq.end(), rem.begin(), rem.end());
            copy(qq.begin(), qq.begin() + upper, q + (t - 2) * n);
        }
        else
        {
            div2n1n(z.begin(), z.end(), b.begin(), b.end(), q + (t - 2) * n,
                    q + (t - 1) * n, rem.begin(), rem.end());
        }
        for (int i = t - 3; i >= 0; i--)
        {
            copy(a.begin() + i * n, a.begin() + (i + 1) * n, z.begin());
            copy(rem.begin(), rem.begin() + n, z.begin() + n);
            fill(rem.begin(), rem.end(), 0);
            div2n1n(z.begin(), z.end(), b.begin(), b.end(), q + i * n,
                    q + (i + 1) * n, rem.begin(), rem.end());
        }
        copy(rem.begin() + digit, rem.begin() + digit + (re - r), r);
        shiftR(r, re, shift);
    }

    static pair<vector<uint32_t>, vector<uint32_t>> divVec(const vector<uint32_t> &x,
                                                            const vector<uint32_t> &y)
    {
        assert(!y.empty());
        if (cmp(x, y) < 0)
        {
            return {{}, x};
        }
        vector<uint32_t> q(x.size() - y.size() + 1), r(y.size());
        divide(x.begin(), x.end(), y.begin(), y.end(), q.begin(), q.end(), r.begin(), r.end());
        trim(q);
        trim(r);
        return {q, r};
    }
};

// 十进制百万位整数：以 10^9 为基数，长乘法复用公共三模 NTT，除法使用 Newton 逆。
struct BigInt
{
    static constexpr uint32_t B = 1000000000;
    bool neg = false;
    vector<uint32_t> a;

    BigInt() = default;
    BigInt(long long x)
    {
        if (x < 0) neg = true, x = -x;
        while (x)
        {
            a.push_back(x % B);
            x /= B;
        }
    }
    BigInt(const string &s)
    {
        assign(s);
    }
    BigInt(bool s, vector<uint32_t> v) : neg(s), a(move(v))
    {
        trim(a);
        if (a.empty()) neg = false;
    }

    BigInt &assign(const string &s)
    {
        assert(!s.empty());
        int p = (s[0] == '-' || s[0] == '+');
        neg = s[0] == '-';
        assert(p < (int)s.size());
        a.clear();
        for (int r = s.size(); r > p; r -= 9)
        {
            int l = max(p, r - 9);
            uint32_t v = 0;
            for (int i = l; i < r; i++)
            {
                assert('0' <= s[i] && s[i] <= '9');
                v = v * 10 + s[i] - '0';
            }
            a.push_back(v);
        }
        trim(a);
        if (a.empty()) neg = false;
        return *this;
    }

    string toDecimal() const
    {
        if (a.empty()) return "0";
        string s = neg ? "-" : "";
        s += to_string(a.back());
        for (int i = (int)a.size() - 2; i >= 0; i--)
        {
            string t = to_string(a[i]);
            s += string(9 - t.size(), '0') + t;
        }
        return s;
    }

    friend bool operator==(const BigInt &x, const BigInt &y)
    {
        return x.neg == y.neg && x.a == y.a;
    }
    friend bool operator<(const BigInt &x, const BigInt &y)
    {
        if (x.neg != y.neg) return x.neg;
        int c = cmp(x.a, y.a);
        return x.neg ? c > 0 : c < 0;
    }
    friend bool operator!=(const BigInt &x, const BigInt &y) { return !(x == y); }
    friend bool operator>(const BigInt &x, const BigInt &y) { return y < x; }
    friend bool operator<=(const BigInt &x, const BigInt &y) { return !(y < x); }
    friend bool operator>=(const BigInt &x, const BigInt &y) { return !(x < y); }

    friend BigInt operator-(const BigInt &x)
    {
        BigInt y = x;
        if (!y.a.empty()) y.neg = !y.neg;
        return y;
    }
    friend BigInt operator+(const BigInt &x, const BigInt &y)
    {
        if (x.neg == y.neg) return {x.neg, addVec(x.a, y.a)};
        int c = cmp(x.a, y.a);
        if (c <= 0) return {c ? y.neg : false, subVec(y.a, x.a)};
        return {x.neg, subVec(x.a, y.a)};
    }
    friend BigInt operator-(const BigInt &x, const BigInt &y) { return x + (-y); }
    friend BigInt operator*(const BigInt &x, const BigInt &y)
    {
        auto z = mulVec(x.a, y.a);
        return {z.empty() ? false : x.neg != y.neg, move(z)};
    }
    friend pair<BigInt, BigInt> divmod(const BigInt &x, const BigInt &y)
    {
        assert(!y.a.empty());
        auto z = divVec(x.a, y.a);
        return {{z.first.empty() ? false : x.neg != y.neg, move(z.first)},
                {z.second.empty() ? false : x.neg, move(z.second)}};
    }
    friend BigInt operator/(const BigInt &x, const BigInt &y) { return divmod(x, y).first; }
    friend BigInt operator%(const BigInt &x, const BigInt &y) { return divmod(x, y).second; }

private:
    static void trim(vector<uint32_t> &x)
    {
        while (!x.empty() && x.back() == 0) x.pop_back();
    }
    static int cmp(const vector<uint32_t> &x, const vector<uint32_t> &y)
    {
        if (x.size() != y.size()) return x.size() < y.size() ? -1 : 1;
        for (int i = (int)x.size() - 1; i >= 0; i--)
            if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
        return 0;
    }
    static vector<uint32_t> addVec(const vector<uint32_t> &x, const vector<uint32_t> &y)
    {
        vector<uint32_t> z(max(x.size(), y.size()) + 1);
        uint64_t carry = 0;
        for (int i = 0; i < (int)z.size() - 1; i++)
        {
            uint64_t v = carry + (i < (int)x.size() ? x[i] : 0) + (i < (int)y.size() ? y[i] : 0);
            z[i] = v % B;
            carry = v / B;
        }
        z.back() = carry;
        trim(z);
        return z;
    }
    static vector<uint32_t> subVec(const vector<uint32_t> &x, const vector<uint32_t> &y)
    {
        assert(cmp(x, y) >= 0);
        vector<uint32_t> z = x;
        int64_t borrow = 0;
        for (int i = 0; i < (int)z.size(); i++)
        {
            int64_t v = (int64_t)z[i] - (i < (int)y.size() ? y[i] : 0) - borrow;
            if (v < 0) v += B, borrow = 1; else borrow = 0;
            z[i] = v;
        }
        trim(z);
        return z;
    }
    static vector<uint32_t> mulSmall(const vector<uint32_t> &x, uint32_t m)
    {
        vector<uint32_t> z(x.size() + 1);
        uint64_t carry = 0;
        for (int i = 0; i < (int)x.size(); i++)
        {
            uint64_t v = (uint64_t)x[i] * m + carry;
            z[i] = v % B;
            carry = v / B;
        }
        z.back() = carry;
        trim(z);
        return z;
    }
    static pair<vector<uint32_t>, uint32_t> divSmall(const vector<uint32_t> &x, uint32_t d)
    {
        vector<uint32_t> q(x.size());
        uint64_t rem = 0;
        for (int i = (int)x.size() - 1; i >= 0; i--)
        {
            uint64_t v = rem * B + x[i];
            q[i] = v / d;
            rem = v % d;
        }
        trim(q);
        return {q, (uint32_t)rem};
    }
    static vector<uint32_t> mulVec(const vector<uint32_t> &x, const vector<uint32_t> &y)
    {
        if (x.empty() || y.empty()) return {};
        if (min(x.size(), y.size()) <= 64)
        {
            vector<__uint128_t> c(x.size() + y.size());
            for (int i = 0; i < (int)x.size(); i++)
                for (int j = 0; j < (int)y.size(); j++) c[i + j] += (__uint128_t)x[i] * y[j];
            vector<uint32_t> z(c.size());
            __uint128_t carry = 0;
            for (int i = 0; i < (int)c.size(); i++)
            {
                __uint128_t v = c[i] + carry;
                z[i] = v % B;
                carry = v / B;
            }
            trim(z);
            return z;
        }
        constexpr uint32_t p0 = 167772161, p1 = 469762049, p2 = 754974721;
        constexpr uint32_t r0 = 3, r1 = 3, r2 = 11;
        vector<unsigned long long> a(x.begin(), x.end()), b(y.begin(), y.end());
        auto c0 = arbModConv::StaticNTT<p0, r0>::conv(a, b);
        auto c1 = arbModConv::StaticNTT<p1, r1>::conv(a, b);
        auto c2 = arbModConv::StaticNTT<p2, r2>::conv(a, b);
        auto inv = [](uint32_t a, uint32_t m)
        {
            uint64_t r = 1, b = a;
            uint32_t e = m - 2;
            while (e)
            {
                if (e & 1) r = r * b % m;
                b = b * b % m;
                e >>= 1;
            }
            return (uint32_t)r;
        };
        uint32_t i01 = inv(p0 % p1, p1), i012 = inv((uint64_t)p0 * p1 % p2, p2);
        vector<uint32_t> z(c0.size());
        __uint128_t carry = 0;
        for (int i = 0; i < (int)c0.size(); i++)
        {
            uint64_t u = c0[i];
            uint64_t v = ((c1[i] + p1 - u % p1) % p1) * (uint64_t)i01 % p1;
            uint64_t seen = (u + (uint64_t)p0 * v) % p2;
            uint64_t w = ((c2[i] + p2 - seen) % p2) * (uint64_t)i012 % p2;
            __uint128_t cur = carry + u + (__uint128_t)p0 * v + (__uint128_t)p0 * p1 * w;
            z[i] = cur % B;
            carry = cur / B;
        }
        while (carry)
        {
            z.push_back(carry % B);
            carry /= B;
        }
        trim(z);
        return z;
    }
    static pair<vector<uint32_t>, vector<uint32_t>> divNaive(const vector<uint32_t> &x,
                                                              const vector<uint32_t> &y)
    {
        if (x.empty() || cmp(x, y) < 0) return {{}, x};
        if (y.size() == 1)
        {
            auto [q, r] = divSmall(x, y[0]);
            return {q, r ? vector<uint32_t>{r} : vector<uint32_t>{}};
        }
        uint32_t norm = B / (y.back() + 1);
        vector<uint32_t> a = mulSmall(x, norm), b = mulSmall(y, norm);
        vector<uint32_t> q(a.size() - b.size() + 1), r(a.end() - b.size(), a.end());
        for (int i = (int)q.size() - 1; i >= 0; i--)
        {
            uint64_t top = r.size() > b.size() ? (uint64_t)r.back() * B + r[r.size() - 2] : 0;
            uint32_t d = top ? min<uint64_t>(B - 1, top / b.back()) : 1;
            vector<uint32_t> z = mulSmall(b, d);
            while (cmp(r, z) < 0) d--, z = subVec(z, b);
            r = subVec(r, z);
            while (cmp(r, b) >= 0) d++, r = subVec(r, b);
            q[i] = d;
            if (i) r.insert(r.begin(), a[i - 1]);
            trim(r);
        }
        auto rr = divSmall(r, norm);
        assert(rr.second == 0);
        trim(q);
        return {q, rr.first};
    }
    static vector<uint32_t> calcInv(const vector<uint32_t> &a, int deg)
    {
        int k = deg, c = a.size();
        while (k > 64) k = (k + 1) / 2;
        vector<uint32_t> z(c + k + 1);
        z.back() = 1;
        z = divNaive(z, a).first;
        while (k < deg)
        {
            vector<uint32_t> s = mulVec(z, z);
            s.insert(s.begin(), 0);
            vector<uint32_t> t(2 * k + 1);
            int take = min<int>(a.size(), t.size());
            copy(a.end() - take, a.end(), t.end() - take);
            vector<uint32_t> u = mulVec(s, t);
            if ((int)u.size() > 2 * k + 1) u.erase(u.begin(), u.begin() + 2 * k + 1);
            else u.clear();
            vector<uint32_t> w(k + 1), w2 = addVec(z, z);
            copy(w2.begin(), w2.end(), back_inserter(w));
            z = subVec(w, u);
            if (!z.empty()) z.erase(z.begin());
            k *= 2;
        }
        if (k > deg) z.erase(z.begin(), z.begin() + k - deg);
        return z;
    }
    static pair<vector<uint32_t>, vector<uint32_t>> divVec(const vector<uint32_t> &x,
                                                            const vector<uint32_t> &y)
    {
        assert(!y.empty());
        if (cmp(x, y) < 0) return {{}, x};
        if (y.size() <= 64 || x.size() - y.size() <= 64) return divNaive(x, y);
        uint32_t norm = B / (y.back() + 1);
        vector<uint32_t> a = mulSmall(x, norm), b = mulSmall(y, norm);
        int deg = a.size() - b.size() + 2;
        vector<uint32_t> inv = calcInv(b, deg);
        vector<uint32_t> q = mulVec(a, inv);
        if ((int)q.size() > (int)b.size() + deg) q.erase(q.begin(), q.begin() + b.size() + deg); else q.clear();
        vector<uint32_t> prod = mulVec(b, q);
        while (cmp(a, prod) < 0) q = subVec(q, {1}), prod = subVec(prod, b);
        vector<uint32_t> r = subVec(a, prod);
        while (cmp(r, b) >= 0) q = addVec(q, {1}), r = subVec(r, b);
        auto rr = divSmall(r, norm);
        assert(rr.second == 0);
        trim(q);
        return {q, rr.first};
    }
};

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int t; cin >> t;
    while (t--)
    {
        string a, b; cin >> a >> b;
        cout << (BigInt(a) + BigInt(b)).toDecimal() << '\n';
    }
}
