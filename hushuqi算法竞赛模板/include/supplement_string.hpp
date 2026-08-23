#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

// 知识点 4.3.003：LCP、Height 与 RMQ。
namespace suffix_lcp
{
vector<int> lcpArray(const string &s, const vector<int> &sa)
{
    // s 是原字符串，sa 是全部后缀按字典序排列的起点；返回相邻后缀 LCP，ans[i]=lcp(sa[i],sa[i+1])。
    auto n = (int)s.size();
    assert((int)sa.size() == n);
    vector<int> rk(n), ht(max<int>(0, n - 1));
    vector<char> seen(n);
    for (int i = 0; i < n; i++)
    {
        assert(0 <= sa[i] && sa[i] < n && !seen[sa[i]]);
        seen[sa[i]] = true;
        rk[sa[i]] = i;
    }
    int h = 0;
    for (int i = 0; i < n; i++)
    {
        if (rk[i] == n - 1)
        {
            h = 0;
            continue;
        }
        auto j = sa[rk[i] + 1];
        while (i + h < n && j + h < n && s[i + h] == s[j + h])
        {
            h++;
        }
        ht[rk[i]] = h;
        if (h)
        {
            h--;
        }
    }
    return ht;
}

struct LcpRMQ
{
    int n = 0;              // n 是字符串长度。
    vector<int> sa;         // sa[r] 是排名 r 的后缀起点。
    vector<int> rk;         // rk[i] 是后缀 s[i..n) 的排名。
    vector<int> ht;         // ht[i] 是 sa[i] 与 sa[i+1] 的最长公共前缀长度。
    vector<int> lg;         // lg[len] 是不超过 len 的最大二次幂指数。
    vector<vector<int>> st; // st[k][i] 是 ht[i..i+2^k) 的最小值。

    LcpRMQ(const string &s, vector<int> order)
        : n((int)s.size()), sa(move(order)), rk(n), ht(lcpArray(s, sa)), lg(n + 1)
    {
        // s 是原字符串，order 是已构造好的后缀数组；建立 Height 的 ST 表，无返回值。
        for (int i = 0; i < n; i++)
        {
            rk[sa[i]] = i;
        }
        for (int i = 2; i <= n; i++)
        {
            lg[i] = lg[i / 2] + 1;
        }
        if (n <= 1)
        {
            return;
        }
        st.assign(lg[n - 1] + 1, {});
        st[0] = ht;
        for (int k = 1; k < (int)st.size(); k++)
        {
            auto len = 1LL << k;
            st[k].resize(n - len);
            for (int i = 0; i + len <= n - 1; i++)
            {
                st[k][i] = min(st[k - 1][i], st[k - 1][i + len / 2]);
            }
        }
    }

    int queryRank(int l, int r) const
    {
        // l、r 是后缀排名且 l<r；返回 sa[l] 与 sa[r] 的最长公共前缀长度。
        assert(0 <= l && l < r && r < n);
        auto k = lg[r - l];
        return min(st[k][l], st[k][r - (1LL << k)]);
    }

    int query(int i, int j) const
    {
        // i、j 是两个后缀起点；返回 s[i..n) 与 s[j..n) 的最长公共前缀长度。
        assert(0 <= i && i < n && 0 <= j && j < n);
        if (i == j)
        {
            return n - i;
        }
        auto x = rk[i];
        auto y = rk[j];
        if (x > y)
        {
            swap(x, y);
        }
        return queryRank(x, y);
    }
};
}
