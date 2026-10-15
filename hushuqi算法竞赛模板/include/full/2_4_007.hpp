#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct SqrtRangeAdd
{
    int n; // 元素个数。
    int bs; // 块长。
    vector<int> a; // 扣除整块懒标记后的元素值。
    vector<int> lazy; // 各块所有元素共同增加的值。
    vector<int> sum; // 各块包含懒标记的真实元素和。

    SqrtRangeAdd(const vector<int> &v) : n(v.size()), a(v)
    {
        // v 是初始数组；构造支持半开区间加与区间和的分块结构。
        bs = max<int>(1, sqrtl(n));
        lazy.resize((n + bs - 1) / bs);
        sum.resize(lazy.size());
        for (int i = 0; i < n; i++)
        {
            sum[i / bs] += a[i];
        }
    }

    void add(int l, int r, int v)
    {
        // l、r 指定 [l,r)，v 是每个元素的增量；原地修改，无返回值。
        assert(0 <= l && l <= r && r <= n);
        while (l < r)
        {
            int b = l / bs, end = min(n, (b + 1) * bs);
            if (l % bs == 0 && end <= r)
            {
                lazy[b] += v;
                sum[b] += (end - l) * v;
                l = end;
            }
            else
            {
                a[l++] += v;
                sum[b] += v;
            }
        }
    }

    int query(int l, int r) const
    {
        // l、r 指定 [l,r)；返回区间和，空区间返回 0。
        assert(0 <= l && l <= r && r <= n);
        int ans = 0;
        while (l < r)
        {
            int b = l / bs, end = min(n, (b + 1) * bs);
            if (l % bs == 0 && end <= r)
            {
                ans += sum[b];
                l = end;
            }
            else
            {
                ans += a[l++] + lazy[b];
            }
        }
        return ans;
    }
};
