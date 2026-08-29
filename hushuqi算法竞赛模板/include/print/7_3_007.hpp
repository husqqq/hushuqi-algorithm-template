#pragma once
#include "1_1_008.hpp"

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
