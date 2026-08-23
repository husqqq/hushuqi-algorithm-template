#pragma once

#include "8_2_016.hpp"

class Stirling2P : public SmallPComb
{
    // s[i][j] 是第二类 Stirling 数 S(i,j) mod p，其中 0<=j<=i<p。
    vector<vector<int32_t>> s;

  public:
    explicit Stirling2P(int p) : SmallPComb(p), s(p)
    {
        // p 是小素数；建立第二类低位表与 Lucas 表。
        s[0] = {1};
        for (int i = 1; i < p; i++)
        {
            s[i].assign(i + 1, 0);
            for (int j = 0; j <= i; j++)
            {
                int x = 0;
                if (j)
                {
                    x += s[i - 1][j - 1];
                }
                if (j < i)
                {
                    x += s[i - 1][j] * j % p;
                }
                s[i][j] = x % p;
            }
        }
    }

    int get(int n, int k) const
    {
        // n、k 是非负 64 位下标；返回 S(n,k) mod p，k>n 时返回 0。
        assert(n >= 0); // 调试检查，可删。
        if (k < 0 || k > n)
        {
            return 0;
        }
        if (n < p)
        {
            return s[n][k];
        }
        int i = k / p;
        int j = k % p;
        int a = (n - i) / (p - 1);
        int b = (n - i) % (p - 1);
        if (b == 0)
        {
            b = p - 1;
            a--;
        }
        if (a < 0 || j > b)
        {
            return 0;
        }
        if (b < p - 1)
        {
            return comb(a, i) * s[b][j] % p;
        }
        if (j == 0)
        {
            return comb(a, i - 1);
        }
        return comb(a, i) * s[p - 1][j] % p;
    }
};
