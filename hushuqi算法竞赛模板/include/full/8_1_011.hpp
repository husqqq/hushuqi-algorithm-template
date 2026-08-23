#pragma once

#include <bits/stdc++.h>
#include "../combinatorics_mod.hpp"
using namespace std;
#define int long long

class QBinomPrime
{
    // p 是素数模，q 是规范到 [0,p) 的参数，d 是首个零 q-整数的位置；无零时 d=p。
    int p, q, d;
    // fac/ifac 是 0..d-1 的 q-阶乘及逆元，cfac/cifac 是高位普通阶乘及逆元。
    vector<int> fac, ifac, cfac, cifac;

    int mul(int a, int b) const
    {
        // a、b 是模 p 乘数；返回 ab mod p。
        return combMulMod(a, b, p);
    }

    int power(int a, int n) const
    {
        // a 是模 p 底数，n 是非负指数；返回 a^n mod p。
        int r = 1;
        while (n)
        {
            if (n & 1)
            {
                r = mul(r, a);
            }
            a = mul(a, a);
            n >>= 1;
        }
        return r;
    }

    int small(int n, int k) const
    {
        // 0<=n<d，k 是下标；返回低位 q-二项式，k 不在 [0,n] 时返回 0。
        if (k < 0 || k > n)
        {
            return 0;
        }
        return mul(mul(fac[n], ifac[k]), ifac[n - k]);
    }

    int ordinary(int n, int k) const
    {
        // n 是高位上标，k 是高位下标；返回普通 C(n,k) mod p。
        if (k < 0 || k > n)
        {
            return 0;
        }
        return mul(mul(cfac[n], cifac[k]), cifac[n - k]);
    }

  public:
    QBinomPrime(int p, int q, int limit = -1) : p(p), q(q)
    {
        // p 是素数，q 是 q-二项式参数，limit 是最大查询 n；建立所需范围的 q-Lucas 表。
        assert(p >= 2); // 调试检查，可删。
        if (limit < 0) limit = p - 1;
        assert(0 <= limit && limit < p);
        this->q %= p;
        if (this->q < 0)
        {
            this->q += p;
        }
        fac = {1};
        vector<int> num;
        int x = 1;
        for (int i = 1; i <= limit; i++)
        {
            if (x == 0)
            {
                break;
            }
            num.push_back(x);
            fac.push_back(mul(fac.back(), x));
            x = (mul(this->q, x) + 1) % p;
        }
        bool found = x == 0;
        d = found ? (int)fac.size() : limit + 1;
        ifac.resize(d);
        ifac[d - 1] = power(fac[d - 1], p - 2);
        for (int i = d - 1; i >= 1; i--)
        {
            ifac[i - 1] = mul(ifac[i], num[i - 1]);
        }

        int m = limit / d;
        cfac.assign(m + 1, 1);
        cifac.assign(m + 1, 1);
        for (int i = 1; i <= m; i++)
        {
            cfac[i] = mul(cfac[i - 1], i);
        }
        cifac[m] = power(cfac[m], p - 2);
        for (int i = m; i >= 1; i--)
        {
            cifac[i - 1] = mul(cifac[i], i);
        }
    }

    int C(int n, int k) const
    {
        // n、k 是 q-二项式参数且 0<=n<p；返回 [n choose k]_q mod p，k 越界时返回 0。
        assert(0 <= n && n < p && n / d < (int)cfac.size()); // 调试检查，可删。
        if (k < 0 || k > n)
        {
            return 0;
        }
        return mul(ordinary(n / d, k / d), small(n % d, k % d));
    }
};
