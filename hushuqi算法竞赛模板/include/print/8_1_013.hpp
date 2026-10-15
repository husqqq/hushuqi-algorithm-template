#pragma once

#include "8_2_005.hpp"

template <class T> vector<T> stirlingTr(const vector<T> &a, bool inv = false)
{
    // a[0..n] 是输入序列；inv=false 用第二类数变换，inv=true 用有符号第一类数反演。
    // 返回 b[i]=sum_{j=0..i} t[i][j]*a[j]，其中 t 为对应数表；空序列返回空表。
    if (a.empty()) return {};
    int n = a.size() - 1;
    auto t = inv ? stirling1<T>(n) : stirling2<T>(n);
    vector<T> b(n + 1);
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            b[i] += t[i][j] * a[j];
        }
    }
    return b;
}
