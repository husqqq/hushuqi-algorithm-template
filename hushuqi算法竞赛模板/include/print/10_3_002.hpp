#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace egf
{
vector<int> egfProduct(const vector<int> &a, const vector<int> &b)
{
    // a[n]、b[n] 是乘 n! 前的 EGF 系数；返回有标号乘积系数。
    if (a.empty() || b.empty())
    {
        return {};
    }
    vector<int> ans(a.size() + b.size() - 1);
    vector<int> c(b.size(), 1);
    for (int i = 0; i < (int)a.size(); i++)
    {
        if (i)
        {
            for (int j = 1; j < (int)b.size(); j++)
            {
                c[j] += c[j - 1];
            }
        }
        for (int j = 0; j < (int)b.size(); j++)
        {
            ans[i + j] += c[j] * a[i] * b[j];
        }
    }
    return ans;
}
}
