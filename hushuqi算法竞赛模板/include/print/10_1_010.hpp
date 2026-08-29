#pragma once

#include "1_1_008.hpp"
using namespace std;
#define int long long

template <class T> void divisorZeta(vector<T> &a)
{
    // a[n] 是下标 n 的系数；原地改成所有约数系数之和，a[0] 不参与。
    int n = a.size();
    for (int d = n - 1; d >= 1; d--)
    {
        for (int m = d * 2; m < n; m += d)
        {
            a[m] += a[d];
        }
    }
}

template <class T> void divMob(vector<T> &a)
{
    // a[n] 是约数 Zeta 变换后的系数；原地恢复原序列，a[0] 不参与。
    int n = a.size();
    for (int d = 1; d < n; d++)
    {
        for (int m = d * 2; m < n; m += d)
        {
            a[m] -= a[d];
        }
    }
}

template <class T> void multipleZeta(vector<T> &a)
{
    // a[n] 是下标 n 的系数；原地改成所有倍数系数之和，a[0] 不参与。
    int n = a.size();
    for (int d = 1; d < n; d++)
    {
        for (int m = d * 2; m < n; m += d)
        {
            a[d] += a[m];
        }
    }
}

template <class T> void mulMob(vector<T> &a)
{
    // a[n] 是倍数 Zeta 变换后的系数；原地恢复原序列，a[0] 不参与。
    int n = a.size();
    for (int d = n - 1; d >= 1; d--)
    {
        for (int m = d * 2; m < n; m += d)
        {
            a[d] -= a[m];
        }
    }
}
