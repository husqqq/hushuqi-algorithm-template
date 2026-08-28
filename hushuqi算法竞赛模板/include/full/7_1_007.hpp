#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <uint32_t P> struct MInt
{
    static_assert(1 < P && P <= 1000000007);
    uint32_t x = 0; // 固定 32 位以保证对象为 4 字节，提高大数组的缓存密度。

    MInt(long long v = 0)
    {
        // v 是要转入模 P 剩余类的整数；构造其最小非负代表元。
        v %= (long long)P;
        if (v < 0)
        {
            v += P;
        }
        x = (uint32_t)v;
    }

    static constexpr uint32_t mod()
    {
        // 无参数；返回编译期模数 P。
        return P;
    }

    uint32_t val() const
    {
        // 无参数；返回当前剩余类的最小非负代表元。
        return x;
    }

    MInt operator-() const
    {
        // 无参数；返回当前剩余类的加法逆元。
        MInt ans;
        ans.x = x ? P - x : 0;
        return ans;
    }

    MInt &operator+=(const MInt &o)
    {
        // o 是要加到当前值上的同模剩余类；原地完成模加并返回当前对象引用。
        x += o.x;
        if (x >= P)
        {
            x -= P;
        }
        return *this;
    }

    MInt &operator-=(const MInt &o)
    {
        // o 是要从当前值减去的同模剩余类；原地完成模减并返回当前对象引用。
        x += P - o.x;
        if (x >= P)
        {
            x -= P;
        }
        return *this;
    }

    MInt &operator*=(const MInt &o)
    {
        // o 是要乘到当前值上的同模剩余类；原地完成模乘并返回当前对象引用。
        // 乘积可能超过 32 位，显式提升到无符号 64 位。
        x = (uint64_t)x * o.x % P;
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

    MInt pow(uint64_t b) const
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

template <uint32_t P> const vector<MInt<P>> &invTable(int n)
{
    // n 是需要的最大下标且 0<=n<P；返回至少覆盖 0..n 的共享模逆元表，其中位置 0 为 0。
    assert(0 <= n && (uint64_t)n < P); // 调试检查，可删
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

constexpr uint32_t mod = 998244353;
using Z = MInt<mod>;
