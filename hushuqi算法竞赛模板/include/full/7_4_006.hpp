#pragma once
#include "7_1_001.hpp"

class PowerTower
{
    struct Enc
    {
        long long rem = 0; // 对当前模数的余数
        bool large = false; // 真实值是否不小于当前模数
    };

    static long long phi(long long n)
    {
        // 返回正整数 n 的 Euler phi 值。
        long long ans = n;
        for (long long p = 2; p <= n / p; p++)
        {
            if (n % p)
            {
                continue;
            }
            while (n % p == 0)
            {
                n /= p;
            }
            ans = ans / p * (p - 1);
        }
        if (n > 1)
        {
            ans = ans / n * (n - 1);
        }
        return ans;
    }

    static long long mulCap(long long a, long long b, long long limit)
    {
        // a、b 非负且 limit 为正；返回 min(a*b,limit)。
        if (!a || !b)
        {
            return 0;
        }
        if (a > (limit - 1) / b)
        {
            return limit;
        }
        return min(limit, a * b);
    }

    static long long powerCapped(long long base, long long exponent,
                                 long long limit)
    {
        // 返回 min(base^exponent,limit)。
        long long result = 1;
        while (exponent && result < limit)
        {
            if (exponent & 1)
            {
                result = mulCap(result, base, limit);
            }
            exponent >>= 1;
            if (exponent)
            {
                base = mulCap(base, base, limit);
            }
        }
        return result;
    }

    static long long towerCapped(long long base, long long height,
                                 long long limit)
    {
        // 返回 height 层幂塔与 limit 的较小值，约定 0^0=1。
        if (height == 0)
        {
            return min(1LL, limit);
        }
        if (base == 0)
        {
            long long v = height & 1 ? 0 : 1;
            return min(v, limit);
        }
        if (base == 1)
        {
            return min(1LL, limit);
        }
        if (limit <= 1)
        {
            return limit;
        }
        long long need = 0;
        long long v = 1;
        while (v < limit)
        {
            v = mulCap(v, base, limit);
            need++;
        }
        long long e = towerCapped(base, height - 1, need);
        if (e >= need)
        {
            return limit;
        }
        return powerCapped(base, e, limit);
    }

    static Enc evaluate(long long base, long long height, long long mod)
    {
        // 返回幂塔余数，并独立记录真实值是否达到 mod。
        if (height == 0)
        {
            return {1 % mod, 1 >= mod};
        }
        if (base == 0)
        {
            long long v = height & 1 ? 0 : 1;
            return {v % mod, v >= mod};
        }
        if (mod == 1)
        {
            return {0, towerCapped(base, height, 1) == 1};
        }
        long long ph = phi(mod);
        auto e = evaluate(base, height - 1, ph);
        unsigned long long exponent = (unsigned long long)e.rem;
        if (e.large)
        {
            exponent += (unsigned long long)ph;
        }
        long long rem = powerMod(base, exponent, mod);
        bool large = towerCapped(base, height, mod) == mod;
        return {rem, large};
    }

  public:
    static long long eval(long long base, long long height, long long mod)
    {
        // 返回 height 层 base 幂塔对 mod 的最小非负余数。
        assert(base >= 0 && height >= 0 && mod > 0); // 调试检查，可删
        return evaluate(base, height, mod).rem;
    }
};
