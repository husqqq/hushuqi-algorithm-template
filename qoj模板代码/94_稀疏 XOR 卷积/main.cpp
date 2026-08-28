// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 94 稀疏 XOR 卷积


#include <bits/stdc++.h>
using namespace std;
#define int long long

template <long long P> struct MInt
{
    static_assert(P > 1);
    using V = conditional_t<(P <= numeric_limits<int32_t>::max()),
                            uint32_t, unsigned long long>;
    static constexpr V M = (V)P; // 与 x 同宽的模数，避免热路径被 P 提升为有符号 64 位
    V x = 0; // 当前剩余类在 [0,P) 内的代表元；常见小模使用无符号 32 位

    MInt() = default;

    MInt(long long v)
    {
        // v 是要转入模 P 剩余类的整数；构造其最小非负代表元。
        v %= P;
        if (v < 0)
        {
            v += P;
        }
        x = (V)v;
    }

    static constexpr long long mod()
    {
        // 无参数；返回编译期模数 P。
        return P;
    }

    long long val() const
    {
        // 无参数；返回当前剩余类的最小非负代表元。
        return x;
    }

    MInt operator-() const
    {
        // 无参数；返回当前剩余类的加法逆元。
        MInt ans;
        ans.x = x ? M - x : 0;
        return ans;
    }

    MInt &operator+=(const MInt &o)
    {
        // o 是要加到当前值上的同模剩余类；原地完成模加并返回当前对象引用。
        x += o.x;
        if (x >= M)
        {
            x -= M;
        }
        return *this;
    }

    MInt &operator-=(const MInt &o)
    {
        // o 是要从当前值减去的同模剩余类；原地完成模减并返回当前对象引用。
        x += M - o.x;
        if (x >= M)
        {
            x -= M;
        }
        return *this;
    }

    MInt &operator*=(const MInt &o)
    {
        // o 是要乘到当前值上的同模剩余类；原地完成模乘并返回当前对象引用。
        if constexpr (P <= 2147483647LL)
        {
            x = (unsigned long long)x * o.x % M;
        }
        else
        {
            x = (unsigned __int128)x * o.x % M;
        }
        return *this;
    }

    MInt &operator/=(const MInt &o)
    {
        // o 是非零同模剩余类且 P 必须为素数；原地乘以 o 的逆元并返回当前对象引用。
        return *this *= o.inv();
    }

    friend MInt operator+(MInt a, const MInt &b)
    {
        // a、b 是同模剩余类；返回 a+b。
        return a += b;
    }

    friend MInt operator-(MInt a, const MInt &b)
    {
        // a、b 是同模剩余类；返回 a-b。
        return a -= b;
    }

    friend MInt operator*(MInt a, const MInt &b)
    {
        // a、b 是同模剩余类；返回 a*b。
        return a *= b;
    }

    friend MInt operator/(MInt a, const MInt &b)
    {
        // a、b 是同模剩余类且 b 非零；返回 a/b。
        return a /= b;
    }

    friend bool operator==(const MInt &, const MInt &) = default;

    MInt pow(unsigned long long b) const
    {
        // b 是非负指数；返回当前剩余类的 b 次幂。
        MInt a = *this;
        MInt ans = 1;
        while (b)
        {
            if (b & 1)
            {
                ans *= a;
            }
            a *= a;
            b >>= 1;
        }
        return ans;
    }

    MInt inv() const
    {
        // 当前值必须非零且 P 必须为素数；返回乘法逆元。
        assert(x != 0); // 调试检查，可删
        return pow(P - 2);
    }
};

template <long long P> const vector<MInt<P>> &invTable(int n)
{
    // n 是需要的最大下标且 0<=n<P；返回至少覆盖 0..n 的共享模逆元表，其中位置 0 为 0。
    assert(0 <= n && n < P); // 调试检查，可删
    static vector<MInt<P>> iv{0, 1};
    int old = iv.size();
    if (old <= n)
    {
        iv.resize(n + 1);
        for (int i = old; i <= n; i++)
        {
            iv[i] = MInt<P>(0) - MInt<P>(P / i) * iv[P % i];
        }
    }
    return iv;
}

constexpr int mod = 998244353;
using Z = MInt<mod>;

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
    // a 指向 n 行、每行 d 项的连续集合函数，n 是 2 的幂；沿行掩码做批量 OR 变换。
    assert(a && n > 0 && d > 0 && n <= UINT32_MAX && d <= UINT32_MAX &&
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
    // a、n、d 的含义同三参数版本；inv 指定正变换或逆变换。
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
    // a 是按掩码编号的一维集合函数；原地完成 OR 正变换或逆变换。
    assert(!a.empty() && a.size() <= UINT32_MAX && has_single_bit(a.size())); // 调试检查，可删。
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
    int n = a.size();
    assert(n > 0 && has_single_bit((unsigned long long)n)); // 调试检查，可删。
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

template <uint32_t P> uint32_t fwtAdd(uint32_t x, uint32_t y)
{
    x += y;
    if (x >= P)
    {
        x -= P;
    }
    return x;
}

template <uint32_t P> uint32_t fwtSub(uint32_t x, uint32_t y)
{
    x += P - y;
    if (x >= P)
    {
        x -= P;
    }
    return x;
}

template <uint32_t P, BitOp Op, bool Inv, uint32_t N> void fwtRec(uint32_t *a)
{
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
                    s = (s + (s & 1) * P) >> 1;
                    d = (d + (d & 1) * P) >> 1;
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
    static_assert(1 < P && P < (1U << 31));
    static_assert(Op != BitOp::Xor || (P & 1));
    assert(a && n && has_single_bit((unsigned long long)n)); // 调试检查，可删
    if constexpr (N <= (1U << 30))
    {
        if (n == N)
        {
            fwtRec<P, Op, Inv, N>(a);
        }
        else
        {
            fwtMod<P, Op, Inv, N << 1>(a, n);
        }
    }
}

template <uint32_t P, BitOp Op, bool Inv = false> void fwtMod(vector<uint32_t> &a)
{
    fwtMod<P, Op, Inv>(a.data(), a.size());
}

template <uint32_t P, BitOp Op>
vector<uint32_t> bitConvMod(vector<uint32_t> a, vector<uint32_t> b)
{
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

// 稀疏 XOR 卷积使用的固定大素数；其乘积必须用 __int128 承载。
constexpr uint64_t sparseXorMod = 1000000000000125953ULL;

class SparseXorMint
{
private:
    uint64_t x = 0;

    static uint64_t mulRaw(uint64_t a, uint64_t b)
    {
        return (uint64_t)((unsigned __int128)a * b % sparseXorMod);
    }

public:
    SparseXorMint() = default;

    // v 是任意有符号整数；返回其在 sparseXorMod 下的最小非负代表元。
    SparseXorMint(long long v)
    {
        v %= (long long)sparseXorMod;
        if (v < 0)
        {
            v += sparseXorMod;
        }
        x = (uint64_t)v;
    }

    // v 是已在 [0,sparseXorMod) 中的代表元；用于内部运算避免重复取模。
    static SparseXorMint raw(uint64_t v)
    {
        SparseXorMint r;
        r.x = v;
        return r;
    }

    uint64_t val() const
    {
        return x;
    }

    SparseXorMint operator-() const
    {
        return raw(x ? sparseXorMod - x : 0);
    }

    SparseXorMint &operator+=(const SparseXorMint &o)
    {
        x += o.x;
        if (x >= sparseXorMod)
        {
            x -= sparseXorMod;
        }
        return *this;
    }

    SparseXorMint &operator-=(const SparseXorMint &o)
    {
        x += sparseXorMod - o.x;
        if (x >= sparseXorMod)
        {
            x -= sparseXorMod;
        }
        return *this;
    }

    SparseXorMint &operator*=(const SparseXorMint &o)
    {
        x = mulRaw(x, o.x);
        return *this;
    }

    friend SparseXorMint operator+(SparseXorMint a, const SparseXorMint &b)
    {
        return a += b;
    }

    friend SparseXorMint operator-(SparseXorMint a, const SparseXorMint &b)
    {
        return a -= b;
    }

    friend SparseXorMint operator*(SparseXorMint a, const SparseXorMint &b)
    {
        return a *= b;
    }

    friend bool operator==(const SparseXorMint &a, const SparseXorMint &b)
    {
        return a.x == b.x;
    }

    friend bool operator!=(const SparseXorMint &a, const SparseXorMint &b)
    {
        return a.x != b.x;
    }

    // e 是非负指数；返回当前值的 e 次幂。
    SparseXorMint pow(uint64_t e) const
    {
        SparseXorMint a = *this, r = 1;
        while (e)
        {
            if (e & 1)
            {
                r *= a;
            }
            a *= a;
            e >>= 1;
        }
        return r;
    }

    // 当前值非零且模数为素数；返回乘法逆元。
    SparseXorMint inv() const
    {
        assert(x != 0); // 调试检查，可删
        return pow(sparseXorMod - 2);
    }
};

class SparseXorInfo
{
private:
    // t 是固定的二次非剩余，invt 是其逆元。
    static SparseXorMint t, invt;
    static int maxBit;

    static int legendre(const SparseXorMint &x)
    {
        SparseXorMint z = x.pow((sparseXorMod - 1) / 2);
        if (z == SparseXorMint(1))
        {
            return 1;
        }
        if (z == SparseXorMint(0))
        {
            return 0;
        }
        return -1;
    }

    static SparseXorMint sqrt(const SparseXorMint &x)
    {
        assert(legendre(x) >= 0); // 调试检查，可删
        if (x == SparseXorMint(0))
        {
            return 0;
        }
        // Cipolla：在 F_p[sqrt(w)] 中做 (a+sqrt(w))^((p+1)/2)。
        SparseXorMint w2;
        SparseXorMint a = 0;
        while (true)
        {
            w2 = a * a - x;
            if (legendre(w2) == -1)
            {
                break;
            }
            a += SparseXorMint(1);
        }
        auto mul = [w2](pair<SparseXorMint, SparseXorMint> u,
                        pair<SparseXorMint, SparseXorMint> v)
        {
            return make_pair(u.first * v.first + u.second * v.second * w2,
                             u.first * v.second + u.second * v.first);
        };
        pair<SparseXorMint, SparseXorMint> r{1, 0}, b{a, 1};
        uint64_t e = (sparseXorMod + 1) / 2;
        while (e)
        {
            if (e & 1)
            {
                r = mul(r, b);
            }
            b = mul(b, b);
            e >>= 1;
        }
        return r.first;
    }

    SparseXorInfo(SparseXorMint p, SparseXorMint q, long long e, int zero)
        : xp(p), xq(q), power(e), zeroCount(zero) {}

public:
    SparseXorMint xp = 1, xq = 1;
    long long power = 0;
    int zeroCount = 0;

    SparseXorInfo() = default;

    // maxBit 是所有稀疏因子中 d-1 的最大值，决定 power 的位权缩放。
    static void setMaxBit(int k)
    {
        maxBit = k;
    }

    // x 是一次局部 Walsh 变换值，bits 是该因子的有效维数。
    SparseXorInfo(SparseXorMint x, int bits) : xp(x)
    {
        if (x == SparseXorMint(0))
        {
            xp = 1;
            zeroCount = 1;
            return;
        }
        for (int i = 0; i < bits; i++)
        {
            if (legendre(xp) == 1)
            {
                xp = sqrt(xp);
            }
            else
            {
                xp = sqrt(xp * invt);
                power |= 1LL << i;
            }
        }
        power <<= maxBit - bits;
    }

    SparseXorInfo operator*(const SparseXorInfo &o) const
    {
        return {xp * o.xp, xq * o.xq, power + o.power, zeroCount + o.zeroCount};
    }

    SparseXorInfo operator/(const SparseXorInfo &o) const
    {
        return {xp * o.xq, xq * o.xp, power - o.power, zeroCount - o.zeroCount};
    }

    SparseXorInfo &operator*=(const SparseXorInfo &o)
    {
        xp *= o.xp;
        xq *= o.xq;
        power += o.power;
        zeroCount += o.zeroCount;
        return *this;
    }

    SparseXorInfo &operator/=(const SparseXorInfo &o)
    {
        xp *= o.xq;
        xq *= o.xp;
        power -= o.power;
        zeroCount -= o.zeroCount;
        return *this;
    }

    // 把内部平方根分解还原成模 sparseXorMod 下的值。
    SparseXorMint value() const
    {
        if (zeroCount)
        {
            return 0;
        }
        SparseXorMint r = xp * xq.inv();
        if (power > 0)
        {
            r *= t.pow(power >> maxBit);
        }
        else if (power < 0)
        {
            r *= invt.pow((-power) >> maxBit);
        }
        return r;
    }

    static void init()
    {
        SparseXorMint a = 0, w2;
        while (true)
        {
            w2 = a * a - SparseXorMint(1);
            if (legendre(w2) == -1)
            {
                t = w2;
                break;
            }
            a += SparseXorMint(1);
        }
        invt = t.inv();
    }
};

inline SparseXorMint SparseXorInfo::t;
inline SparseXorMint SparseXorInfo::invt;
inline int SparseXorInfo::maxBit = 0;

// factors 每项是一个稀疏 Walsh 因子，pair 为 {掩码,系数}，最后一项作为基准项。
// 返回长度 2^n 的异或卷积结果，所有系数均按 sparseXorMod 取模。
inline vector<uint64_t> sparseXorConvolution(int n,
                                             const vector<vector<pair<int, long long>>> &factors)
{
    assert(0 <= n && n < 30); // 调试检查，可删
    int size = 1LL << n;
    int maxBit = 0;
    for (const auto &factor : factors)
    {
        assert(!factor.empty());
        maxBit = max(maxBit, (int)factor.size() - 1);
    }
    assert(maxBit < 60); // power 使用有符号 64 位位集保存平方根分支。
    SparseXorInfo::setMaxBit(maxBit);
    SparseXorInfo::init();
    vector<SparseXorInfo> all(size);
    int delta = 0;
    for (const auto &factor : factors)
    {
        int bits = factor.size() - 1;
        assert(bits < 60); // 调试检查，可删
        int base = factor.back().first;
        SparseXorMint offset = factor.back().second;
        delta ^= base;
        int localSize = 1LL << bits;
        vector<int> mask(localSize);
        vector<SparseXorMint> walsh(localSize);
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
        vector<SparseXorInfo> local(localSize);
        for (int s = 0; s < localSize; s++)
        {
            local[s] = SparseXorInfo(walsh[s], bits);
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
    vector<SparseXorMint> ans(size);
    for (int i = 0; i < size; i++)
    {
        ans[i] = all[i].value();
    }
    SparseXorMint inv2 = SparseXorMint(2).inv();
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
    vector<uint64_t> result(size);
    for (int i = 0; i < size; i++)
    {
        result[i] = ans[i ^ delta].val();
    }
    return result;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;
    vector<vector<pair<int, long long>>> factors(m);
    for (auto &factor : factors)
    {
        int d;
        cin >> d;
        factor.resize(d);
        for (auto &[mask, value] : factor)
        {
            cin >> mask;
        }
        for (auto &[mask, value] : factor)
        {
            cin >> value;
        }
    }
    for (uint64_t x : sparseXorConvolution(n, factors))
    {
        cout << x << ' ';
    }
}
