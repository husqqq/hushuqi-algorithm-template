#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

using Complex = complex<long double>;

vector<Complex> dftNaive(const vector<Complex> &a, bool inv = false)
{
    // a 是时域或频域序列，inv=false/true 表示 DFT/IDFT；返回变换结果，逆变换已除以长度。
    int n = a.size();
    vector<Complex> b(n);
    long double pi = acosl(-1);
    for (int k = 0; k < n; k++)
    {
        for (int j = 0; j < n; j++)
        {
            long double ang = 2 * pi * j * k / n * (inv ? -1 : 1);
            b[k] += a[j] * Complex(cosl(ang), sinl(ang));
        }
        if (inv)
        {
            b[k] /= n;
        }
    }
    return b;
}
