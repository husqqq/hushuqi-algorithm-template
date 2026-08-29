#pragma once
#include "3_3_002.hpp"
#include "4_3_002.hpp"
#include "4_3_004.hpp"
#include "support/supplement_string.hpp"

inline long long subDistinct(const string &s, int l, int r)
{
    // s 是原字符串，l、r 是半开端点；返回 s[l,r) 的本质不同子串数量。
    assert(0 <= l && l <= r && r <= (int)s.size()); // 调试检查，可删
    string t = s.substr(l, r - l);
    auto sa = suffixArray(t);
    auto h = suffix_lcp::lcpArray(t, sa);
    long long n = t.size();
    return n * (n + 1) / 2 - accumulate(h.begin(), h.end(), 0LL);
}

struct RangeSubMax
{
    // n、z 是叶子数和补齐后的二次幂；tr 保存区间最大值。
    int n, z = 1;
    vector<int> tr;

    RangeSubMax(int n) : n(n)
    {
        // n 是叶子数；初始各位置均为 0。
        while (z < n) z <<= 1;
        tr.assign(2 * z, 0);
    }

    void setMax(int p, int v)
    {
        // p 是位置，v 是候选值；执行单点取最大。
        for (p += z, tr[p] = max(tr[p], v); p >>= 1;)
            tr[p] = max(tr[2 * p], tr[2 * p + 1]);
    }

    int query(int l, int r) const
    {
        // l、r 是半开端点；返回 [l,r) 的最大值。
        int ans = 0;
        for (l += z, r += z; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1) ans = max(ans, tr[l++]);
            if (r & 1) ans = max(ans, tr[--r]);
        }
        return ans;
    }
};

inline vector<int> rangeSubs(const string &s,
                             const vector<pair<int, int>> &qs)
{
    // s 是小写字母串；qs 是 0 下标半开区间；返回各区间的本质不同子串数。
    int n = s.size(), q = qs.size();
    assert(n > 0); // 调试检查，可删
    vector<vector<pair<int, int>>> at(n + 1);
    for (int i = 0; i < q; i++)
    {
        auto [l, r] = qs[i];
        assert(0 <= l && l < r && r <= n); // 调试检查，可删
        at[r].push_back({l + 1, i});
    }
    SAM<26, 'a'> sam(s);
    int states = sam.t.size();
    vector<vector<int>> son(states);
    for (int i = 1; i < states; i++) son[sam.t[i].link].push_back(i);
    vector<int> in(states), out(states), it(states), st{0};
    int tim = 0;
    while (!st.empty())
    {
        int u = st.back();
        if (!in[u]) in[u] = ++tim;
        if (it[u] < (int)son[u].size()) st.push_back(son[u][it[u]++]);
        else out[u] = tim, st.pop_back();
    }
    LazySeg<Info, Tag> seg(n);
    RangeSubMax mx(states + 1);
    vector<int> top(n + 1), ans(q);
    vector<unsigned char> seen(states);
    int u = 0;
    for (int r = 1; r <= n; r++)
    {
        u = sam.t[u].next[s[r - 1] - 'a'];
        int v = u;
        while (v && !seen[v]) seen[v] = 1, v = sam.t[v].link;
        while (v)
        {
            int p = mx.query(in[v] - 1, out[v]);
            int old = top[p];
            seg.apply(p - sam.t[v].len, p - sam.t[old].len, Tag{-1});
            swap(top[p], v);
        }
        mx.setMax(in[u] - 1, r);
        top[r] = 0;
        for (auto [l, id] : at[r])
        {
            int len = r - l + 1;
            ans[id] = len * (len + 1) / 2 + seg.query(l - 1, r).sum;
        }
    }
    return ans;
}
