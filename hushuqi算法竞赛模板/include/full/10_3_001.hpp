#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace ogf
{
vector<int> ogfProduct(const vector<int> &a, const vector<int> &b)
{
    // a、b 是 OGF 系数；返回乘积系数，即普通卷积。
    if (a.empty() || b.empty())
    {
        return {};
    }
    vector<int> c(a.size() + b.size() - 1);
    for (int i = 0; i < (int)a.size(); i++)
    {
        for (int j = 0; j < (int)b.size(); j++)
        {
            c[i + j] += a[i] * b[j];
        }
    }
    return c;
}
}
