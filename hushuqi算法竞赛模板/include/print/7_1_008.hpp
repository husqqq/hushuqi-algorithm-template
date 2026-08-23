#pragma once
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
