#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T> vector<T> recMul(const vector<T> &a, const vector<T> &b,
                                    const vector<T> &rec)
{
    // a、b 是次数小于 k 的系数；返回乘积模特征多项式后的 k 项系数。
    int k = rec.size();
    assert(k > 0 && (int)a.size() == k && (int)b.size() == k); // 调试检查，可删
    vector<T> c(2 * k - 1);
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            c[i + j] += a[i] * b[j];
        }
    }
    for (int i = 2 * k - 2; i >= k; i--)
    {
        for (int j = 1; j <= k; j++)
        {
            c[i - j] += c[i] * rec[j - 1];
        }
    }
    c.resize(k);
    return c;
}

template <class T>
vector<T> recCoef(const vector<T> &rec, unsigned long long n)
{
    // 返回 x^n 模特征多项式的 k 个系数，系数下标对应 x^0..x^(k-1)。
    int k = rec.size();
    assert(k > 0); // 调试检查，可删
    vector<T> ans(k), x(k);
    ans[0] = T(1);
    if (k == 1)
    {
        x[0] = rec[0];
    }
    else
    {
        x[1] = T(1);
    }
    while (n)
    {
        if (n & 1)
        {
            ans = recMul(ans, x, rec);
        }
        n >>= 1;
        if (n)
        {
            x = recMul(x, x, rec);
        }
    }
    return ans;
}

template <class T> T linearRec(const vector<T> &init, const vector<T> &rec,
                               unsigned long long n)
{
    // init 是至少 k 项初值，rec 定义 a[n]=sum(rec[i-1]*a[n-i])，n 是下标；返回 a[n]。
    int k = rec.size();
    assert(k > 0 && (int)init.size() >= k); // 调试检查，可删。
    if (n < (unsigned long long)k)
    {
        return init[n];
    }
    auto ans = recCoef(rec, n);
    T res{};
    for (int i = 0; i < k; i++)
    {
        res += ans[i] * init[i];
    }
    return res;
}
