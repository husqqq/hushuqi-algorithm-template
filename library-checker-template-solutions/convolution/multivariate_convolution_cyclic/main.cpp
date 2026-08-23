

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

vector<Z> convLarge(const vector<Z> &a, const vector<Z> &b)
{
    // a、b 是总长可超过单次 NTT 上限的序列；返回分块线性卷积。
    if (a.empty() || b.empty()) return {};
    if (convFit(a.size(), b.size())) return conv(a, b);
    constexpr int block = NTT_LIM / 2;
    vector<Z> ans(a.size() + b.size() - 1);
    vector<vector<Z>> x, y;
    for (int i = 0; i < (int)a.size(); i += block)
    {
        vector<Z> v(a.begin() + i, a.begin() + min<int>(a.size(), i + block));
        v.resize(NTT_LIM);
        dft(v);
        x.push_back(move(v));
    }
    for (int i = 0; i < (int)b.size(); i += block)
    {
        vector<Z> v(b.begin() + i, b.begin() + min<int>(b.size(), i + block));
        v.resize(NTT_LIM);
        dft(v);
        y.push_back(move(v));
    }
    for (int s = 0; s + 1 < (int)x.size() + (int)y.size(); s++)
    {
        vector<Z> z(NTT_LIM);
        for (int i = max<int>(0, s - (int)y.size() + 1);
             i <= s && i < (int)x.size(); i++)
        {
            int j = s - i;
            for (int k = 0; k < NTT_LIM; k++)
            {
                z[k] += x[i][k] * y[j][k];
            }
        }
        idft(z);
        int offset = s * block;
        int len = min<int>(NTT_LIM, ans.size() - offset);
        for (int k = 0; k < len; k++)
        {
            ans[offset + k] += z[k];
        }
    }
    return ans;
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
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Barrett
{
    unsigned m;            // 当前约减使用的正模数
    unsigned long long im; // m>1 时为 ceil(2^64/m)，作估商用的定点倒数

    Barrett(unsigned m = 1) : m(m), im(0)
    {
        // m 是正模数；预计算定点倒数。
        assert(m > 0); // 调试检查，可删
        im = ~0ULL / m + 1;
    }

    unsigned mul(unsigned a, unsigned b) const
    {
        // a、b 均在 [0,m)；返回 a*b mod m。
        if (m == 1)
        {
            return 0;
        }
        unsigned long long z = 1ULL * a * b;
        unsigned long long x = (unsigned __int128)z * im >> 64;
        unsigned v = z - x * m;
        if (m <= v)
        {
            v += m;
        }
        return v;
    }
};

struct DynMod
{
    inline static Barrett bt; // 全类共享的当前约减器
    unsigned x = 0;           // 当前模数下的最小非负代表元

    static void setMod(unsigned m)
    {
        // 把后续所有 DynMod 对象的模数设为 m。
        assert(m > 0 && m < (1U << 31)); // 调试检查，可删
        bt = Barrett(m);
    }

    static unsigned mod()
    {
        // 无参数；返回当前设定的模数。
        return bt.m;
    }

    DynMod(long long v = 0)
    {
        // 把普通整数 v 规范到当前模数下的最小非负代表元。
        v %= mod();
        if (v < 0)
        {
            v += mod();
        }
        x = v;
    }

    DynMod &operator+=(DynMod b)
    {
        // b 是同模剩余类；原地完成模加并返回当前对象引用。
        x += b.x;
        if (x >= mod())
        {
            x -= mod();
        }
        return *this;
    }

    DynMod &operator-=(DynMod b)
    {
        // b 是同模剩余类；原地完成模减并返回当前对象引用。
        x += mod() - b.x;
        if (x >= mod())
        {
            x -= mod();
        }
        return *this;
    }

    DynMod &operator*=(DynMod b)
    {
        // b 是同模剩余类；原地完成模乘并返回当前对象引用。
        x = bt.mul(x, b.x);
        return *this;
    }

    friend DynMod operator+(DynMod a, DynMod b)
    {
        // a、b 是同模剩余类；返回 a+b。
        return a += b;
    }

    friend DynMod operator-(DynMod a, DynMod b)
    {
        // a、b 是同模剩余类；返回 a-b。
        return a -= b;
    }

    friend DynMod operator*(DynMod a, DynMod b)
    {
        // a、b 是同模剩余类；返回 a*b。
        return a *= b;
    }

    bool operator==(const DynMod &b) const = default;

    DynMod pow(unsigned long long n) const
    {
        // n 是非负指数；返回当前剩余类的 n 次幂。
        DynMod a = *this;
        DynMod r = 1;
        while (n)
        {
            if (n & 1)
            {
                r *= a;
            }
            a *= a;
            n >>= 1;
        }
        return r;
    }
};
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

long long primitiveRoot(long long p)
{
    // p 是素数；返回模 p 的最小正原根，p=2 时返回 1。
    assert(p >= 2); // 调试检查，可删
    if (p == 2)
    {
        return 1;
    }
    long long n = p - 1;
    auto raw = factor64(n);
    raw.erase(unique(raw.begin(), raw.end()), raw.end());
    for (long long g = 2;; g++)
    {
        bool ok = true;
        for (auto d : raw)
        {
            if (powerMod(g, n / d, p) == 1)
            {
                ok = false;
            }
        }
        if (ok)
        {
            return g;
        }
    }
}

vector<long long> primitiveRoots(long long p)
{
    // p 是素数；返回模 p 的全部原根并按数值升序排列。
    long long g = primitiveRoot(p);
    long long n = p - 1;
    vector<long long> a;
    for (long long k = 1; k <= n; k++)
    {
        if (gcd(k, n) == 1)
        {
            a.push_back(powerMod(g, k, p));
        }
    }
    sort(a.begin(), a.end());
    return a;
}

vector<Z> multiConv(const vector<int> &base, const vector<Z> &f, const vector<Z> &g)
{
    // base 依次给出各维长度，f、g 按最低维在前展平；返回各维无进位卷积。
    int k = base.size();
    int n = 1;
    for (int x : base)
    {
        assert(x > 0 && n <= (1 << 22) / x); // 调试检查，可删；同时保证 NTT 长度可用。
        n *= x;
    }
    assert((int)f.size() == n && (int)g.size() == n); // 调试检查，可删。
    if (n == 1)
    {
        return {f[0] * g[0]};
    }
    auto cls = [&](int x)
    {
        int c = 0;
        for (int b : base)
        {
            x /= b;
            c += x;
        }
        return c % k;
    };
    int len = bit_ceil((unsigned long long)(2 * n - 1));
    vector a(k, vector<Z>(len));
    vector b(k, vector<Z>(len));
    for (int i = 0; i < n; i++)
    {
        a[cls(i)][i] = f[i];
        b[cls(i)][i] = g[i];
    }
    for (int i = 0; i < k; i++)
    {
        FastNTT::dft(a[i]);
        FastNTT::dft(b[i]);
    }
    vector c(k, vector<Z>(len));
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            for (int p = 0; p < len; p++)
            {
                c[(i + j) % k][p] += a[i][p] * b[j][p];
            }
        }
    }
    for (auto &v : c)
    {
        FastNTT::idft(v);
    }
    vector<Z> ans(n);
    for (int i = 0; i < n; i++)
    {
        ans[i] = c[cls(i)][i];
    }
    return ans;
}

vector<DynMod> geomDft(vector<DynMod> f, DynMod r)
{
    // f 是低次在前的系数，r 是非零单位根；返回 f(1),f(r),...,f(r^(n-1))。
    int n = f.size();
    if (n <= 100)
    {
        vector<DynMod> ans(n);
        DynMod x = 1;
        for (int i = 0; i < n; i++)
        {
            DynMod pw = 1;
            for (int j = 0; j < n; j++)
            {
                ans[i] += f[j] * pw;
                pw *= x;
            }
            x *= r;
        }
        return ans;
    }
    vector<DynMod> a(2 * n - 1), b(n);
    DynMod pw = 1;
    a[0] = 1;
    for (int i = 1; i < 2 * n - 1; i++)
    {
        a[i] = a[i - 1] * pw;
        pw *= r;
    }
    DynMod ir = r.pow(DynMod::mod() - 2);
    pw = 1;
    b[0] = 1;
    for (int i = 1; i < n; i++)
    {
        b[i] = b[i - 1] * pw;
        pw *= ir;
    }
    for (int i = 0; i < n; i++)
    {
        f[i] *= b[i];
    }
    reverse(f.begin(), f.end());
    vector<long long> x(n), y(2 * n - 1);
    for (int i = 0; i < n; i++)
    {
        x[i] = f[i].x;
    }
    for (int i = 0; i < 2 * n - 1; i++)
    {
        y[i] = a[i].x;
    }
    auto c = convMod(move(x), move(y), DynMod::mod());
    vector<DynMod> ans(n);
    for (int i = 0; i < n; i++)
    {
        ans[i] = DynMod(c[n - 1 + i]) * b[i];
    }
    return ans;
}

vector<long long> multiCyclicConv(long long p, const vector<int> &base,
                                  const vector<long long> &f, const vector<long long> &g)
{
    // p 是不超过 1E9 的素数，各维长度整除 p-1；f、g 最低维在前，返回逐维循环卷积。
    assert(2 <= p && p <= 1000000000); // 调试检查，可删。
    DynMod::setMod(p);
    int n = 1;
    for (int x : base)
    {
        assert(x >= 2 && (p - 1) % x == 0 && n <= 262144 / x); // 调试检查，可删。
        n *= x;
    }
    assert((int)f.size() == n && (int)g.size() == n); // 调试检查，可删。
    vector<DynMod> a(n), b(n);
    for (int i = 0; i < n; i++)
    {
        a[i] = f[i];
        b[i] = g[i];
    }
    DynMod gen = primitiveRoot(p);
    auto transform = [&](vector<DynMod> &v, bool inv)
    {
        int step = 1;
        for (int len : base)
        {
            DynMod root = gen.pow((p - 1) / len);
            if (inv)
            {
                root = root.pow(p - 2);
            }
            for (int block = 0; block < n; block += step * len)
            {
                for (int off = 0; off < step; off++)
                {
                    vector<DynMod> row(len);
                    for (int j = 0; j < len; j++)
                    {
                        row[j] = v[block + off + step * j];
                    }
                    row = geomDft(move(row), root);
                    for (int j = 0; j < len; j++)
                    {
                        v[block + off + step * j] = row[j];
                    }
                }
            }
            step *= len;
        }
    };
    transform(a, false);
    transform(b, false);
    for (int i = 0; i < n; i++)
    {
        a[i] *= b[i];
    }
    transform(a, true);
    DynMod scale = DynMod(n).pow(p - 2);
    vector<long long> ans(n);
    for (int i = 0; i < n; i++)
    {
        ans[i] = (a[i] * scale).x;
    }
    return ans;
}

signed main(){ ios::sync_with_stdio(false);cin.tie(nullptr);long long p;int k;cin>>p>>k;vector<int>b(k);int n=1;for(int&x:b){cin>>x;n*=x;}vector<long long>f(n),g(n);for(auto&x:f)cin>>x;for(auto&x:g)cin>>x;auto c=multiCyclicConv(p,b,f,g);for(int i=0;i<n;i++)cout<<c[i]<<" \n"[i+1==n]; }
