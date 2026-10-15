#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T> class Comb
{
    // fac[i]、ifac[i] 分别保存 i! 与其乘法逆元。
    vector<T> fac{1}, ifac{1};

  public:
    void init(int n)
    {
        // n 是要支持的最大上标；把阶乘表扩展到 n，无返回值。
        assert(n >= 0); // 调试检查，可删。
        if ((int)fac.size() > n)
        {
            return;
        }
        int old = fac.size();
        fac.resize(n + 1);
        ifac.resize(n + 1);
        for (int i = old; i <= n; i++)
        {
            fac[i] = fac[i - 1] * T(i);
        }
        ifac[n] = T(1) / fac[n];
        for (int i = n; i >= old; i--)
        {
            ifac[i - 1] = ifac[i] * T(i);
        }
    }

    T C(int n, int k)
    {
        // n 是对象数，k 是选择数；返回组合数，k 不在 [0,n] 时返回 0。
        if (k < 0 || k > n)
        {
            return T(0);
        }
        init(n);
        return fac[n] * ifac[k] * ifac[n - k];
    }

    T A(int n, int k)
    {
        // n 是对象数，k 是有序选取数；返回排列数，k 不在 [0,n] 时返回 0。
        if (k < 0 || k > n)
        {
            return T(0);
        }
        init(n);
        return fac[n] * ifac[n - k];
    }
};
