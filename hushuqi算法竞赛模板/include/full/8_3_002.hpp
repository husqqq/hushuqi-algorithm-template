#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

unsigned long long cantor(const vector<int> &p)
{
    // p 是由 0 到 n-1 组成的排列；返回其从零开始的字典序排名。
    int n = p.size();
    assert(n <= 20); // 调试检查，可删。
    vector<int> seen(n);
    for (int x : p)
    {
        assert(0 <= x && x < n && !seen[x]); // 调试检查，可删。
        seen[x] = 1;
    }
    vector<unsigned long long> f(n + 1, 1);
    for (int i = 1; i <= n; i++)
    {
        f[i] = f[i - 1] * i;
    }
    vector<int> bit(n + 1);
    auto add = [&](int x)
    {
        // x 是要加入的排列值；把它在树状数组中的计数加一。
        for (++x; x <= n; x += x & -x)
        {
            bit[x]++;
        }
    };
    auto sum = [&](int x)
    {
        // x 是值域右端；返回已加入值中严格小于 x 的个数。
        int ans = 0;
        for (; x; x -= x & -x)
        {
            ans += bit[x];
        }
        return ans;
    };
    unsigned long long ans = 0;
    for (int i = n - 1; i >= 0; i--)
    {
        ans += f[n - 1 - i] * sum(p[i]);
        add(p[i]);
    }
    return ans;
}

vector<int> invCantor(int n, unsigned long long k)
{
    // n 是排列长度，k 是从零开始的排名；返回由 0 到 n-1 组成的对应排列。
    assert(0 <= n && n <= 20); // 调试检查，可删。
    vector<unsigned long long> f(n + 1, 1);
    for (int i = 1; i <= n; i++)
    {
        f[i] = f[i - 1] * i;
    }
    assert(k < f[n]); // 调试检查，可删。
    vector<int> a(n), ans;
    iota(a.begin(), a.end(), 0);
    for (int i = n; i; i--)
    {
        unsigned long long q = k / f[i - 1];
        k %= f[i - 1];
        assert(q < a.size()); // 调试检查，可删。
        ans.push_back(a[q]);
        a.erase(a.begin() + q);
    }
    return ans;
}
