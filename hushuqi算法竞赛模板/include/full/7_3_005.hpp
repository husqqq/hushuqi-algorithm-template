#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T> void divisorZeta(vector<T> &a)
{
    // a 在下标 1..n 保存原函数；原地改成每个位置的约数和，位置 0 不使用。
    assert(!a.empty()); // 调试检查，可删
    int n = a.size() - 1;
    vector<char> comp(n + 1);
    vector<int> p;
    for (int x = 2; x <= n; x++)
    {
        if (!comp[x])
        {
            p.push_back(x);
        }
        for (auto q : p)
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
    for (auto q : p)
    {
        for (int x = 1; x <= n / q; x++)
        {
            a[x * q] += a[x];
        }
    }
}

template <class T> void divisorMu(vector<T> &a)
{
    // a 是 divisorZeta 的结果；原地恢复下标 1..n 的原函数，位置 0 不使用。
    assert(!a.empty()); // 调试检查，可删
    int n = a.size() - 1;
    vector<char> comp(n + 1);
    vector<int> p;
    for (int x = 2; x <= n; x++)
    {
        if (!comp[x])
        {
            p.push_back(x);
        }
        for (auto q : p)
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
    for (auto it = p.rbegin(); it != p.rend(); it++)
    {
        int q = *it;
        for (int x = n / q; x >= 1; x--)
        {
            a[x * q] -= a[x];
        }
    }
}
