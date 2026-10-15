#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Sieve
{
    vector<int> minp;   // minp[x] 是 x 的最小质因子
    vector<int> primes; // 升序质数表

    Sieve(int n) : minp(n + 1)
    {
        // n 是非负筛表上界；线性筛 [2,n] 并填充 minp 与 primes。
        for (int i = 2; i <= n; i++)
        {
            if (!minp[i])
            {
                minp[i] = i;
                primes.push_back(i);
            }
            for (int p : primes)
            {
                if (p > minp[i] || i > n / p)
                {
                    break;
                }
                minp[i * p] = p;
            }
        }
    }

    vector<pair<int, int>> factor(int x) const
    {
        // x 在 [1,minp.size()) 内；返回按质因子升序的 {质因子,指数}。
        assert(1 <= x && x < (int)minp.size()); // 调试检查，可删
        vector<pair<int, int>> ans;
        while (x > 1)
        {
            int p = minp[x];
            int c = 0;
            while (x % p == 0)
            {
                x /= p;
                c++;
            }
            ans.push_back({p, c});
        }
        return ans;
    }
};

vector<int> enumPrimes(int n)
{
    // n 是非负上界；仅标记奇数并返回 [2,n] 的全部质数。
    if (n < 2)
    {
        return {};
    }
    vector<bool> composite(n / 2 + 1);
    for (int p = 3; p <= n / p; p += 2)
    {
        if (composite[p / 2])
        {
            continue;
        }
        for (int x = p * p; x <= n; x += p * 2)
        {
            composite[x / 2] = true;
        }
    }
    vector<int> primes{2};
    for (int x = 3; x <= n; x += 2)
    {
        if (!composite[x / 2])
        {
            primes.push_back(x);
        }
    }
    return primes;
}
