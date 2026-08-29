#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <class T> vector<int> lis(const vector<T> &a)
{
    // a 是原序列；返回一条严格上升子序列的原下标。
    int n = a.size();
    vector<T> v;
    vector<int> at, pre(n, -1);
    for (int i = 0; i < n; i++)
    {
        int p = lower_bound(v.begin(), v.end(), a[i]) - v.begin();
        if (p == (int)v.size())
        {
            v.push_back(a[i]);
            at.push_back(i);
        }
        else
        {
            v[p] = a[i];
            at[p] = i;
        }
        if (p)
        {
            pre[i] = at[p - 1];
        }
    }
    vector<int> ans;
    for (int x = at.empty() ? -1 : at.back(); x >= 0; x = pre[x])
    {
        ans.push_back(x);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}
