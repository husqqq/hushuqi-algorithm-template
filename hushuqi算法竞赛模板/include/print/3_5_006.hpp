#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <class T> struct ODT
{
    struct Node
    {
        // [l,r) 是常值段，v 是该段的值。
        int l, r;
        mutable T v;

        bool operator<(const Node &o) const
        {
            // o 是另一常值段；只按左端点排序。
            return l < o.l;
        }
    };

    // n 是数组长度，s 按左端点保存两两不交的常值段。
    int n;
    set<Node> s;

    ODT(const vector<T> &a) : n(a.size())
    {
        // a 是初始数组；空数组得到空段集。
        for (int l = 0, r; l < n; l = r)
        {
            for (r = l + 1; r < n && a[r] == a[l]; r++)
            {
                ;
            }
            s.insert({l, r, a[l]});
        }
    }

    auto split(int x)
    {
        // x 是分割位置；返回左端点为 x 的段，x=n 时返回 end()。
        assert(0 <= x && x <= n); // 调试检查，可删
        if (x == n)
        {
            return s.end();
        }
        auto it = prev(s.upper_bound({x, 0, {}}));
        if (it->l == x)
        {
            return it;
        }
        auto [l, r, v] = *it;
        s.erase(it);
        s.insert({l, x, v});
        return s.insert({x, r, v}).first;
    }

    void assign(int l, int r, const T &v)
    {
        // l、r 是半开区间端点，v 是新值；空区间无操作。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (l == r)
        {
            return;
        }
        auto y = split(r), x = split(l);
        s.erase(x, y);
        s.insert({l, r, v});
    }

    template <class F> void apply(int l, int r, F f)
    {
        // l、r 是半开区间端点；对相交常值段的值逐段调用 f(T&)，空区间不调用。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (l == r)
        {
            return;
        }
        auto y = split(r), x = split(l);
        for (auto it = x; it != y; it++)
        {
            f(it->v);
        }
    }

    T kth(int l, int r, int k)
    {
        // l、r 是非空半开区间端点，k 是 0 基排名；返回按位置计重后的第 k 小值。
        assert(0 <= l && l < r && r <= n && 0 <= k && k < r - l); // 调试检查，可删
        auto y = split(r), x = split(l);
        vector<pair<T, int>> a;
        for (auto it = x; it != y; it++)
        {
            a.push_back({it->v, it->r - it->l});
        }
        sort(a.begin(), a.end());
        for (auto [v, c] : a)
        {
            if (k < c)
            {
                return v;
            }
            k -= c;
        }
        assert(false);
        return {};
    }

    vector<T> values() const
    {
        // 返回当前数组，元素顺序按下标排列。
        vector<T> a(n);
        for (auto [l, r, v] : s)
        {
            fill(a.begin() + l, a.begin() + r, v);
        }
        return a;
    }
};
