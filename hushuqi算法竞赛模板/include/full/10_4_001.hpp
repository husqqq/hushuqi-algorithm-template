#pragma once

#include "7_1_007.hpp"
using namespace std;
#define int long long

template <class T> vector<T> BM(const vector<T> &s)
{
    // s 是已知序列前缀；返回最短 rec，使 s[n]=sum(rec[i-1]*s[n-i])。
    vector<T> c;
    vector<T> old;
    int len = 0;
    int gap = 1;
    T last = 1;
    for (int n = 0; n < (int)s.size(); n++)
    {
        T d = s[n];
        for (int i = 1; i <= len; i++)
        {
            d += c[i - 1] * s[n - i];
        }
        if (d == T{})
        {
            gap++;
            continue;
        }
        auto pre = c;
        T q = -d / last;
        if ((int)c.size() < (int)old.size() + gap)
        {
            c.resize(old.size() + gap);
        }
        c[gap - 1] += q;
        for (int i = 0; i < (int)old.size(); i++)
        {
            c[i + gap] += q * old[i];
        }
        if (2 * len <= n)
        {
            len = n + 1 - len;
            old = move(pre);
            last = d;
            gap = 1;
        }
        else
        {
            gap++;
        }
    }
    for (auto &x : c)
    {
        x = -x;
    }
    return c;
}
