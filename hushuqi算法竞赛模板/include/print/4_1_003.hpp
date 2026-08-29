#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

vector<int> zfunc(const string &s)
{
    // s 是原字符串；返回 Z 数组，z[i]=lcp(s,s[i..n))。
    int n = s.size();
    vector<int> z(n);
    if (!n)
    {
        return z;
    }
    z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; ++i)
    {
        if (i < r)
        {
            z[i] = min(r - i, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
        {
            ++z[i];
        }
        if (i + z[i] > r)
        {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}
