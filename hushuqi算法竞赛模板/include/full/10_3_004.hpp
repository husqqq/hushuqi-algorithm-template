#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace binom_trans
{
vector<int> binomTrans(const vector<int> &a, bool inverse = false)
{
    // a 是原序列，inverse 表示做逆变换；返回二项式变换结果。
    int n = a.size();
    vector<int> b(n), c{1};
    for (int i = 0; i < n; i++)
    {
        if (i)
        {
            c.push_back(0);
        }
        for (int j = i; j >= 1; j--)
        {
            c[j] += c[j - 1];
        }
        for (int j = 0; j <= i; j++)
        {
            int sign = inverse && ((i - j) & 1) ? -1 : 1;
            b[i] += sign * c[j] * a[j];
        }
    }
    return b;
}
}
