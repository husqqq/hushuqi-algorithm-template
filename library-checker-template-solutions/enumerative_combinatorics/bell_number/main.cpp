

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

unsigned long long kthRoot(unsigned long long n, int k)
{
    // n 是被开方的非负整数，k 是正整数次数；返回 floor(n^(1/k))。
    assert(k > 0); // 调试检查，可删
    if (k == 1 || n <= 1)
    {
        return n;
    }
    if (k >= 64)
    {
        return 1;
    }
    auto ok = [&](unsigned long long x)
    {
        unsigned __int128 y = 1;
        for (int i = 0; i < k; i++)
        {
            y *= x;
            if (y > n)
            {
                return false;
            }
        }
        return true;
    };
    unsigned long long l = 1;
    unsigned long long r = min(n, 1ULL << (64 / k + 1));
    while (l < r)
    {
        unsigned long long m = l + (r - l + 1) / 2;
        if (ok(m))
        {
            l = m;
        }
        else
        {
            r = m - 1;
        }
    }
    return l;
}

optional<long long> bsgs(long long a, long long b, long long m,
                         long long ord = -1)
{
    // 在 [0,ord) 内求最小解；ord=-1 时使用通用上界 m，无解返回空。
    assert(m > 0); // 调试检查，可删
    if (m == 1)
    {
        return 0;
    }
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
    assert(binaryGcd(a, m) == 1); // 调试检查，可删
    if (ord == -1)
    {
        ord = m;
    }
    assert(ord > 0); // 调试检查，可删
    long long n = kthRoot((unsigned long long)ord, 2);
    if (n <= (ord - 1) / n && n * n < ord)
    {
        n++;
    }
    unordered_map<long long, long long> baby;
    long long x = 1;
    for (long long j = 0; j < n; j++)
    {
        if (!baby.contains(x))
        {
            baby[x] = j;
        }
        x = mulMod(x, a, m);
    }
    long long step = powerMod(*invMod(a, m), n, m);
    x = b;
    long long rounds = ord / n + (ord % n != 0);
    for (long long i = 0; i < rounds; i++)
    {
        if (auto it = baby.find(x); it != baby.end())
        {
            long long j = it->second;
            if (j < ord && i <= (ord - 1 - j) / n)
            {
                return i * n + j;
            }
        }
        x = mulMod(x, step, m);
    }
    return nullopt;
}

optional<long long> exbsgs(long long a, long long b, long long m)
{
    // 求 a^x=b (mod m) 的最小非负解，不要求 a 与 m 互素。
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
    if (m == 1 || b == 1 % m)
    {
        return 0;
    }
    long long k = 1;
    long long z = 0;
    while (true)
    {
        long long g = (long long)binaryGcd(a, m);
        if (g == 1)
        {
            break;
        }
        if (b == k)
        {
            return z;
        }
        if (b % g)
        {
            return nullopt;
        }
        b /= g;
        m /= g;
        z++;
        k = mulMod(k, a / g, m);
    }
    if (m == 1)
    {
        return z;
    }
    auto x = bsgs(a, mulMod(b, *invMod(k, m), m), m);
    if (!x)
    {
        return nullopt;
    }
    return *x + z;
}

optional<long long> sqrtMod(long long n, long long p)
{
    // n 是被开方数，p 是素数模；返回较小平方根，非二次剩余返回空。
    assert(p >= 2); // 调试检查，可删
    n %= p;
    if (n < 0)
    {
        n += p;
    }
    if (n == 0)
    {
        return 0;
    }
    if (p == 2)
    {
        return n;
    }
    if (powerMod(n, (p - 1) / 2, p) != 1)
    {
        return nullopt;
    }
    if (p % 4 == 3)
    {
        long long x = powerMod(n, (p + 1) / 4, p);
        return min(x, p - x);
    }
    long long q = p - 1;
    long long s = 0;
    while (!(q & 1))
    {
        q >>= 1;
        s++;
    }
    long long z = 2;
    while (powerMod(z, (p - 1) / 2, p) != p - 1)
    {
        z++;
    }
    long long c = powerMod(z, q, p);
    long long x = powerMod(n, (q + 1) / 2, p);
    long long t = powerMod(n, q, p);
    long long m = s;
    while (t != 1)
    {
        long long i = 1;
        long long y = mulMod(t, t, p);
        while (y != 1)
        {
            y = mulMod(y, y, p);
            i++;
        }
        long long b = powerMod(c, 1ULL << (m - i - 1), p);
        x = mulMod(x, b, p);
        c = mulMod(b, b, p);
        t = mulMod(t, c, p);
        m = i;
    }
    return min(x, p - x);
}



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


Poly deriv(const Poly &a)
{
    // a 是输入多项式；返回形式导数。
    if (a.empty())
    {
        return {};
    }
    Poly b(a.size() - 1);
    for (int i = 1; i < (int)a.size(); i++)
    {
        b[i - 1] = a[i] * i;
    }
    return b;
}

Poly integr(const Poly &a)
{
    // a 是输入多项式；返回常数项为 0 的形式积分，次数必须小于 mod-1。
    assert(a.size() < mod); // 调试检查，可删。
    Poly b(a.size() + 1);
    const auto &iv = invTable<mod>(a.size());
    for (int i = 0; i < (int)a.size(); i++)
    {
        b[i + 1] = a[i] * iv[i + 1];
    }
    return b;
}

vector<Z> batchInv(const vector<Z> &a)
{
    // a 是允许含零的数组；返回逐项逆元，零位置仍为零。
    vector<Z> pre(a.size() + 1, 1);
    vector<Z> ans(a.size());
    for (int i = 0; i < (int)a.size(); i++)
    {
        pre[i + 1] = pre[i] * (a[i] == Z(0) ? Z(1) : a[i]);
    }
    Z suf = pre.back().inv();
    for (int i = (int)a.size() - 1; i >= 0; i--)
    {
        if (a[i] != Z(0))
        {
            ans[i] = suf * pre[i];
            suf *= a[i];
        }
    }
    return ans;
}

Poly fpsLog(const Poly &a, int n)
{
    // a 的常数项必须为 1，n 是不超过 2^22 的截断长度；返回 ln(a) mod x^n。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    assert(!a.empty() && a[0] == Z(1)); // 调试检查，可删。
    Poly f = a.cut(n);
    return integr((deriv(f) * fpsInvFast(f, n)).cut(n - 1)).cut(n);
}

Poly fpsExpNewton(const Poly &a, int n)
{
    // a 的常数项必须为 0，n 是 [0,2^22] 内的截断长度；用 Newton 迭代返回 exp(a) mod x^n。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    assert(a.empty() || a[0] == Z(0)); // 调试检查，可删。
    Poly g{1};
    for (int k = 2; k < 2 * n; k <<= 1)
    {
        Poly q = a.cut(k) - fpsLog(g, k);
        q[0] += 1;
        g = (g * q).cut(k);
    }
    return g.cut(n);
}

namespace rexp
{
constexpr int B = 16;
constexpr int S = 64;
constexpr int LIM = 1 << 20;

struct RExp
{
    int n; // 实际截断长度。
    int sz; // 补齐后的二次幂长度。
    NTT32 nt; // 预分配切片使用的公共二进制 NTT。
    vector<uint32_t> dg; // dg[i]=i*g[i]，是输入指数多项式的导数系数。
    vector<uint32_t> f; // 已经求出的 exp(g) 系数。
    vector<uint32_t> iv; // iv[i] 是 i 的模逆元，iv[0]=iv[1]=1。
    vector<uint32_t> t; // 当前层逆变换的临时数组。
    vector<uint32_t> wf; // 各层答案块的预分配频域工作区。
    vector<uint32_t> wg; // 各层导数块的预分配频域工作区。
    vector<uint64_t> sum; // 当前频点的延迟取模乘积和。

    static size_t wsize(int n)
    {
        // n 是当前 16 叉分治区间长度；返回各递归层频域块所需的 uint32_t 元素总数。
        size_t ans = 0;
        while (n > S)
        {
            ans += 2 * n;
            n /= B;
        }
        return ans;
    }

    RExp(const Poly &g, int m)
        : n(m), sz(bit_ceil((unsigned long long)max<int>(1, m))), nt(sz > S ? sz / 8 : 0),
          dg(sz), f(sz), iv(m, 1),
          t(sz > S ? sz / 8 : 0), wf(wsize(sz)), wg(wsize(sz)), sum(t.size())
    {
        // g 是常数项为 0 的输入 FPS，m 是实际截断长度；初始化在线卷积所需全部状态。
        // dg[i]=i*g[i]，使 exp(g)'=g'*exp(g) 成为在线卷积递推。
        for (int i = 1; i < min(m, (int)g.size()); i++)
        {
            dg[i] = (uint64_t)g[i].x * i % mod;
        }
        f[0] = 1;
        const auto &ziv = invTable<mod>(m - 1);
        for (int i = 1; i < m; i++)
        {
            iv[i] = ziv[i].val();
        }
    }

    void dfs(int l, int w, uint32_t *pf, uint32_t *pg)
    {
        // l、w 确定当前区间，pf、pg 是本层答案块与导数块的频域工作区；补全该区间答案，无返回值。
        if (w <= S)
        {
            int r = min(n, l + w);
            for (int i = l; i < r; i++)
            {
                uint64_t v = 0;
                for (int j = l; j < i; j++)
                {
                    v += (uint64_t)f[j] * dg[i - j];
                    if (((j - l) & 15) == 15)
                    {
                        v %= mod;
                    }
                }
                uint32_t x = f[i] + v % mod;
                if (x >= mod)
                {
                    x -= mod;
                }
                f[i] = (uint64_t)x * iv[i] % mod;
            }
            return;
        }

        int b = w / B;
        int len = 2 * b;
        array<uint32_t *, B - 1> tf, tg;
        for (int i = 0; i < B - 1; i++)
        {
            tf[i] = pf + i * len;
            tg[i] = pg + i * len;
        }

        // 卷积核只与下标差有关，同一层的所有结点复用这些频域块。
        if (l == 0)
        {
            for (int i = 0; i < B - 1 && (i + 1) * b < this->n; i++)
            {
                copy_n(dg.data() + i * b, len, tg[i]);
                nt.dft(tg[i], len);
            }
        }

        for (int i = 0; i < B && l + i * b < this->n; i++)
        {
            if (i)
            {
                fill(sum.begin(), sum.begin() + len, uint64_t(0));
                for (int j = 0; j < i; j++)
                {
                    for (int k = 0; k < len; k++)
                    {
                        sum[k] += (uint64_t)tf[j][k] * tg[i - j - 1][k];
                    }
                }
                for (int k = 0; k < len; k++)
                {
                    t[k] = sum[k] % mod;
                }
                nt.idft(t.data(), len);
                for (int k = 0; k < b && l + i * b + k < this->n; k++)
                {
                    uint32_t &x = f[l + i * b + k];
                    x += t[k + b];
                    if (x >= mod)
                    {
                        x -= mod;
                    }
                }
            }

            dfs(l + i * b, b, pf + 2 * w, pg + 2 * w);
            if (i + 1 < B && l + (i + 1) * b < this->n)
            {
                fill(tf[i], tf[i] + len, uint32_t(0));
                copy_n(f.data() + l + i * b, b, tf[i]);
                nt.dft(tf[i], len);
            }
        }
        fill(pf, pf + (B - 1) * len, uint32_t(0));
    }

    Poly run()
    {
        // 无参数；执行在线分治并返回输入 FPS 指数的前 n 项。
        dfs(0, sz, wf.data(), wg.data());
        Poly ans(n);
        for (int i = 0; i < n; i++)
        {
            ans[i].x = f[i];
        }
        return ans;
    }
};
}

Poly fpsExpRelaxed(const Poly &a, int n)
{
    // a 的常数项必须为 0，n 不超过 2^20；用 16 叉在线卷积返回 exp(a) mod x^n。
    assert(n >= 0 && n <= rexp::LIM); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    assert(a.empty() || a[0] == Z(0)); // 调试检查，可删。
    return rexp::RExp(a, n).run();
}

Poly fpsExp(const Poly &a, int n)
{
    // a 的常数项必须为 0，n 是截断长度；默认使用在线分块算法，超长时回退 Newton 迭代。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n <= rexp::LIM)
    {
        return fpsExpRelaxed(a, n);
    }
    return fpsExpNewton(a, n);
}

optional<Poly> fpsSqrtAt(const Poly &a, int n, int p, Z s)
{
    // a[p] 是前 n 项中的首个非零系数，s 是它的一组平方根；返回对应 FPS 平方根，无解时返回空。
    assert(0 <= p && p < min(n, (int)a.size()) && a[p] != Z(0)); // 调试检查，可删。
    if ((p & 1) || s * s != a[p])
    {
        return nullopt;
    }
    int m = n - p;
    Poly b(a.begin() + p, a.begin() + min((int)a.size(), p + m));
    b.resize(m);
    Poly g{s};
    Z iv2 = Z(2).inv();
    for (int k = 2; k < 2 * m; k <<= 1)
    {
        g = ((g + (b.cut(k) * fpsInvFast(g, k)).cut(k)) * iv2).cut(k);
    }
    Poly ans(p / 2);
    g = g.cut(n - p / 2);
    ans.insert(ans.end(), g.begin(), g.end());
    return ans.cut(n);
}

optional<Poly> fpsSqrt(const Poly &a, int n, Z s)
{
    // a 是输入 FPS，n 是不超过 2^22 的截断长度，s 是首个非零系数的一组平方根；无平方根时返回空。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n == 0)
    {
        return Poly{};
    }
    int p = 0;
    while (p < min(n, (int)a.size()) && a[p] == Z(0))
    {
        p++;
    }
    if (p == min(n, (int)a.size()))
    {
        return Poly(n);
    }
    return fpsSqrtAt(a, n, p, s);
}

optional<Poly> fpsSqrt(const Poly &a, int n)
{
    // a 是输入 FPS，n 是不超过 2^22 的截断长度；自动求首个非零系数平方根，返回一组平方根或空。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n == 0)
    {
        return Poly{};
    }
    int p = 0;
    while (p < min(n, (int)a.size()) && a[p] == Z(0))
    {
        p++;
    }
    if (p == min(n, (int)a.size()))
    {
        return Poly(n);
    }
    if (p & 1)
    {
        return nullopt;
    }
    auto s = sqrtMod(a[p].val(), mod);
    if (!s)
    {
        return nullopt;
    }
    return fpsSqrtAt(a, n, p, Z(*s));
}

Poly fpsPow(const Poly &a, unsigned long long k, int n)
{
    // a 是输入 FPS，k 是非负指数，n 是不超过 2^22 的截断长度；返回 a^k mod x^n。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    if (k == 0)
    {
        Poly ans(n);
        ans[0] = 1;
        return ans;
    }
    int lim = min(n, (int)a.size());
    int p = 0;
    while (p < lim && a[p] == Z(0))
    {
        p++;
    }
    if (p == lim || (p && k > (unsigned long long)(n - 1) / p))
    {
        return Poly(n);
    }
    Z c = a[p];
    int sh = p * k;
    int m = n - sh;
    Poly b(a.begin() + p, a.begin() + min((int)a.size(), p + m));
    b.resize(m);
    b *= c.inv();
    b = fpsExp(fpsLog(b, m) * Z(k % mod), m) * c.pow(k);
    Poly ans(sh);
    ans.insert(ans.end(), b.begin(), b.end());
    return ans.cut(n);
}
using namespace std;
#define int long long

template <class T> vector<T> bell(int n)
{
    // n 是要计算的最大下标；返回 B_0 到 B_n 的 Bell 数。
    assert(n >= 0); // 调试检查，可删。
    vector<vector<T>> a(n + 1, vector<T>(n + 1));
    vector<T> b(n + 1);
    a[0][0] = 1;
    b[0] = 1;
    for (int i = 1; i <= n; i++)
    {
        a[i][0] = a[i - 1][i - 1];
        b[i] = a[i][0];
        for (int j = 1; j <= i; j++)
        {
            a[i][j] = a[i][j - 1] + a[i - 1][j - 1];
        }
    }
    return b;
}

vector<Z> bellFast(int n)
{
    // n 是最大下标且 n<998244353；返回 B_0 到 B_n，复杂度 O(n log n)。
    assert(0 <= n && n < mod);
    vector<Z> fac(n + 1, 1), ifac(n + 1, 1);
    for (int i = 1; i <= n; i++) fac[i] = fac[i - 1] * i;
    ifac[n] = fac[n].inv();
    for (int i = n; i; i--) ifac[i - 1] = ifac[i] * i;
    Poly f(n + 1);
    for (int i = 1; i <= n; i++) f[i] = ifac[i];
    f = fpsExp(f, n + 1);
    for (int i = 0; i <= n; i++) f[i] *= fac[i];
    return f;
}

signed main()
{
    int n; cin >> n;
    for (Z x : bellFast(n)) cout << x.val() << ' ';
    cout << '\n';
}
