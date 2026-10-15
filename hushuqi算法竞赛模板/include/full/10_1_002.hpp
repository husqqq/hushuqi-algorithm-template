#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

using C = complex<long double>;

void fft(vector<C> &a, bool inv = false)
{
    // a 是待变换序列，inv 表示是否做逆变换；原地完成 FFT，逆变换已除以长度。
    int n = a.size();
    assert(n > 0 && has_single_bit((unsigned long long)n)); // 调试检查，可删。
    int j = 0;
    for (int i = 1; i < n; i++)
    {
        int k = n >> 1;
        while (j & k)
        {
            j ^= k;
            k >>= 1;
        }
        j ^= k;
        if (i < j)
        {
            swap(a[i], a[j]);
        }
    }
    const long double pi = acosl(-1);
    for (int len = 2; len <= n; len <<= 1)
    {
        long double ang = 2 * pi / len * (inv ? -1 : 1);
        C wlen(cosl(ang), sinl(ang));
        for (int l = 0; l < n; l += len)
        {
            C w{1.0L, 0.0L};
            for (int j = 0; j < len / 2; j++)
            {
                C x = a[l + j];
                C y = a[l + j + len / 2] * w;
                a[l + j] = x + y;
                a[l + j + len / 2] = x - y;
                w *= wlen;
            }
        }
    }
    if (inv)
    {
        for (C &x : a)
        {
            x /= n;
        }
    }
}

vector<long double> convolution(const vector<long double> &a, const vector<long double> &b)
{
    // a、b 是实系数序列；返回二者的线性卷积，任一输入为空时返回空。
    if (a.empty() || b.empty())
    {
        return {};
    }
    int sz = a.size() + b.size() - 1;
    int n = bit_ceil((unsigned long long)sz);
    vector<C> x(n);
    vector<C> y(n);
    copy(a.begin(), a.end(), x.begin());
    copy(b.begin(), b.end(), y.begin());
    fft(x);
    fft(y);
    for (int i = 0; i < n; i++)
    {
        x[i] *= y[i];
    }
    fft(x, true);
    vector<long double> c(sz);
    for (int i = 0; i < sz; i++)
    {
        c[i] = x[i].real();
    }
    return c;
}
