#pragma once

#include "10_1_003.hpp"

void kFWT(vector<Z> &a, int k, bool inv = false)
{
    // a 按 k 进制向量编号，k 是每一位的取值数；inv 指定正变换或逆变换，结果写回 a。
    assert(!a.empty() && k > 1 && (Z::mod() - 1) % k == 0); // 调试检查，可删。
    int n = a.size();
    int q = n;
    while (q % k == 0)
    {
        q /= k;
    }
    assert(q == 1); // 调试检查，可删。
    Z w = Z(3).pow((Z::mod() - 1) / k);
    if (inv)
    {
        w = w.inv();
    }
    vector<Z> pw(k, 1);
    vector<Z> x(k);
    vector<Z> y(k);
    for (int i = 1; i < k; i++)
    {
        pw[i] = pw[i - 1] * w;
    }
    for (int len = 1; len < n; len *= k)
    {
        for (int l = 0; l < n; l += len * k)
        {
            for (int p = 0; p < len; p++)
            {
                for (int i = 0; i < k; i++)
                {
                    x[i] = a[l + i * len + p];
                }
                fill(y.begin(), y.end(), Z(0));
                for (int i = 0; i < k; i++)
                {
                    for (int j = 0; j < k; j++)
                    {
                        y[i] += x[j] * pw[i * j % k];
                    }
                }
                for (int i = 0; i < k; i++)
                {
                    a[l + i * len + p] = y[i];
                }
            }
        }
    }
    if (inv)
    {
        Z q = Z(n).inv();
        for (Z &x : a)
        {
            x *= q;
        }
    }
}
