#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T = long long, class F> vector<T> multTable(int n, F primePower)
{
    // n 是非负上界；primePower(p,e) 返回 f(p^e)，且 f 必须为积性函数；返回 f[0..n]，位置 0 不使用。
    assert(n >= 0); // 调试检查，可删
    vector<T> f(n + 1);
    vector<int> minp(n + 1);
    vector<int> exp(n + 1);
    vector<int> pp(n + 1);
    vector<int> primes;
    if (n >= 1)
    {
        f[1] = 1;
        pp[1] = 1;
    }
    for (int x = 2; x <= n; x++)
    {
        if (minp[x] == 0)
        {
            minp[x] = x;
            exp[x] = 1;
            pp[x] = x;
            primes.push_back(x);
            f[x] = primePower(x, 1);
        }
        for (auto p : primes)
        {
            if (p > minp[x] || x > n / p)
            {
                break;
            }
            int y = x * p;
            minp[y] = p;
            if (p == minp[x])
            {
                exp[y] = exp[x] + 1;
                pp[y] = pp[x] * p;
                f[y] = f[x / pp[x]] * primePower(p, exp[y]);
                break;
            }
            exp[y] = 1;
            pp[y] = p;
            f[y] = f[x] * primePower(p, 1);
        }
    }
    return f;
}
