#pragma once
#include "7_1_001.hpp"

class PPFactorial
{
    int p; // 质数底
    int e; // 模数中的质数指数
    int mod = 1; // p^e
    vector<int> unitPrefix; // 删除 p 倍数后的单周期前缀积
    vector<int> unitPrefInv; // unitPrefix 各项的模逆元，供批量组合数查询复用

    int unitFact(int n) const
    {
        // n 是非负整数；返回 n! 删除全部质因子 p 后的单位部分模 mod。
        if (n == 0)
        {
            return 1;
        }
        int z = mulMod(powerMod(unitPrefix[mod], n / mod, mod), unitPrefix[n % mod], mod);
        return mulMod(z, unitFact(n / p), mod);
    }

  public:
    PPFactorial(int p, int e) : p(p), e(e)
    {
        // p 是质数，e 是正指数；预处理模 p^e 的单位前缀积。
        assert(p >= 2 && e >= 1); // 调试检查，可删
        for (int i = 0; i < e; i++)
        {
            assert(mod <= 5000000 / p); // 调试检查，可删
            mod *= p;
        }
        unitPrefix.assign(mod + 1, 1);
        for (int i = 1; i <= mod; i++)
        {
            unitPrefix[i] = mulMod(unitPrefix[i - 1], i % p ? i : 1, mod);
        }
        unitPrefInv.assign(mod + 1, 1);
        unitPrefInv[mod] = *invMod(unitPrefix[mod], mod);
        for (int i = mod; i; i--)
        {
            unitPrefInv[i - 1] = mulMod(unitPrefInv[i], i % p ? i : 1, mod);
        }
    }

    int modulus() const
    {
        // 无参数；返回构造时确定的质数幂模数 p^e。
        return mod;
    }

    int exponent(int n) const
    {
        // n 是非负整数；返回 n! 中质因子 p 的指数。
        assert(n >= 0); // 调试检查，可删
        int ans = 0;
        while (n)
        {
            n /= p;
            ans += n;
        }
        return ans;
    }

    int fact(int n) const
    {
        // n 是非负整数；返回 n! mod p^e。
        assert(n >= 0); // 调试检查，可删
        int q = exponent(n);
        if (q >= e)
        {
            return 0;
        }
        return mulMod(unitFact(n), powerMod(p, q, mod), mod);
    }

    array<int, 2> split(int n) const
    {
        // n 是非负整数；返回 {n! 的 p-adic 指数,删除 p 后的单位部分}。
        assert(n >= 0); // 调试检查，可删
        return {exponent(n), unitFact(n)};
    }

    int binomial(int n, int k) const
    {
        // n、k 是组合数参数；返回 C(n,k) mod p^e，预处理后每次 O(log_p n)。
        if (k < 0 || k > n)
        {
            return 0;
        }
        int r = n - k;
        int e0 = 0, eq = 0, depth = 0;
        int ans = 1;
        while (n)
        {
            ans = mulMod(ans, unitPrefix[n % mod], mod);
            ans = mulMod(ans, unitPrefInv[k % mod], mod);
            ans = mulMod(ans, unitPrefInv[r % mod], mod);
            n /= p;
            k /= p;
            r /= p;
            int carry = n - k - r;
            e0 += carry;
            if (e0 >= e)
            {
                return 0;
            }
            if (++depth >= e)
            {
                eq += carry;
            }
        }
        int delta = p == 2 && e >= 3 ? 1 : mod - 1;
        ans = mulMod(ans, powerMod(delta, eq, mod), mod);
        return mulMod(ans, powerMod(p, e0, mod), mod);
    }
};
