#pragma once

#include "7_2_009.hpp"

struct BinomMod
{
    struct PP
    {
        // p 是质因子，e 是 p 在总模数中的指数。
        int p, e;
        // fac 维护模 p^e 的单位阶乘与 p-adic 指数。
        PPFactorial fac;

        PP(int p, int e) : p(p), e(e), fac(p, e)
        {
            // p 是质因子，e 是正指数；建立对应的质数幂阶乘表。
        }

        int modulus() const
        {
            // 无参数；返回本分量的质数幂模数 p^e。
            return fac.modulus();
        }

        int comb(int n, int k) const
        {
            // n、k 是组合数参数；返回 C(n,k) mod p^e，k 不在 [0,n] 时返回 0。
            assert(n >= 0); // 调试检查，可删。
            return fac.binomial(n, k);
        }

        int fact(int n) const
        {
            // n 是非负整数；返回 n! mod p^e。
            return fac.fact(n);
        }
    };

    // mod 是查询所用的正模数，a 保存 mod 的全部质数幂分量。
    int mod;
    vector<PP> a;
    vector<int> coef; // coef[i] 是第 i 个质数幂分量的固定 CRT 重建系数。

    explicit BinomMod(int mod) : mod(mod)
    {
        // mod 是正模数且每个质数幂分量不超过 5e6；分解 mod 并建立单位阶乘表。
        assert(mod > 0); // 调试检查，可删。
        int n = mod;
        for (int p = 2; p <= n / p; p++)
        {
            if (n % p == 0)
            {
                int e = 0;
                while (n % p == 0)
                {
                    n /= p;
                    e++;
                }
                a.emplace_back(p, e);
            }
        }
        if (n > 1)
        {
            a.emplace_back(n, 1);
        }
        for (const auto &q : a)
        {
            int qmod = q.modulus();
            int m = mod / qmod;
            coef.push_back(mulMod(m, *invMod(m, qmod), mod));
        }
    }

    template <class F> long long crt(F get) const
    {
        // get(q) 返回模质数幂 q.mod 的余数；返回 CRT 合并后的模 mod 最小非负余数。
        if (mod == 1)
        {
            return 0;
        }
        long long ans = 0;
        for (int i = 0; i < (int)a.size(); i++)
        {
            int x = mulMod(get(a[i]), coef[i], mod);
            ans = ans >= mod - x ? ans - (mod - x) : ans + x;
        }
        return ans;
    }

    int C(int n, int k) const
    {
        // n、k 是组合数参数；返回 C(n,k) mod mod，k 不在 [0,n] 时返回 0。
        assert(n >= 0); // 调试检查，可删。
        return crt(
            [&](const auto &q)
            {
                return q.comb(n, k);
            });
    }

    int fact(int n) const
    {
        // n 是非负整数；返回 n! mod mod。
        assert(n >= 0); // 调试检查，可删。
        return crt(
            [&](const auto &q)
            {
                return q.fact(n);
            });
    }
};
