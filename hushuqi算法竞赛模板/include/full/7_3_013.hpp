#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

long long sqrtFloor(long long n)
{
    long long x = sqrtl((long double)n);
    while ((x + 1) <= n / (x + 1))
    {
        x++;
    }
    while (x > n / x)
    {
        x--;
    }
    return x;
}

vector<int8_t> squarefreeMu(int n)
{
    vector<int8_t> mu(n + 1, 1);
    vector<bool> composite(n + 1);
    for (int p = 2; p <= n; p++)
    {
        if (composite[p])
        {
            continue;
        }
        mu[p] = -1;
        for (int j = p * 2; j <= n; j += p)
        {
            composite[j] = true;
            mu[j] = -mu[j];
        }
        if (p <= n / p)
        {
            for (int j = p * p; j <= n; j += p * p)
            {
                mu[j] = 0;
            }
        }
    }
    return mu;
}

long long countSqFree(long long n)
{
    if (n <= 0)
    {
        return 0;
    }
    int I = max<long long>(1, powl((long double)n, 0.2L));
    int D = sqrtFloor(n / I);
    auto mu = squarefreeMu(D);
    vector<int32_t> mertens(D + 1);
    long long direct = 0;
    for (int k = 1; k <= D; k++)
    {
        mertens[k] = mertens[k - 1] + mu[k];
        direct += (long long)mu[k] * (n / k / k);
    }
    mu.clear();
    mu.shrink_to_fit();

    vector<long long> large;
    large.reserve(I - 1);
    long long sum = 0;
    for (int i = I - 1; i >= 1; i--)
    {
        long long x = sqrtFloor(n / i);
        long long s = sqrtFloor(x);
        long long cur = 1;
        for (int q = 1; q <= x / (s + 1); q++)
        {
            cur -= (x / q - x / (q + 1)) * mertens[q];
        }
        for (int j = 2; j <= s; j++)
        {
            long long y = x / j;
            if (y <= D)
            {
                cur -= mertens[y];
            }
            else
            {
                cur -= large[I - j * j * i - 1];
            }
        }
        large.push_back(cur);
        sum += cur;
    }
    return direct + sum - (I - 1) * (long long)mertens[D];
}
