#pragma once

#include "1_1_008.hpp"
using namespace std;
#define int long long

enum class BitOp
{
    Or,
    And,
    Xor
};

template <bool Inv = false, class T> void orFWT(T *a, int n, int d)
{
    // a 提供 n*d 项连续空间，每行 d 个通道，n 是至多 2^30 的正 2 的幂；Inv 指定逆变换，原地修改，无返回值。
    assert(a && n > 0 && d > 0 && n <= (1ULL << 30) && d <= 2147483647ULL &&
           (size_t)n <= numeric_limits<size_t>::max() / sizeof(T) / (size_t)d &&
           has_single_bit((unsigned long long)n)); // 调试检查，可删。
    uint32_t m = n;
    uint32_t w = d;
    for (uint32_t h = 1; h < m; h <<= 1)
    {
        for (uint32_t l = 0; l < m; l += h << 1)
        {
            for (uint32_t s = 0; s < h; s++)
            {
                T *x = a + (size_t)(l + s) * w;
                T *y = x + (size_t)h * w;
                if constexpr (!Inv)
                {
                    for (uint32_t k = 0; k < w; k++)
                    {
                        y[k] += x[k];
                    }
                }
                else
                {
                    for (uint32_t k = 0; k < w; k++)
                    {
                        y[k] -= x[k];
                    }
                }
            }
        }
    }
}

template <class T> void orFWT(T *a, int n, int d, bool inv)
{
    // a 提供 n*d 项连续空间，每行 d 个通道；inv=false/true 指定正/逆 OR 变换，原地修改，无返回值。
    if (inv)
    {
        orFWT<true>(a, n, d);
    }
    else
    {
        orFWT<false>(a, n, d);
    }
}

template <bool Inv = false, class T> void orFWT(vector<T> &a)
{
    // a 按掩码编号，长度是至多 2^30 的正 2 的幂；Inv 指定逆变换，原地修改，无返回值。
    assert(!a.empty() && a.size() <= (1ULL << 30) && has_single_bit(a.size())); // 调试检查，可删。
    uint32_t n = a.size();
    for (uint32_t h = 1; h < n; h <<= 1)
    {
        for (uint32_t i = 0; i < n; i += h << 1)
        {
            for (uint32_t j = 0; j < h; j++)
            {
                if constexpr (!Inv)
                {
                    a[i + j + h] += a[i + j];
                }
                else
                {
                    a[i + j + h] -= a[i + j];
                }
            }
        }
    }
}

template <class T> void fwt(vector<T> &a, BitOp op, bool inv = false)
{
    // a 是按掩码编号的系数，op 指定或、与或异或，inv 指定正变换或逆变换；原地完成变换。
    assert(!a.empty() && a.size() <= (1ULL << 30) && has_single_bit(a.size()));
    int n = a.size();
    if (op == BitOp::Or)
    {
        if (inv)
        {
            orFWT<true>(a);
        }
        else
        {
            orFWT<false>(a);
        }
        return;
    }
    for (int h = 1; h < n; h <<= 1)
    {
        for (int i = 0; i < n; i += h << 1)
        {
            for (int j = 0; j < h; j++)
            {
                T x = a[i + j];
                T y = a[i + j + h];
                if (op == BitOp::Xor)
                {
                    a[i + j] = x + y;
                    a[i + j + h] = x - y;
                }
                else
                {
                    if (!inv)
                    {
                        a[i + j] += y;
                    }
                    else
                    {
                        a[i + j] -= y;
                    }
                }
            }
        }
    }
    if (op == BitOp::Xor && inv)
    {
        for (T &x : a)
        {
            x /= T(n);
        }
    }
}

// 方向由函数名表达；BitOp 版本保留给旧代码。
template <class T> void orFwt(vector<T> &a, bool inv = false)
{
    // a 按掩码编号，长度是正 2 的幂；inv=false/true 指定正/逆 OR 变换，原地修改，无返回值。
    fwt(a, BitOp::Or, inv);
}
template <class T> void andFwt(vector<T> &a, bool inv = false)
{
    // a 按掩码编号，长度是正 2 的幂；inv=false/true 指定正/逆 AND 变换，原地修改，无返回值。
    fwt(a, BitOp::And, inv);
}
template <class T> void xorFwt(vector<T> &a, bool inv = false)
{
    // a 按掩码编号，长度是正 2 的幂；inv=false/true 指定正/逆 XOR 变换，原地修改，无返回值。
    fwt(a, BitOp::Xor, inv);
}

template <class T> vector<T> bitConv(vector<T> a, vector<T> b, BitOp op)
{
    // a、b 是等长的掩码系数，op 指定卷积下标使用或、与或异或；返回对应的按位卷积。
    assert(!a.empty() && a.size() == b.size()); // 调试检查，可删。
    fwt(a, op);
    fwt(b, op);
    for (int i = 0; i < (int)a.size(); i++)
    {
        a[i] *= b[i];
    }
    fwt(a, op, true);
    return a;
}

template <class T> vector<T> orConv(vector<T> a, vector<T> b)
{
    // a、b 等长且长度是正 2 的幂；返回按掩码顺序的 OR 卷积，不修改调用者的输入。
    return bitConv(move(a), move(b), BitOp::Or);
}
template <class T> vector<T> andConv(vector<T> a, vector<T> b)
{
    // a、b 等长且长度是正 2 的幂；返回按掩码顺序的 AND 卷积，不修改调用者的输入。
    return bitConv(move(a), move(b), BitOp::And);
}
template <class T> vector<T> xorConv(vector<T> a, vector<T> b)
{
    // a、b 等长且长度是正 2 的幂；返回按掩码顺序的 XOR 卷积，不修改调用者的输入。
    return bitConv(move(a), move(b), BitOp::Xor);
}

template <uint32_t P> uint32_t fwtAdd(uint32_t x, uint32_t y)
{
    // x、y 在 [0,P) 内，1<P<2^31；返回 (x+y) mod P，加宽中间和以兼容有符号打印版。
    unsigned long long s = (unsigned long long)x + y;
    return s >= P ? s - P : s;
}

template <uint32_t P> uint32_t fwtSub(uint32_t x, uint32_t y)
{
    // x、y 在 [0,P) 内，1<P<2^31；返回 (x-y) mod P，各分支结果均在 [0,P)。
    return x >= y ? x - y : x + (P - y);
}

template <uint32_t P, BitOp Op, bool Inv, uint32_t N> void fwtRec(uint32_t *a)
{
    // a 指向 N 项 [0,P) 代表元；N 是正 2 的幂，Op 指定位运算，Inv 指定逆变换；原地修改，无返回值。
    constexpr uint32_t h = N / 2;
    if constexpr (h)
    {
        fwtRec<P, Op, Inv, h>(a);
        fwtRec<P, Op, Inv, h>(a + h);
        for (uint32_t i = 0; i < h; i++)
        {
            uint32_t x = a[i], y = a[i + h];
            if constexpr (Op == BitOp::Or)
            {
                a[i + h] = Inv ? fwtSub<P>(y, x) : fwtAdd<P>(x, y);
            }
            else if constexpr (Op == BitOp::And)
            {
                a[i] = Inv ? fwtSub<P>(x, y) : fwtAdd<P>(x, y);
            }
            else
            {
                uint32_t s = fwtAdd<P>(x, y);
                uint32_t d = fwtSub<P>(x, y);
                if constexpr (Inv)
                {
                    s = ((unsigned long long)s + (s & 1) * P) >> 1;
                    d = ((unsigned long long)d + (d & 1) * P) >> 1;
                }
                a[i] = s;
                a[i + h] = d;
            }
        }
    }
}

template <uint32_t P, BitOp Op, bool Inv = false, uint32_t N = 1>
void fwtMod(uint32_t *a, uint32_t n)
{
    // a 指向 n 项 [0,P) 代表元，n 是至多 2^30 的正 2 的幂；Op 指定位运算，Inv 指定逆变换。
    // N 是递归分派起点，调用时保持默认值；原地修改 a，无返回值。
    static_assert(1 < P && P < (1U << 31));
    static_assert(Op != BitOp::Xor || (P & 1));
    static_assert(N > 0 && N <= (1U << 30) && has_single_bit((unsigned long long)N));
    assert(a && n >= N && n <= (1U << 30) && has_single_bit((unsigned long long)n));
    if (n == N)
    {
        fwtRec<P, Op, Inv, N>(a);
    }
    else if constexpr (N < (1U << 30))
    {
        fwtMod<P, Op, Inv, N << 1>(a, n);
    }
}

template <uint32_t P, BitOp Op, bool Inv = false> void fwtMod(vector<uint32_t> &a)
{
    // a 按掩码保存 [0,P) 代表元；Op 指定位运算，Inv 指定逆变换，原地修改，无返回值。
    assert(!a.empty() && a.size() <= (1ULL << 30) && has_single_bit(a.size()));
    fwtMod<P, Op, Inv>(a.data(), a.size());
}

template <uint32_t P, bool Inv = false> void orFwtMod(vector<uint32_t> &a)
{
    // a 按掩码保存 [0,P) 代表元；Inv 指定逆 OR 变换，原地修改，无返回值。
    fwtMod<P, BitOp::Or, Inv>(a);
}
template <uint32_t P, bool Inv = false> void andFwtMod(vector<uint32_t> &a)
{
    // a 按掩码保存 [0,P) 代表元；Inv 指定逆 AND 变换，原地修改，无返回值。
    fwtMod<P, BitOp::And, Inv>(a);
}
template <uint32_t P, bool Inv = false> void xorFwtMod(vector<uint32_t> &a)
{
    // a 按掩码保存 [0,P) 代表元；Inv 指定逆 XOR 变换，原地修改，无返回值。
    fwtMod<P, BitOp::Xor, Inv>(a);
}

template <uint32_t P, BitOp Op>
vector<uint32_t> bitConvMod(vector<uint32_t> a, vector<uint32_t> b)
{
    // a、b 等长，长度是正 2 的幂，各项在 [0,P)；返回 c[i Op j]+=a[i]*b[j] 的模 P 代表元。
    assert(!a.empty() && a.size() == b.size()); // 调试检查，可删
    fwtMod<P, Op>(a);
    fwtMod<P, Op>(b);
    for (uint32_t i = 0; i < a.size(); i++)
    {
        a[i] = (unsigned long long)a[i] * b[i] % P;
    }
    fwtMod<P, Op, true>(a);
    return a;
}

template <uint32_t P> vector<uint32_t> orConvMod(vector<uint32_t> a, vector<uint32_t> b)
{
    // a、b 等长，长度是正 2 的幂，各项在 [0,P)；返回模 P 的 OR 卷积，不修改调用者输入。
    return bitConvMod<P, BitOp::Or>(move(a), move(b));
}
template <uint32_t P> vector<uint32_t> andConvMod(vector<uint32_t> a, vector<uint32_t> b)
{
    // a、b 等长，长度是正 2 的幂，各项在 [0,P)；返回模 P 的 AND 卷积，不修改调用者输入。
    return bitConvMod<P, BitOp::And>(move(a), move(b));
}
template <uint32_t P> vector<uint32_t> xorConvMod(vector<uint32_t> a, vector<uint32_t> b)
{
    // a、b 等长，长度是正 2 的幂，各项在 [0,P)；返回模 P 的 XOR 卷积，不修改调用者输入。
    return bitConvMod<P, BitOp::Xor>(move(a), move(b));
}

// 稀疏 XOR 卷积专用接口，模数固定为大素数。
constexpr uint64_t sparseXorMod = 1000000000000125953ULL;

class XorMint
{
private:
    uint64_t x = 0;
    static uint64_t mulRaw(uint64_t a, uint64_t b)
    {
        return (uint64_t)((unsigned __int128)a * b % sparseXorMod);
    }

public:
    XorMint() = default;
    XorMint(long long v)
    {
        v %= (long long)sparseXorMod;
        if (v < 0) v += sparseXorMod;
        x = (uint64_t)v;
    }
    static XorMint raw(uint64_t v)
    {
        XorMint r;
        r.x = v;
        return r;
    }
    uint64_t val() const { return x; }
    XorMint operator-() const { return raw(x ? sparseXorMod - x : 0); }
    XorMint &operator+=(const XorMint &o)
    {
        x += o.x;
        if (x >= sparseXorMod) x -= sparseXorMod;
        return *this;
    }
    XorMint &operator-=(const XorMint &o)
    {
        x += sparseXorMod - o.x;
        if (x >= sparseXorMod) x -= sparseXorMod;
        return *this;
    }
    XorMint &operator*=(const XorMint &o)
    {
        x = mulRaw(x, o.x);
        return *this;
    }
    friend XorMint operator+(XorMint a, const XorMint &b) { return a += b; }
    friend XorMint operator-(XorMint a, const XorMint &b) { return a -= b; }
    friend XorMint operator*(XorMint a, const XorMint &b) { return a *= b; }
    friend bool operator==(const XorMint &a, const XorMint &b) { return a.x == b.x; }
    XorMint pow(uint64_t e) const
    {
        XorMint a = *this, r = 1;
        while (e)
        {
            if (e & 1) r *= a;
            a *= a;
            e >>= 1;
        }
        return r;
    }
    XorMint inv() const
    {
        assert(x != 0); // 调试检查，可删
        return pow(sparseXorMod - 2);
    }
};

class XorInfo
{
private:
    static XorMint t, invt;
    static int maxBit;
    static int legendre(const XorMint &x)
    {
        XorMint z = x.pow((sparseXorMod - 1) / 2);
        if (z == XorMint(1)) return 1;
        if (z == XorMint(0)) return 0;
        return -1;
    }
    static XorMint sqrt(const XorMint &x)
    {
        assert(legendre(x) >= 0); // 调试检查，可删
        if (x == XorMint(0)) return 0;
        XorMint w2, a = 0;
        while (true)
        {
            w2 = a * a - x;
            if (legendre(w2) == -1) break;
            a += XorMint(1);
        }
        auto mul = [w2](pair<XorMint, XorMint> u,
                        pair<XorMint, XorMint> v)
        {
            return make_pair(u.first * v.first + u.second * v.second * w2,
                             u.first * v.second + u.second * v.first);
        };
        pair<XorMint, XorMint> r{1, 0}, b{a, 1};
        uint64_t e = (sparseXorMod + 1) / 2;
        while (e)
        {
            if (e & 1) r = mul(r, b);
            b = mul(b, b);
            e >>= 1;
        }
        return r.first;
    }
    XorInfo(XorMint p, XorMint q, long long e, int zero)
        : xp(p), xq(q), power(e), zeroCount(zero) {}

public:
    XorMint xp = 1, xq = 1;
    long long power = 0;
    int zeroCount = 0;
    XorInfo() = default;
    static void setMaxBit(int k) { maxBit = k; }
    XorInfo(XorMint x, int bits) : xp(x)
    {
        if (x == XorMint(0))
        {
            xp = 1;
            zeroCount = 1;
            return;
        }
        for (int i = 0; i < bits; i++)
        {
            if (legendre(xp) == 1) xp = sqrt(xp);
            else
            {
                xp = sqrt(xp * invt);
                power |= 1LL << i;
            }
        }
        power <<= maxBit - bits;
    }
    XorInfo operator*(const XorInfo &o) const
    {
        return {xp * o.xp, xq * o.xq, power + o.power, zeroCount + o.zeroCount};
    }
    XorInfo operator/(const XorInfo &o) const
    {
        return {xp * o.xq, xq * o.xp, power - o.power, zeroCount - o.zeroCount};
    }
    XorInfo &operator*=(const XorInfo &o)
    {
        xp *= o.xp;
        xq *= o.xq;
        power += o.power;
        zeroCount += o.zeroCount;
        return *this;
    }
    XorInfo &operator/=(const XorInfo &o)
    {
        xp *= o.xq;
        xq *= o.xp;
        power -= o.power;
        zeroCount -= o.zeroCount;
        return *this;
    }
    XorMint value() const
    {
        if (zeroCount) return 0;
        XorMint r = xp * xq.inv();
        if (power > 0) r *= t.pow(power >> maxBit);
        else if (power < 0) r *= invt.pow((-power) >> maxBit);
        return r;
    }
    static void init()
    {
        XorMint a = 0, w2;
        while (true)
        {
            w2 = a * a - XorMint(1);
            if (legendre(w2) == -1)
            {
                t = w2;
                break;
            }
            a += XorMint(1);
        }
        invt = t.inv();
    }
};

inline XorMint XorInfo::t;
inline XorMint XorInfo::invt;
inline int XorInfo::maxBit = 0;

// factors 每项为 {局部 Walsh 基向量, 基准掩码/偏移}，返回长度 2^n 的异或卷积。
inline vector<uint64_t> sparseXor(
    int n, const vector<vector<pair<int, long long>>> &factors)
{
    assert(0 <= n && n < 30); // 调试检查，可删
    int size = 1LL << n, maxBit = 0;
    for (const auto &factor : factors)
    {
        assert(!factor.empty());
        maxBit = max(maxBit, (int)factor.size() - 1);
    }
    assert(maxBit < 60); // 调试检查，可删
    XorInfo::setMaxBit(maxBit);
    XorInfo::init();
    vector<XorInfo> all(size);
    int delta = 0;
    for (const auto &factor : factors)
    {
        int bits = factor.size() - 1, base = factor.back().first;
        XorMint offset = factor.back().second;
        delta ^= base;
        int localSize = 1LL << bits;
        vector<int> mask(localSize);
        vector<XorMint> walsh(localSize);
        for (int i = 0; i < bits; i++)
        {
            mask[1LL << i] = factor[i].first ^ base;
            walsh[1LL << i] = factor[i].second;
        }
        for (int s = 1; s < localSize; s++)
        {
            int b = s & -s;
            mask[s] = mask[s ^ b] ^ mask[b];
        }
        for (int h = 1; h < localSize; h <<= 1)
        {
            for (int l = 0; l < localSize; l += h << 1)
            {
                for (int j = 0; j < h; j++)
                {
                    auto x = walsh[l + j], y = walsh[l + j + h];
                    walsh[l + j] = x + y;
                    walsh[l + j + h] = x - y;
                }
            }
        }
        for (auto &x : walsh)
        {
            x += offset;
        }
        vector<XorInfo> local(localSize);
        for (int s = 0; s < localSize; s++)
        {
            local[s] = XorInfo(walsh[s], bits);
        }
        for (int h = 1; h < localSize; h <<= 1)
        {
            for (int l = 0; l < localSize; l += h << 1)
            {
                for (int j = 0; j < h; j++)
                {
                    auto x = local[l + j], y = local[l + j + h];
                    local[l + j] = x * y;
                    local[l + j + h] = x / y;
                }
            }
        }
        for (int s = 0; s < localSize; s++)
        {
            all[mask[s]] *= local[s];
        }
    }
    for (int h = 1; h < size; h <<= 1)
    {
        for (int l = 0; l < size; l += h << 1)
        {
            for (int j = 0; j < h; j++)
            {
                auto x = all[l + j], y = all[l + j + h];
                all[l + j] = x * y;
                all[l + j + h] = x / y;
            }
        }
    }
    vector<XorMint> ans(size);
    for (int i = 0; i < size; i++)
    {
        ans[i] = all[i].value();
    }
    XorMint inv2 = XorMint(2).inv();
    for (int h = 1; h < size; h <<= 1)
    {
        for (int l = 0; l < size; l += h << 1)
        {
            for (int j = 0; j < h; j++)
            {
                auto x = ans[l + j], y = ans[l + j + h];
                ans[l + j] = (x + y) * inv2;
                ans[l + j + h] = (x - y) * inv2;
            }
        }
    }
    vector<uint64_t> res(size);
    for (int i = 0; i < size; i++)
    {
        res[i] = ans[i ^ delta].val();
    }
    return res;
}
