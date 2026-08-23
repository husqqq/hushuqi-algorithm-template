#pragma once

#include "12_1_001.hpp"

template<class T>
vector<T> lis(const vector<T> &a)
{
    int n = a.size();
    vector<T> tail;
    vector<int> at;
    vector<int> pre(n, -1);
    for (int i = 0; i < n; i++)
    {
        int k = lower_bound(tail.begin(), tail.end(), a[i]) - tail.begin();
        if (k > 0)
        {
            pre[i] = at[k - 1];
        }
        if (k == (int)tail.size())
        {
            tail.push_back(a[i]);
            at.push_back(i);
        }
        else
        {
            tail[k] = a[i];
            at[k] = i;
        }
    }
    if (at.empty())
    {
        return {};
    }
    vector<T> ans;
    for (int x = at.back(); x != -1; x = pre[x])
    {
        ans.push_back(a[x]);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}
