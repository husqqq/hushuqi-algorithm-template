#pragma once

#include "10_1_002.hpp"

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
