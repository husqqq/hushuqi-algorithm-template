





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

struct PolyTree
{
    static constexpr int S = 128; // 子树点数不超过 S 时直接逐点 Horner 求值。
    // n 保存求值点个数，z 保存补齐到的 2 的幂叶子数。
    int n;
    int z;
    // x 保存各求值点，t 保存乘积树多项式。
    vector<Z> x;
    vector<Poly> t;

    PolyTree(const vector<Z> &points)
        : n(points.size()), z(bit_ceil((unsigned long long)max<int>(1, n))), x(points)
    {
        // points 是全部求值点；建立乘积树。
        assert(z <= NTT_LIM); // 调试检查，可删。
        t.assign(2 * z, Poly{1});
        for (int i = 0; i < n; i++)
        {
            t[z + i] = {-x[i], 1};
        }
        for (int i = z - 1; i; i--)
        {
            t[i] = t[i << 1] * t[i << 1 | 1];
        }
    }

    vector<Z> eval(const Poly &a) const
    {
        // a 是待求值多项式；返回 a(x[i])，顺序与构造参数 x 相同。
        if (n == 0)
        {
            return {};
        }
        int m = a.size();
        while (m && a[m - 1] == Z(0))
        {
            m--;
        }
        int b = t[1].size();
        if (m >= b && m > 256 && b > 256)
        {
            int q = m - b + 1;
            assert(convFit(q, q) && convFit(b, q)); // 调试检查，可删。
        }
        vector<Z> ans(n);
        auto dfs = [&](auto &&self, int p, int l, int w, Poly r) -> void
        {
            int cnt = max<int>(0, min(n, l + w) - l);
            if (cnt <= S)
            {
                for (int i = l; i < l + cnt; i++)
                {
                    Z v = 0;
                    for (auto it = r.rbegin(); it != r.rend(); ++it)
                    {
                        v = v * x[i] + *it;
                    }
                    ans[i] = v;
                }
                return;
            }
            if (l + w / 2 >= n)
            {
                self(self, p << 1, l, w / 2,
                     polyDivmod(move(r), t[p << 1]).second);
                return;
            }
            Poly left = polyDivmod(r, t[p << 1]).second;
            Poly right = polyDivmod(move(r), t[p << 1 | 1]).second;
            self(self, p << 1, l, w / 2, move(left));
            self(self, p << 1 | 1, l + w / 2, w / 2, move(right));
        };
        dfs(dfs, 1, 0, z, polyDivmod(a, t[1]).second);
        return ans;
    }
};

Z factorialFast(int n)
{
    // n 满足 0<=n<Z::mod()；返回 n!，使用分块多项式乘积与多点求值降低单次复杂度。
    assert(0 <= n && n < Z::mod()); // 调试检查，可删。
    if (n <= 1)
    {
        return 1;
    }
    if (n > (Z::mod() - 1) / 2)
    {
        int m = Z::mod() - 1 - n;
        Z s = m & 1 ? Z(1) : Z(-1);
        return s / factorialFast(m);
    }
    int b = max<int>(1, sqrtl(n));
    while ((b + 1) * (b + 1) <= n)
    {
        b++;
    }
    while (b * b > n)
    {
        b--;
    }
    int q = n / b;
    vector<Z> roots(b);
    vector<Z> xs(q);
    for (int i = 0; i < b; i++)
    {
        roots[i] = -Z(i + 1);
    }
    for (int i = 0; i < q; i++)
    {
        xs[i] = Z(i * b);
    }
    PolyTree prod(roots);
    PolyTree eval(xs);
    auto val = eval.eval(prod.t[1]);
    Z ans = 1;
    for (auto x : val)
    {
        ans *= x;
    }
    for (int x = q * b + 1; x <= n; x++)
    {
        ans *= x;
    }
    return ans;
}

vector<Z> factorialBatch(const vector<int> &q)
{
    // q 是所有待查询的非负阶乘下标；返回与 q 同序的 n!。
    // 先用多点求值预处理整块阶乘，再用一棵乘积树批量求全部块内前缀积。
    if (q.empty())
    {
        return {};
    }
    constexpr int LOG_BLOCK = 15;
    constexpr int BLOCK = 1 << LOG_BLOCK;
    static const vector<Z> blockFac = []
    {
        int cnt = (mod - 1) / BLOCK;
        vector<Z> roots(BLOCK), xs(cnt);
        for (int i = 0; i < BLOCK; i++) roots[i] = -Z(i + 1);
        for (int i = 0; i < cnt; i++) xs[i] = Z(i * BLOCK);
        Poly f;
        {
            PolyTree prod(roots);
            f = prod.t[1];
        }
        vector<Z> val;
        {
            PolyTree eval(xs);
            val = eval.eval(f);
        }
        vector<Z> fac(cnt + 1, 1);
        for (int i = 0; i < cnt; i++) fac[i + 1] = fac[i] * val[i];
        return fac;
    }();

    vector<Z> ans(q.size());
    vector<vector<pair<int, int>>> queries(BLOCK);
    int leaves = BLOCK - 1;
    for (int i = 0; i < (int)q.size(); i++)
    {
        int n = q[i];
        assert(0 <= n && n < mod);
        int base = n / BLOCK * BLOCK;
        int r = n % BLOCK;
        ans[i] = blockFac[n / BLOCK];
        if (r)
        {
            queries[r].push_back({base, i});
            leaves++;
        }
    }

    int z = bit_ceil((unsigned long long)max<int>(1, leaves));
    vector<int> queryLeaf(q.size(), -1);
    vector<Poly> factors(2 * z, Poly{1}), points(2 * z, Poly{1});
    int at = 0;
    for (int r = 1; r < BLOCK; r++)
    {
        factors[z + at++] = {Z(r), Z(1)};
        for (auto [x, id] : queries[r])
        {
            queryLeaf[id] = z + at;
            points[z + at++] = {-Z(x), Z(1)};
        }
    }
    for (int p = z - 1; p > 1; p--)
    {
        factors[p] = factors[p << 1] * factors[p << 1 | 1];
        points[p] = points[p << 1] * points[p << 1 | 1];
    }
    points[1] = {Z(1)};
    for (int p = 1; p < z; p++)
    {
        Poly prefix = points[p];
        points[p << 1] = polyDivmod(prefix, points[p << 1]).second;
        points[p << 1 | 1] = polyDivmod(prefix * factors[p << 1],
                                         points[p << 1 | 1]).second;
    }
    for (int i = 0; i < (int)q.size(); i++)
    {
        if (queryLeaf[i] != -1)
        {
            const Poly &r = points[queryLeaf[i]];
            ans[i] *= r.empty() ? Z(0) : r[0];
        }
    }
    return ans;
}

signed main()
{
    int t; cin >> t;
    vector<int> q(t);
    for (int &n : q) cin >> n;
    for (Z x : factorialBatch(q)) cout << x.val() << '\n';
}
