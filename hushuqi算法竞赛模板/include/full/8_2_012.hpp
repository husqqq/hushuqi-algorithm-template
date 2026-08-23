#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace polya
{
template <class T = long long>
T burnside(const vector<vector<int>> &group, int colors)
{
    // group 中每项是位置置换，colors 是颜色数；返回本质不同染色数。
    static_assert(numeric_limits<T>::is_integer && numeric_limits<T>::is_signed &&
                  sizeof(T) >= sizeof(long long));
    assert(!group.empty() && colors >= 0); // 调试检查，可删。
    int n = group[0].size();
    T sum = 0;
    for (const auto &p : group)
    {
        assert((int)p.size() == n); // 调试检查，可删。
        vector<int> cnt(n);
        for (int x : p)
        {
            assert(0 <= x && x < n); // 调试检查，可删。
            cnt[x]++;
        }
        for (int x : cnt)
        {
            assert(x == 1); // 调试检查，可删。
        }
        vector<int> vis(n);
        int cyc = 0;
        for (int i = 0; i < n; i++)
        {
            if (vis[i])
            {
                continue;
            }
            cyc++;
            int x = i;
            while (!vis[x])
            {
                vis[x] = 1;
                x = p[x];
            }
        }
        T ways = 1;
        for (int i = 0; i < cyc; i++)
        {
            ways *= colors;
        }
        sum += ways;
    }
    int sz = group.size();
    assert(sum % sz == 0); // 调试检查，可删。
    return sum / sz;
}
}
