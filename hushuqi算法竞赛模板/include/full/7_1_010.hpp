#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <unsigned P> struct Mont
{
    unsigned x = 0; // 真值乘 2^32 后模 P 的 Montgomery 域表示，范围 [0,2P)

    static_assert(P > 0 && (P & 1));
    static_assert(P < (1U << 30));

    static constexpr unsigned inv()
    {
        // 返回 P 关于 2^32 的负逆。
        unsigned x = P;
        for (int i = 0; i < 5; i++)
        {
            x *= 2 - P * x;
        }
        return -x;
    }

    static constexpr unsigned r = inv();
    static constexpr unsigned r2 = -(unsigned long long)P % P;

    static constexpr unsigned reduce(unsigned long long x)
    {
        // x 小于 2^32*P；返回 x*2^-32 mod P，结果在 [0,2P) 内。
        return (x + (unsigned long long)((unsigned)x * r) * P) >> 32;
    }

    Mont(long long v = 0)
    {
        // 把普通整数 v 编码进 Montgomery 域。
        v %= P;
        if (v < 0)
        {
            v += P;
        }
        x = reduce((unsigned long long)v * r2);
    }

    unsigned val() const
    {
        // 无参数；返回当前剩余类的最小非负代表元。
        unsigned v = reduce(x);
        return v >= P ? v - P : v;
    }

    Mont &operator+=(const Mont &o)
    {
        // o 是同模 Montgomery 数；原地完成域内加法并返回当前对象引用。
        x += o.x;
        if (x >= 2 * P)
        {
            x -= 2 * P;
        }
        return *this;
    }

    Mont &operator-=(const Mont &o)
    {
        // o 是同模 Montgomery 数；原地完成域内减法并返回当前对象引用。
        x -= o.x;
        if (x >= 2 * P)
        {
            x += 2 * P;
        }
        return *this;
    }

    Mont &operator*=(const Mont &o)
    {
        // o 是同模 Montgomery 数；原地完成域内乘法并返回当前对象引用。
        x = reduce((unsigned long long)x * o.x);
        return *this;
    }

    Mont pow(long long n) const
    {
        // n 是非负指数；返回当前元素的 n 次幂。
        assert(n >= 0); // 调试检查，可删
        Mont a = *this;
        Mont r = 1;
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

    Mont invMod() const
    {
        // 当前元素必须非零且 P 必须为素数；返回乘法逆元。
        assert(val() != 0); // 调试检查，可删
        return pow(P - 2);
    }

    Mont &operator/=(const Mont &o)
    {
        // o 是非零同模 Montgomery 数；原地除以 o 并返回当前对象引用。
        return *this *= o.invMod();
    }

    friend Mont operator+(Mont a, const Mont &b)
    {
        // a、b 是同模 Montgomery 数；返回 a+b。
        return a += b;
    }

    friend Mont operator-(Mont a, const Mont &b)
    {
        // a、b 是同模 Montgomery 数；返回 a-b。
        return a -= b;
    }

    friend Mont operator*(Mont a, const Mont &b)
    {
        // a、b 是同模 Montgomery 数；返回 a*b。
        return a *= b;
    }

    friend Mont operator/(Mont a, const Mont &b)
    {
        // a、b 是同模 Montgomery 数且 b 非零；返回 a/b。
        return a /= b;
    }

    friend bool operator==(const Mont &a, const Mont &b)
    {
        // a、b 是同模 Montgomery 数；返回二者是否代表同一剩余类。
        return a.val() == b.val();
    }
};
