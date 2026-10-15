#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

template <class T> struct SuffixBIT
{
    // n 是数组长度；a[i] 是反向 lowbit 块和。
    int n;
    vector<T> a;

    SuffixBIT(int n = 0) : n(n), a(n + 1)
    {
        // n 是数组长度；初值全为零。
    }

    void add(int x, T v)
    {
        // x 是 0 下标位置，v 是增量；把该点增加 v。
        for (++x; x; x -= x & -x)
        {
            a[x] += v;
        }
    }

    T sumSuffix(int l) const
    {
        // l 是后缀左端点；返回 [l,n) 的和，允许 l=n。
        T ans{};
        for (++l; l <= n; l += l & -l)
        {
            ans += a[l];
        }
        return ans;
    }

    T sum(int l, int r) const
    {
        // l、r 是半开区间端点；返回 [l,r) 的和。
        return sumSuffix(l) - sumSuffix(r);
    }
};
