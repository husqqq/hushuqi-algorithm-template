



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

constexpr int NTT_LIM = 1 << 23;

bool nttFit(int n, int m)
{
    // n、m 是两输入序列长度；返回一次不超过 NTT_LIM 的变换能否覆盖其线性卷积。
    assert(n >= 0 && m >= 0); // 调试检查，可删。
    return n == 0 || m == 0 || (__int128)n + m - 1 <= NTT_LIM;
}

bool convFit(int n, int m)
{
    // n、m 是两输入序列长度；返回公共 conv 能否计算其线性卷积。
    // 结果只超上限一至两项时，conv 会拆掉两端最高项再做一次较短卷积。
    assert(n >= 0 && m >= 0); // 调试检查，可删。
    return n == 0 || m == 0 || min(n, m) <= 64 ||
           (__int128)n + m - 1 <= NTT_LIM + 2;
}

class FastNTT
{
    // root[i]、iroot[i] 分别保存 2 的 i 次单位根及其逆元。
    static inline array<uint32_t, 24> root;
    static inline array<uint32_t, 24> iroot;
    // rate2、irate2 保存二进制蝶形跨块时的旋转因子更新量。
    static inline array<uint32_t, 22> rate2;
    static inline array<uint32_t, 22> irate2;
    // rate3、irate3 保存四进制蝶形跨块时的旋转因子更新量。
    static inline array<uint32_t, 21> rate3;
    static inline array<uint32_t, 21> irate3;
    // ready 标记单位根表是否已经初始化。
    static inline bool ready = false;

    static void init()
    {
        // 无参数；首次调用时预处理各阶单位根和跨块旋转因子，后续调用直接复用。
        if (ready)
        {
            return;
        }
        ready = true;
        root[23] = 31;
        iroot[23] = Z(root[23]).inv().val();
        for (int i = 22; i >= 0; i--)
        {
            root[i] = (uint64_t)root[i + 1] * root[i + 1] % mod;
            iroot[i] = (uint64_t)iroot[i + 1] * iroot[i + 1] % mod;
        }
        uint32_t prod = 1;
        uint32_t iprod = 1;
        for (int i = 0; i <= 21; i++)
        {
            rate2[i] = (uint64_t)root[i + 2] * prod % mod;
            irate2[i] = (uint64_t)iroot[i + 2] * iprod % mod;
            prod = (uint64_t)prod * iroot[i + 2] % mod;
            iprod = (uint64_t)iprod * root[i + 2] % mod;
        }
        prod = 1;
        iprod = 1;
        for (int i = 0; i <= 20; i++)
        {
            rate3[i] = (uint64_t)root[i + 3] * prod % mod;
            irate3[i] = (uint64_t)iroot[i + 3] * iprod % mod;
            prod = (uint64_t)prod * iroot[i + 3] % mod;
            iprod = (uint64_t)iprod * root[i + 3] % mod;
        }
    }

  public:
    static void dft(Z *a, int n)
    {
        // a 指向长度 n 的连续系数，n 是至多 2^23 的正 2 的幂；原地完成正变换。
        // 长度 n 的相邻位置 2i、2i+1 对应 z、-z；前者与长度 n/2 变换的第 i 个频点对应同一个 z^2。
        assert(n > 0 && has_single_bit((unsigned long long)n) && n <= NTT_LIM); // 调试检查，可删。
        init();
        int z = n;
        int h = countr_zero((uint32_t)z);
        int len = 0;
        while (len < h)
        {
            if (h - len == 1)
            {
                int p = 1 << (h - len - 1);
                uint32_t rot = 1;
                for (int s = 0; s < (1 << len); s++)
                {
                    int off = s << (h - len);
                    for (int i = 0; i < p; i++)
                    {
                        uint32_t l = a[off + i].x;
                        uint32_t r = (uint64_t)a[off + i + p].x * rot % mod;
                        a[off + i].x = l + r < mod ? l + r : l + r - mod;
                        a[off + i + p].x = l >= r ? l - r : l + mod - r;
                    }
                    if (s + 1 != (1 << len))
                    {
                        rot = (uint64_t)rot * rate2[countr_zero((uint32_t)~s)] % mod;
                    }
                }
                len++;
            }
            else
            {
                int p = 1 << (h - len - 2);
                uint32_t rot = 1;
                uint32_t imag = root[2];
                for (int s = 0; s < (1 << len); s++)
                {
                    uint32_t rot2 = (uint64_t)rot * rot % mod;
                    uint32_t rot3 = (uint64_t)rot2 * rot % mod;
                    int off = s << (h - len);
                    for (int i = 0; i < p; i++)
                    {
                        uint64_t m2 = (uint64_t)mod * mod;
                        uint64_t a0 = a[off + i].x;
                        uint64_t a1 = (uint64_t)a[off + i + p].x * rot;
                        uint64_t a2 = (uint64_t)a[off + i + 2 * p].x * rot2;
                        uint64_t a3 = (uint64_t)a[off + i + 3 * p].x * rot3;
                        uint64_t x = (uint64_t)Z(a1 + m2 - a3).val() * imag;
                        uint64_t na2 = m2 - a2;
                        a[off + i].x = (a0 + a2 + a1 + a3) % mod;
                        a[off + i + p].x = (a0 + a2 + 2 * m2 - a1 - a3) % mod;
                        a[off + i + 2 * p].x = (a0 + na2 + x) % mod;
                        a[off + i + 3 * p].x = (a0 + na2 + m2 - x) % mod;
                    }
                    if (s + 1 != (1 << len))
                    {
                        rot = (uint64_t)rot * rate3[countr_zero((uint32_t)~s)] % mod;
                    }
                }
                len += 2;
            }
        }
    }

    static void dft(vector<Z> &a)
    {
        // a 的长度是至多 2^23 的正 2 的幂；转发到连续内存正变换，无返回值。
        dft(a.data(), a.size());
    }

    static void idft(Z *a, int n)
    {
        // a 指向本类 dft 产生的 n 个频域值；原地逆变换、除以 n 并恢复自然顺序系数。
        assert(n > 0 && has_single_bit((unsigned long long)n) && n <= NTT_LIM); // 调试检查，可删。
        init();
        int z = n;
        int h = countr_zero((uint32_t)z);
        int len = h;
        while (len)
        {
            if (len == 1)
            {
                int p = 1 << (h - len);
                uint32_t rot = 1;
                for (int s = 0; s < (1 << (len - 1)); s++)
                {
                    int off = s << (h - len + 1);
                    for (int i = 0; i < p; i++)
                    {
                        uint32_t l = a[off + i].x;
                        uint32_t r = a[off + i + p].x;
                        a[off + i].x = l + r < mod ? l + r : l + r - mod;
                        a[off + i + p].x = (uint64_t)(l + mod - r) * rot % mod;
                    }
                    if (s + 1 != (1 << (len - 1)))
                    {
                        rot = (uint64_t)rot * irate2[countr_zero((uint32_t)~s)] % mod;
                    }
                }
                len--;
            }
            else
            {
                int p = 1 << (h - len);
                uint32_t rot = 1;
                uint32_t imag = iroot[2];
                for (int s = 0; s < (1 << (len - 2)); s++)
                {
                    uint32_t rot2 = (uint64_t)rot * rot % mod;
                    uint32_t rot3 = (uint64_t)rot2 * rot % mod;
                    int off = s << (h - len + 2);
                    for (int i = 0; i < p; i++)
                    {
                        uint64_t a0 = a[off + i].x;
                        uint64_t a1 = a[off + i + p].x;
                        uint64_t a2 = a[off + i + 2 * p].x;
                        uint64_t a3 = a[off + i + 3 * p].x;
                        uint64_t x = Z((mod + a2 - a3) * imag).val();
                        a[off + i].x = (a0 + a1 + a2 + a3) % mod;
                        a[off + i + p].x = (a0 + mod - a1 + x) % mod * rot % mod;
                        a[off + i + 2 * p].x = (a0 + a1 + 2 * mod - a2 - a3) % mod * rot2 % mod;
                        a[off + i + 3 * p].x = (a0 + 2 * mod - a1 - x) % mod * rot3 % mod;
                    }
                    if (s + 1 != (1 << (len - 2)))
                    {
                        rot = (uint64_t)rot * irate3[countr_zero((uint32_t)~s)] % mod;
                    }
                }
                len -= 2;
            }
        }
        Z iv = Z(n).inv();
        for (int i = 0; i < z; i++)
        {
            a[i] *= iv;
        }
    }

    static void idft(vector<Z> &a)
    {
        // a 必须来自本类 dft；转发到连续内存逆变换，无返回值。
        idft(a.data(), a.size());
    }

    static vector<Z> conv(vector<Z> a, vector<Z> b)
    {
        // a、b 按低次到高次保存系数；返回普通线性卷积，任一输入为空时返回空。
        if (a.empty() || b.empty())
        {
            return {};
        }
        int n = a.size();
        int m = b.size();
        int s = n + m - 1;
        if (min(n, m) <= 64)
        {
            vector<Z> c(s);
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    c[i + j] += a[i] * b[j];
                }
            }
            return c;
        }
        assert(convFit(n, m)); // 调试检查，可删。
        int z = bit_ceil((unsigned long long)s);
        if (s - 2 <= z / 2)
        {
            Z al = a.back();
            Z bl = b.back();
            a.pop_back();
            b.pop_back();
            auto x = a;
            auto y = b;
            auto c = conv(move(a), move(b));
            c.resize(s);
            c[s - 1] = al * bl;
            for (int i = 0; i < n - 1; i++)
            {
                c[i + m - 1] += x[i] * bl;
            }
            for (int i = 0; i < m - 1; i++)
            {
                c[i + n - 1] += y[i] * al;
            }
            return c;
        }
        bool same = a == b;
        a.resize(z);
        b.resize(z);
        dft(a);
        if (same)
        {
            b = a;
        }
        else
        {
            dft(b);
        }
        for (int i = 0; i < z; i++)
        {
            a[i] *= b[i];
        }
        idft(a);
        a.resize(s);
        return a;
    }
};

class NTT32
{
    // 本类服务原始 uint32_t 切片，复用位逆序表与 uint64_t 工作区，并延迟蝶形加减结果的归一化。
    // 它与 FastNTT 的 Z 容器接口用途不同；2^17 上限保证最终归一化乘法不会溢出 uint64_t。
    int mx = 0; // 允许的最大变换长度，所有实际长度必须整除 mx。
    vector<uint32_t> w; // w[l..2l) 保存长度 2l 蝶形使用的正单位根。
    vector<uint64_t> buf; // 蝶形使用的延迟取模工作区。
    array<vector<uint32_t>, 24> rev; // rev[k] 保存长度 2^k 的二进制翻转下标。

    const vector<uint32_t> &getRev(int n)
    {
        // n 是不超过 mx 的二次幂；返回对应二进制翻转下标表的引用。
        int k = countr_zero((unsigned long long)n);
        auto &r = rev[k];
        if (r.empty())
        {
            r.resize(n);
            for (int i = 1; i < n; i++)
            {
                r[i] = (r[i >> 1] >> 1) | ((uint32_t)(i & 1) << (k - 1));
            }
        }
        return r;
    }

    void trans(uint32_t *a, int n, bool inv)
    {
        // a 指向长度 n 且各项在 [0,mod) 内的预分配切片，inv 表示是否逆变换；使用 uint64_t 工作区原地完成变换。
        assert(n > 0 && has_single_bit((unsigned long long)n) && n <= mx && mx % n == 0); // 调试检查，可删。
        const auto &r = getRev(n);
        for (int i = 0; i < n; i++)
        {
            buf[i] = a[r[i]];
        }
        for (int h = 1; h < n; h <<= 1)
        {
            const uint32_t *o = w.data() + h;
            for (int l = 0; l < n; l += 2 * h)
            {
                for (int j = 0; j < h; j++)
                {
                    uint64_t x = buf[l + j];
                    uint64_t y = buf[l + j + h] * o[j] % mod;
                    buf[l + j] = x + y;
                    buf[l + j + h] = x + mod - y;
                }
            }
        }
        if (!inv)
        {
            for (int i = 0; i < n; i++)
            {
                a[i] = buf[i] % mod;
            }
            return;
        }
        int k = countr_zero((unsigned long long)n);
        uint32_t iv = mod - ((mod - 1) >> k);
        a[0] = buf[0] * iv % mod;
        for (int i = 1; i < n; i++)
        {
            a[i] = buf[n - i] * iv % mod;
        }
    }

  public:
    NTT32(int n = 0) : mx(n), w(n), buf(n)
    {
        // n 是最大变换长度，必须为 0 或不超过 2^17 的二次幂；预处理各级连续单位根。
        assert(n == 0 || (has_single_bit((unsigned long long)n) && n <= (1 << 17))); // 调试检查，可删。
        if (n == 0)
        {
            return;
        }
        if (n == 1)
        {
            w[0] = 1;
            return;
        }
        uint32_t r = Z(3).pow((mod - 1) / n).val();
        w[n / 2] = 1;
        for (int i = n / 2 + 1; i < n; i++)
        {
            w[i] = (uint64_t)w[i - 1] * r % mod;
        }
        for (int i = n / 2 - 1; i; i--)
        {
            w[i] = w[i << 1];
        }
    }

    void dft(uint32_t *a, int n)
    {
        // a 指向长度 n 且各项在 [0,mod) 内的预分配切片；原地完成自然频点顺序的正变换，无返回值。
        trans(a, n, false);
    }

    void idft(uint32_t *a, int n)
    {
        // a 指向本对象 dft 产生的 n 个频域值；原地完成逆变换并除以 n，无返回值。
        trans(a, n, true);
    }
};

void dft(vector<Z> &a)
{
    // a 的长度是至多 2^23 的正 2 的幂；调用卡常正变换，结果满足 FastNTT 的递归配对排列。
    FastNTT::dft(a);
}

void dft(Z *a, int n)
{
    // a 指向长度 n 的连续系数；调用卡常正变换，结果满足 FastNTT 的递归配对排列。
    FastNTT::dft(a, n);
}

void idft(vector<Z> &a)
{
    // a 必须来自全局 dft；调用卡常逆变换并恢复自然顺序系数。
    FastNTT::idft(a);
}

void idft(Z *a, int n)
{
    // a 指向长度 n 且来自全局 dft 的频域值；调用卡常逆变换并恢复自然顺序系数。
    FastNTT::idft(a, n);
}

vector<Z> conv(vector<Z> a, vector<Z> b)
{
    // a、b 按低次到高次保存系数；返回卡常 NTT 计算的普通线性卷积。
    return FastNTT::conv(move(a), move(b));
}

namespace plain_ntt
{
void dft(vector<Z> &a)
{
    // a 的长度是至多 2^23 的正 2 的幂；原地完成自然顺序的正 NTT。
    int n = a.size();
    assert(n > 0 && has_single_bit((unsigned long long)n) && n <= NTT_LIM); // 调试检查，可删。
    int rev = 0;
    for (int i = 1; i < n; i++)
    {
        int b = n >> 1;
        for (; rev & b; b >>= 1)
        {
            rev ^= b;
        }
        rev ^= b;
        if (i < rev)
        {
            swap(a[i], a[rev]);
        }
    }
    for (int len = 2; len <= n; len <<= 1)
    {
        Z w = Z(3).pow((mod - 1) / len);
        for (int i = 0; i < n; i += len)
        {
            Z x = 1;
            for (int j = 0; j < len / 2; j++)
            {
                Z u = a[i + j];
                Z v = a[i + j + len / 2] * x;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                x *= w;
            }
        }
    }
}

void idft(vector<Z> &a)
{
    // a 是自然顺序的频域值；原地完成逆 NTT 并除以长度。
    assert(!a.empty() && has_single_bit((unsigned long long)a.size()) && a.size() <= NTT_LIM); // 调试检查，可删。
    reverse(a.begin() + 1, a.end());
    plain_ntt::dft(a);
    Z iv = Z(a.size()).inv();
    for (auto &x : a)
    {
        x *= iv;
    }
}

vector<Z> conv(vector<Z> a, vector<Z> b)
{
    // a、b 按低次到高次保存系数；返回普通线性卷积，任一输入为空时返回空。
    if (a.empty() || b.empty())
    {
        return {};
    }
    int s = a.size() + b.size() - 1;
    if (min(a.size(), b.size()) <= 32)
    {
        vector<Z> c(s);
        for (int i = 0; i < (int)a.size(); i++)
        {
            for (int j = 0; j < (int)b.size(); j++)
            {
                c[i + j] += a[i] * b[j];
            }
        }
        return c;
    }
    assert(nttFit(a.size(), b.size())); // 调试检查，可删。
    int n = bit_ceil((unsigned long long)s);
    a.resize(n);
    b.resize(n);
    plain_ntt::dft(a);
    plain_ntt::dft(b);
    for (int i = 0; i < n; i++)
    {
        a[i] *= b[i];
    }
    plain_ntt::idft(a);
    a.resize(s);
    return a;
}
}

struct Poly : vector<Z>
{
    using vector<Z>::vector;
    Poly(const vector<Z> &a = {}) : vector<Z>(a)
    {
        // a 按低次到高次给出系数；构造同系数多项式，无返回值。
    }

    Poly(vector<Z> &&a) : vector<Z>(move(a))
    {
        // a 按低次到高次给出系数；移动构造同系数多项式。
    }

    void norm()
    {
        // 无参数；删除末尾多余零系数，无返回值。
        while (!empty() && back() == Z(0))
        {
            pop_back();
        }
    }

    Poly cut(int n) const
    {
        // n 是非负截断长度；返回恰含前 n 项的形式幂级数。
        assert(n >= 0); // 调试检查，可删。
        Poly a(begin(), begin() + min(n, (int)size()));
        a.resize(n);
        return a;
    }

    Poly &operator+=(const Poly &b)
    {
        // b 是要相加的多项式；返回更新后的当前多项式。
        resize(max(size(), b.size()));
        for (int i = 0; i < (int)b.size(); i++)
        {
            (*this)[i] += b[i];
        }
        return *this;
    }

    Poly &operator-=(const Poly &b)
    {
        // b 是要减去的多项式；返回更新后的当前多项式。
        resize(max(size(), b.size()));
        for (int i = 0; i < (int)b.size(); i++)
        {
            (*this)[i] -= b[i];
        }
        return *this;
    }

    Poly &operator*=(const Poly &b)
    {
        // b 是乘数多项式；返回卷积后的当前多项式。
        if (this == &b)
        {
            Poly a = b;
            return *this = conv(move(a), move(*this));
        }
        return *this = conv(move(*this), b);
    }

    Poly &operator*=(Z k)
    {
        // k 是标量系数；返回所有系数乘 k 后的当前多项式。
        for (auto &x : *this)
        {
            x *= k;
        }
        return *this;
    }

    friend Poly operator+(Poly a, const Poly &b)
    {
        // a、b 是两个多项式；返回系数和。
        return a += b;
    }

    friend Poly operator-(Poly a, const Poly &b)
    {
        // a、b 是两个多项式；返回系数差。
        return a -= b;
    }

    friend Poly operator*(Poly a, const Poly &b)
    {
        // a、b 是两个多项式；返回多项式乘积。
        return a *= b;
    }

    friend Poly operator*(Poly a, Z k)
    {
        // a 是多项式，k 是标量；返回缩放后的多项式。
        return a *= k;
    }

    friend Poly operator*(Z k, Poly a)
    {
        // k 是标量，a 是多项式；返回缩放后的多项式。
        return a *= k;
    }
};

Poly midProd(const Poly &a, const Poly &b)
{
    // a、b 满足 a.size()>=b.size()>0；返回 c[i]=sum_j b[j]*a[i+j] 的中段乘积。
    assert(!b.empty() && a.size() >= b.size()); // 调试检查，可删。
    int m = a.size() - b.size() + 1;
    if (min((int)b.size(), m) <= 64)
    {
        Poly c(m);
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < (int)b.size(); j++)
            {
                c[i] += b[j] * a[i + j];
            }
        }
        return c;
    }
    assert(nttFit(b.size(), m)); // 调试检查，可删。
    int n = bit_ceil((unsigned long long)a.size());
    Poly x = a;
    Poly y(b.rbegin(), b.rend());
    x.resize(n);
    y.resize(n);
    dft(x);
    dft(y);
    for (int i = 0; i < n; i++)
    {
        x[i] *= y[i];
    }
    idft(x);
    return Poly(x.begin() + b.size() - 1, x.begin() + a.size());
}

Poly fpsInv(const Poly &a, int n)
{
    // a 是输入 FPS，n 是 [0,2^22] 内的截断长度；n>0 时 a[0] 必须非零，返回满足 a*g=1 mod x^n 的 g。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    assert(!a.empty() && a[0] != Z(0)); // 调试检查，可删。
    Poly g{a[0].inv()};
    for (int k = 2; k < 2 * n; k <<= 1)
    {
        g = (g * (Poly{2} - a.cut(k) * g)).cut(k);
    }
    return g.cut(n);
}

Poly fpsInvFast(const Poly &a, int n)
{
    // a 是输入 FPS，n 是 [0,2^23] 内的截断长度；n>0 时 a[0] 必须非零，返回满足 a*g=1 mod x^n 的快速逆元。
    assert(n >= 0 && n <= (1 << 23)); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    assert(!a.empty() && a[0] != Z(0)); // 调试检查，可删。

    int m = min<int>(n, 64); // m 是当前已经求出的逆元项数。
    Poly g(m);
    g[0] = a[0].inv();
    for (int i = 1; i < m; i++)
    {
        Z sum = 0;
        for (int j = 1; j <= i && j < (int)a.size(); j++)
        {
            sum += a[j] * g[i - j];
        }
        g[i] = -sum * g[0];
    }

    vector<Z> fg; // fg 保存当前 g 的频域值，供本轮两次乘法复用。
    vector<Z> err; // err 依次保存 a、ag 的高半误差和最终校正乘积。
    int cap = bit_ceil((unsigned long long)n);
    fg.reserve(cap);
    err.reserve(cap);
    while (m < n)
    {
        int k = min(2 * m, n); // k 是本轮要求的逆元项数。
        int z = 2 * m; // z 是本轮循环卷积的 NTT 长度。
        fg.assign(z, Z(0));
        err.assign(z, Z(0));
        copy(g.begin(), g.end(), fg.begin());
        copy(a.begin(), a.begin() + min(k, (int)a.size()), err.begin());

        dft(fg);
        dft(err);
        for (int i = 0; i < z; i++)
        {
            err[i] *= fg[i];
        }
        idft(err);
        fill(err.begin(), err.begin() + m, Z(0));
        fill(err.begin() + k, err.end(), Z(0));

        dft(err);
        for (int i = 0; i < z; i++)
        {
            err[i] *= fg[i];
        }
        idft(err);

        g.resize(k);
        for (int i = m; i < k; i++)
        {
            g[i] = -err[i];
        }
        m = k;
    }
    return g;
}

pair<Poly, Poly> polyDivmod(Poly a, Poly b)
{
    // a 是被除式，b 是非零除式；返回 {商,余式}，余式次数严格小于 b。
    a.norm();
    b.norm();
    assert(!b.empty()); // 调试检查，可删。
    if (a.size() < b.size())
    {
        return {{}, a};
    }
    int n = a.size() - b.size() + 1;
    if (a.size() <= 256 || b.size() <= 256)
    {
        Poly q(n);
        Z iv = b.back().inv();
        for (int i = n - 1; i >= 0; i--)
        {
            q[i] = a[i + b.size() - 1] * iv;
            for (int j = 0; j < (int)b.size(); j++)
            {
                a[i + j] -= q[i] * b[j];
            }
        }
        a.resize(b.size() - 1);
        q.norm();
        a.norm();
        return {move(q), move(a)};
    }
    assert(convFit(n, n) && convFit(b.size(), n)); // 调试检查，可删。
    Poly x = a;
    Poly y = b;
    reverse(x.begin(), x.end());
    reverse(y.begin(), y.end());
    Poly q = (x.cut(n) * fpsInvFast(y, n)).cut(n);
    reverse(q.begin(), q.end());
    Poly r = a - b * q;
    r.resize(b.size() - 1);
    q.norm();
    r.norm();
    return {q, r};
}


#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T> vector<T> recMul(const vector<T> &a, const vector<T> &b,
                                    const vector<T> &rec)
{
    // a、b 是次数小于 k 的系数；返回乘积模特征多项式后的 k 项系数。
    int k = rec.size();
    assert(k > 0 && (int)a.size() == k && (int)b.size() == k); // 调试检查，可删
    vector<T> c(2 * k - 1);
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            c[i + j] += a[i] * b[j];
        }
    }
    for (int i = 2 * k - 2; i >= k; i--)
    {
        for (int j = 1; j <= k; j++)
        {
            c[i - j] += c[i] * rec[j - 1];
        }
    }
    c.resize(k);
    return c;
}

template <class T>
vector<T> recCoef(const vector<T> &rec, unsigned long long n)
{
    // 返回 x^n 模特征多项式的 k 个系数，系数下标对应 x^0..x^(k-1)。
    int k = rec.size();
    assert(k > 0); // 调试检查，可删
    vector<T> ans(k), x(k);
    ans[0] = T(1);
    if (k == 1)
    {
        x[0] = rec[0];
    }
    else
    {
        x[1] = T(1);
    }
    while (n)
    {
        if (n & 1)
        {
            ans = recMul(ans, x, rec);
        }
        n >>= 1;
        if (n)
        {
            x = recMul(x, x, rec);
        }
    }
    return ans;
}

template <class T> T linearRec(const vector<T> &init, const vector<T> &rec,
                               unsigned long long n)
{
    // init 是至少 k 项初值，rec 定义 a[n]=sum(rec[i-1]*a[n-i])，n 是下标；返回 a[n]。
    int k = rec.size();
    assert(k > 0 && (int)init.size() >= k); // 调试检查，可删。
    if (n < (unsigned long long)k)
    {
        return init[n];
    }
    auto ans = recCoef(rec, n);
    T res{};
    for (int i = 0; i < k; i++)
    {
        res += ans[i] * init[i];
    }
    return res;
}


pair<Poly, Poly> recurrenceFraction(const vector<Z> &init, const vector<Z> &rec)
{
    // init 是至少 k 项初值，rec 定义 a[n]=sum(rec[i-1]*a[n-i])；返回生成函数 P(x)/Q(x)。
    int k = rec.size();
    assert(k > 0 && (int)init.size() >= k); // 调试检查，可删。
    Poly q(k + 1);
    q[0] = 1;
    for (int i = 0; i < k; i++)
    {
        q[i + 1] = -rec[i];
    }
    Poly a(init.begin(), init.begin() + k);
    Poly p = (a * q).cut(k);
    p.norm();
    return {p, q};
}

pair<Poly, Poly> bmStep(Poly q)
{
    // q 的首尾系数非零；返回 {q(-x),v(x)}，其中 q(x)q(-x)=v(x^2)。
    assert(!q.empty() && q[0] != Z(0) && q.back() != Z(0)); // 调试检查，可删。
    Poly neg = q;
    for (int i = 1; i < (int)neg.size(); i += 2)
    {
        neg[i] = -neg[i];
    }
    Poly a = conv(move(q), neg);
    Poly nq((a.size() + 1) / 2);
    for (int i = 0; i < (int)nq.size(); i++)
    {
        nq[i] = a[2 * i];
    }
    return {move(neg), move(nq)};
}

Z bostanMori(Poly p, Poly q, unsigned long long n)
{
    // p、q 是分子分母且 q[0] 非零，n 是所求下标；返回 [x^n]p(x)/q(x)。
    p.norm();
    q.norm();
    assert(!q.empty() && q[0] != Z(0)); // 调试检查，可删。
    while (n)
    {
        auto [neg, nq] = bmStep(move(q));
        Poly a = conv(move(p), move(neg));
        Poly np;
        np.reserve((a.size() + 1) / 2);
        for (int i = n & 1; i < (int)a.size(); i += 2)
        {
            np.push_back(a[i]);
        }
        p = move(np);
        q = move(nq);
        n >>= 1;
    }
    return p.empty() ? Z(0) : p[0] / q[0];
}

Poly invTail(Poly q, unsigned long long k)
{
    // q 的次数 d>0 且首尾系数非零；返回 [x^k..x^(k+d-1)]1/q(x)。
    q.norm();
    assert(q.size() > 1 && q[0] != Z(0) && q.back() != Z(0)); // 调试检查，可删。
    int d = q.size() - 1;
    assert(d <= NTT_LIM); // 调试检查，可删。
    if (k <= (unsigned long long)d)
    {
        assert(k <= (unsigned long long)(NTT_LIM - d)); // 调试检查，可删。
    }
    else
    {
        assert(convFit(d + 1, 2 * d - 1)); // 调试检查，可删。
    }
    vector<pair<Poly, int>> st;
    st.reserve(64);
    while (k > (unsigned long long)d)
    {
        auto [neg, nq] = bmStep(move(q));
        int t = (k - (unsigned long long)d) & 1ULL;
        st.push_back({move(neg), t});
        q = move(nq);
        k = (k - (unsigned long long)d + t) / 2;
    }

    int s = k;
    Poly a = fpsInvFast(q, s + d);
    a.erase(a.begin(), a.begin() + s);
    while (!st.empty())
    {
        Poly neg = move(st.back().first);
        int t = st.back().second;
        st.pop_back();
        Poly b(2 * d - 1);
        for (int i = 0; i < d; i++)
        {
            b[2 * i] = a[i];
        }
        Poly f = conv(move(neg), move(b));
        int l = d - t;
        a = Poly(f.begin() + l, f.begin() + l + d);
    }
    return a;
}

Poly ratBlock(Poly p, Poly q, unsigned long long k, int m)
{
    // p/q 是有理 FPS，q[0] 非零；返回从第 k 项起的 m 项。
    // 设 L=log(1+k/d)，时间 O(M(|p|)+M(d)L+M(d+m))，空间 O(|p|+dL+d+m)。
    // 所有实际卷积长度须不超过公共 NTT 的 2^23 上限。
    assert(m >= 0); // 调试检查，可删。
    q.norm();
    assert(!q.empty() && q[0] != Z(0)); // 调试检查，可删。
    if (!m)
    {
        return {};
    }
    assert(k <= numeric_limits<unsigned long long>::max() -
                    (unsigned long long)(m - 1)); // 调试检查，可删。
    p.norm();
    Z iv = q[0].inv();
    p *= iv;
    q *= iv;

    if (q.size() == 1)
    {
        Poly ans(m);
        if (k < (unsigned long long)p.size())
        {
            int n = min<unsigned long long>(m, (unsigned long long)p.size() - k);
            copy_n(p.begin() + (size_t)k, n, ans.begin());
        }
        return ans;
    }

    auto [a, r] = polyDivmod(move(p), q);
    Poly ans(m);
    if (k < (unsigned long long)a.size())
    {
        int n = min<unsigned long long>(m, (unsigned long long)a.size() - k);
        for (int i = 0; i < n; i++)
        {
            ans[i] = a[(size_t)k + i];
        }
    }

    int d = q.size() - 1;
    if (r.empty())
    {
        return ans;
    }
    assert(m <= NTT_LIM && convFit(d + 1, d)); // 调试检查，可删。
    Poly t = invTail(q, k);
    Poly u = conv(q, move(t));
    u.resize(d);
    Poly v = conv(move(u), move(r));
    Poly s = polyDivmod(move(v), q).second;
    assert(convFit(s.size(), m)); // 调试检查，可删。
    Poly iq = fpsInvFast(q, m);
    Poly b = conv(move(s), move(iq));
    b.resize(m);
    ans += b;
    return ans;
}

void printPoly(const vector<Z> &a)
{
    // a 是低次在前的模系数数组；按 Library Checker 格式输出一行，无返回值。
    for (int i = 0; i < (int)a.size(); i++)
    {
        if (i)
        {
            cout << ' ';
        }
        cout << a[i].val();
    }
    cout << '\n';
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int d, m;
    unsigned long long k;
    cin >> d >> k >> m;
    vector<Z> a(d), c(d);
    for (int i = 0, x; i < d; i++)
    {
        cin >> x;
        a[i] = x;
    }
    for (int i = 0, x; i < d; i++)
    {
        cin >> x;
        c[i] = x;
    }
    auto [p, q] = recurrenceFraction(a, c);
    printPoly(ratBlock(move(p), move(q), k, m));
    return 0;
}
