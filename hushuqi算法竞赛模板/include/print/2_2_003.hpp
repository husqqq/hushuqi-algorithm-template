#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <class T> struct Discrete
{
    vector<T> a; // a 保存升序且去重后的原值。

    explicit Discrete(vector<T> v) : a(move(v))
    {
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
    }

    int id(const T &x) const
    {
        auto it = lower_bound(a.begin(), a.end(), x);
        assert(it != a.end() && *it == x); // 调试检查，可删
        return it - a.begin();
    }
};

template <class T> int invMerge(vector<T> a)
{
    // a 是按值复制的序列；返回严格逆序对数量。
    vector<T> b;
    b.reserve(a.size());
    int ans = 0;
    auto dfs = [&](auto &&self, int l, int r) -> void
    {
        if (r - l <= 1)
        {
            return;
        }
        auto m = midpoint(l, r);
        self(self, l, m);
        self(self, m, r);
        auto i = l;
        auto j = m;
        b.clear();
        while (i < m || j < r)
        {
            if (j == r || (i < m && !(a[j] < a[i])))
            {
                b.push_back(move(a[i++]));
            }
            else
            {
                ans += m - i;
                b.push_back(move(a[j++]));
            }
        }
        move(b.begin(), b.end(), a.begin() + l);
    };
    dfs(dfs, 0, a.size());
    return ans;
}

template <class T> int invBit(const vector<T> &a)
{
    // a 是原序列；返回用离散化和 Fenwick 统计的严格逆序对数量。
    Discrete<T> dis(a);
    vector<int> bit(dis.a.size() + 1);
    auto add = [&](int p)
    {
        for (p++; p < (int)bit.size(); p += p & -p)
        {
            bit[p]++;
        }
    };
    auto sum = [&](int p)
    {
        int ans = 0;
        for (; p > 0; p -= p & -p)
        {
            ans += bit[p];
        }
        return ans;
    };
    int ans = 0;
    for (auto it = a.rbegin(); it != a.rend(); it++)
    {
        auto p = dis.id(*it);
        ans += sum(p);
        add(p);
    }
    return ans;
}
