// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 49 Min_25 筛

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


template <class Mint> struct Min25T
{
    long long n; // 前缀和上界
    long long sq; // floor(sqrt(n))
    long long hi; // 大商与小商的分界
    long long s; // 不同整除商数组长度
    vector<int> p; // 不超过 sq 的素数
    vector<Mint> buf; // 各离散商处的素数 f 值前缀和
    Mint A; // f(p^e)=A*e+B*p 的指数系数
    Mint B; // f(p^e)=A*e+B*p 的质数系数
    Mint ans; // 当前查询答案

    Min25T(long long n) : n(n), sq(0)
    {
        // n 是正前缀上界；建立整除商表并筛出不超过 sqrt(n) 的素数。
        assert(n > 0); // 调试检查，可删
        sq = sqrtl(n);
        while (sq + 1 <= n / (sq + 1))
        {
            sq++;
        }
        while (sq > n / sq)
        {
            sq--;
        }
        hi = n / sq;
        if (hi != 1 && n / (hi - 1) == sq)
        {
            hi--;
        }
        s = hi + sq;
        vector<bool> isp(sq + 1, true);
        for (int i = 2; i <= sq; i++)
        {
            if (!isp[i])
            {
                continue;
            }
            p.push_back(i);
            if (i <= sq / i)
            {
                for (int j = i * i; j <= sq; j += i)
                {
                    isp[j] = false;
                }
            }
        }
    }

    int id(long long x) const
    {
        // x 是整除商 n/l；返回其离散数组下标。
        return x <= sq ? s - x : n / x;
    }

    vector<Mint> primeCount() const
    {
        // 返回各离散整除商对应的不超过该值的素数个数。
        vector<Mint> h(s);
        for (long long x = 1; x < s; x++)
        {
            long long v = x < hi ? n / x : s - x;
            h[x] = v - 1;
        }
        int cnt = 0;
        for (long long q : p)
        {
            for (long long x = 1; x < s; x++)
            {
                long long v = x < hi ? n / x : s - x;
                if (v < q * q)
                {
                    break;
                }
                h[x] -= h[id(v / q)] - cnt;
            }
            cnt++;
        }
        return h;
    }

    vector<Mint> primeSum() const
    {
        // 返回各离散整除商对应的不超过该值的素数和。
        vector<Mint> h(s);
        for (long long x = 1; x < s; x++)
        {
            long long v = x < hi ? n / x : s - x;
            if (v & 1)
            {
                h[x] = Mint(v) * Mint(v / 2 + 1) - 1;
            }
            else
            {
                h[x] = Mint(v / 2) * Mint(v + 1) - 1;
            }
        }
        for (long long q : p)
        {
            Mint before = h[id(q - 1)];
            for (long long x = 1; x < s; x++)
            {
                long long v = x < hi ? n / x : s - x;
                if (v < q * q)
                {
                    break;
                }
                h[x] -= Mint(q) * (h[id(v / q)] - before);
            }
        }
        return h;
    }

    Mint f(long long q, int e) const
    {
        // q 是质数，e 是正指数；返回 f(q^e)=A*e+B*q。
        return A * e + B * q;
    }

    void dfs(int i, int e, long long prod, Mint cur)
    {
        // i、e 指当前质数及指数，prod 是已选乘积，cur 是此前质因子贡献；递归累加后续贡献。
        ans += cur * f(p[i], e + 1);
        long long lim = n / prod;
        if (lim >= 1LL * p[i] * p[i])
        {
            dfs(i, e + 1, prod * p[i], cur);
        }
        cur *= f(p[i], e);
        ans += cur * (buf[id(lim)] - buf[id(p[i])]);
        int j = i + 1;
        for (; j < (int)p.size() && p[j] < (1LL << 21) &&
               p[j] <= lim / p[j] / p[j]; j++)
        {
            dfs(j, 1, prod * p[j], cur);
        }
        for (; j < (int)p.size() && p[j] <= lim / p[j]; j++)
        {
            Mint sum = f(p[j], 2);
            sum += f(p[j], 1) * (buf[id(lim / p[j])] - buf[id(p[j])]);
            ans += cur * sum;
        }
    }

    Mint linearPPow(Mint a, Mint b)
    {
        // 对 f(p^e)=a*e+b*p，返回 sum_{i=1}^n f(i)。
        A = a;
        B = b;
        auto c = primeCount();
        auto d = primeSum();
        buf.resize(s);
        for (int i = 0; i < s; i++)
        {
            buf[i] = c[i] * A + d[i] * B;
        }
        ans = buf[id(n)] + 1;
        for (int i = 0; i < (int)p.size(); i++)
        {
            dfs(i, 1, p[i], 1);
        }
        return ans;
    }
};

using Min25 = Min25T<Z>;
