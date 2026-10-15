#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<int> phiTable(int n)
{
    // n 是非负筛表上界；返回 phi[0..n]，其中 phi[1]=1。
    assert(n >= 0); // 调试检查，可删
    vector<int> phi(n + 1);
    vector<int> minp(n + 1);
    vector<int> primes;
    if (n >= 1)
    {
        phi[1] = 1;
    }
    for (int i = 2; i <= n; i++)
    {
        if (minp[i] == 0)
        {
            minp[i] = i;
            primes.push_back(i);
            phi[i] = i - 1;
        }
        for (int p : primes)
        {
            if (p > minp[i] || i > n / p)
            {
                break;
            }
            minp[i * p] = p;
            if (i % p == 0)
            {
                phi[i * p] = phi[i] * p;
                break;
            }
            phi[i * p] = phi[i] * (p - 1);
        }
    }
    return phi;
}

vector<int> mobiusTable(int n)
{
    // n 是非负筛表上界；返回 mu[0..n]，其中 mu[1]=1。
    assert(n >= 0); // 调试检查，可删
    vector<int> mu(n + 1);
    vector<int> minp(n + 1);
    vector<int> primes;
    if (n >= 1)
    {
        mu[1] = 1;
    }
    for (int i = 2; i <= n; i++)
    {
        if (minp[i] == 0)
        {
            minp[i] = i;
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p : primes)
        {
            if (p > minp[i] || i > n / p)
            {
                break;
            }
            minp[i * p] = p;
            if (i % p == 0)
            {
                mu[i * p] = 0;
                break;
            }
            mu[i * p] = -mu[i];
        }
    }
    return mu;
}

template <class F> vector<int> multTable(int n, F primePower)
{
    // primePower(p,e) 给出 f(p^e)；返回积性函数 f 在 0..n 的值。
    assert(n >= 0); // 调试检查，可删
    vector<int> f(n + 1);
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
        for (int p : primes)
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

template <class T> vector<T> dirichlet(const vector<T> &a,
                                            const vector<T> &b)
{
    // a、b 的位置 1..n 保存算术函数；返回公共上界内的 Dirichlet 卷积。
    assert(!a.empty() && !b.empty()); // 调试检查，可删
    int n = min(a.size(), b.size()) - 1;
    vector<T> c(n + 1);
    for (int d = 1; d <= n; d++)
    {
        for (int k = 1; k <= n / d; k++)
        {
            c[d * k] += a[d] * b[k];
        }
    }
    return c;
}

inline vector<int> divTransP(int n)
{
    // 返回不超过 n 的全部素数，供整除偏序变换逐维更新。
    vector<char> comp(n + 1);
    vector<int> p;
    for (int x = 2; x <= n; x++)
    {
        if (!comp[x])
        {
            p.push_back(x);
        }
        for (int q : p)
        {
            if (q > n / x)
            {
                break;
            }
            comp[x * q] = true;
            if (x % q == 0)
            {
                break;
            }
        }
    }
    return p;
}

template <class T> void divisorZeta(vector<T> &a)
{
    // 把 a[1..n] 原地变成各位置的约数和。
    assert(!a.empty()); // 调试检查，可删
    int n = a.size() - 1;
    for (int q : divTransP(n))
    {
        for (int x = 1; x <= n / q; x++)
        {
            a[x * q] += a[x];
        }
    }
}

template <class T> void divisorMu(vector<T> &a)
{
    // 把 divisorZeta 的结果原地恢复成原函数。
    assert(!a.empty()); // 调试检查，可删
    int n = a.size() - 1;
    auto p = divTransP(n);
    for (auto it = p.rbegin(); it != p.rend(); it++)
    {
        int q = *it;
        for (int x = n / q; x >= 1; x--)
        {
            a[x * q] -= a[x];
        }
    }
}

vector<array<int, 3>> factorStats(int n)
{
    // 返回 {omega(x),Omega(x),d(x)} 表，三维依次为不同质因子数、含重数质因子数、约数数。
    assert(n >= 0); // 调试检查，可删
    vector<array<int, 3>> s(n + 1);
    vector<int> minp(n + 1);
    vector<int> exp(n + 1);
    vector<int> primes;
    if (n >= 1)
    {
        s[1] = {0, 0, 1};
    }
    for (int x = 2; x <= n; x++)
    {
        if (minp[x] == 0)
        {
            minp[x] = x;
            exp[x] = 1;
            primes.push_back(x);
            s[x] = {1, 1, 2};
        }
        for (int p : primes)
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
                s[y] = {s[x][0], s[x][1] + 1,
                        s[x][2] / (exp[x] + 1) * (exp[y] + 1)};
                break;
            }
            exp[y] = 1;
            s[y] = {s[x][0] + 1, s[x][1] + 1, s[x][2] * 2};
        }
    }
    return s;
}
