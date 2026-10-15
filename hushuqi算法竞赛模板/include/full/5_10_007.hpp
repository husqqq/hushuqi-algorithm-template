#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

int chromaticNum(const vector<unsigned long long> &e)
{
    // e 是简单无向图邻接位集；返回最小点染色数。
    int n = e.size();
    assert(n <= 24); // 调试检查，可删：状态数必须能承受。
    size_t z = 1ULL << n;
    vector<bool> ind(z);
    ind[0] = true;
    for (size_t s = 1; s < z; s++)
    {
        int v = countr_zero((unsigned long long)s);
        size_t t = s ^ (1ULL << v);
        ind[s] = ind[t] && !(e[v] & t);
    }
    vector<int> f(z, n);
    f[0] = 0;
    for (size_t s = 1; s < z; s++)
    {
        size_t b = s & -s;
        for (size_t t = s; t; t = (t - 1) & s)
        {
            if ((t & b) && ind[t])
            {
                f[s] = min(f[s], f[s ^ t] + 1);
            }
        }
    }
    return f.back();
}
