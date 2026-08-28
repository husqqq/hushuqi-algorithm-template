#pragma once

#include <bits/stdc++.h>
#include "../../hushuqi算法竞赛模板/include/print/3_3_002.hpp"
using namespace std;
#define int long long

struct SAM
{
    // to、link、len 分别是转移、后缀链接和状态最长串长。
    vector<array<int, 26>> to;
    vector<int> link, len;
    int sz = 1, last = 1;
    SAM(int n) : to(2 * n + 2), link(2 * n + 2), len(2 * n + 2)
    {
        // n 是原串长度上界；预留 2n 个状态。
    }
    void add(int c)
    {
        // c 是 0..25 的字符；把字符加入自动机。
        int cur = ++sz, p = last;
        len[cur] = len[last] + 1;
        while (p && !to[p][c])
        {
            to[p][c] = cur;
            p = link[p];
        }
        if (!p)
        {
            link[cur] = 1;
        }
        else
        {
            int q = to[p][c];
            if (len[q] == len[p] + 1)
            {
                link[cur] = q;
            }
            else
            {
                int cp = ++sz;
                to[cp] = to[q];
                len[cp] = len[p] + 1;
                link[cp] = link[q];
                while (p && to[p][c] == q)
                {
                    to[p][c] = cp;
                    p = link[p];
                }
                link[q] = link[cur] = cp;
            }
        }
        last = cur;
    }
};

struct MaxSeg
{
    // n 是叶子数；tr 是区间最大值。
    int n, z;
    vector<int> tr;
    MaxSeg(int m) : n(m), z(1)
    {
        // m 是叶子数；初始化为 0。
        while (z < n)
        {
            z <<= 1;
        }
        tr.assign(2 * z, 0);
    }
    void set(int p, int v)
    {
        // p 是位置，v 是候选值；执行单点取最大。
        p += z;
        tr[p] = max(tr[p], v);
        for (; p >>= 1;)
        {
            tr[p] = max(tr[2 * p], tr[2 * p + 1]);
        }
    }
    int query(int l, int r) const
    {
        // [l,r) 是查询区间；返回最大值。
        int x = 0, y = 0;
        for (l += z, r += z; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1)
            {
                x = max(x, tr[l++]);
            }
            if (r & 1)
            {
                y = max(tr[--r], y);
            }
        }
        return max(x, y);
    }
};

vector<int> rangeSubs(const string &s, const vector<pair<int, int>> &qs)
{
    // s 是小写字母串；qs 是 0 下标左闭右开区间；返回本质不同子串数。
    int n = s.size(), q = qs.size();
    assert(n > 0);
    vector<vector<pair<int, int>>> que(n + 1);
    for (int i = 0; i < q; i++)
    {
        int l = qs[i].first, r = qs[i].second;
        assert(0 <= l && l < r && r <= n);
        que[r].push_back({l + 1, i});
    }
    SAM sam(n);
    for (char c : s)
    {
        sam.add(c - 'a');
    }
    vector<vector<int>> ch(sam.sz + 1);
    for (int i = 2; i <= sam.sz; i++)
    {
        ch[sam.link[i]].push_back(i);
    }
    vector<int> in(sam.sz + 1), out(sam.sz + 1), it(sam.sz + 1), st{1};
    int tim = 0;
    while (!st.empty())
    {
        int u = st.back();
        if (!in[u])
        {
            in[u] = ++tim;
        }
        if (it[u] < (int)ch[u].size())
        {
            st.push_back(ch[u][it[u]++]);
        }
        else
        {
            out[u] = tim;
            st.pop_back();
        }
    }
    LazySeg<Info, Tag> seg(n);
    MaxSeg mx(sam.sz + 1);
    vector<int> top(n + 1, 1);
    vector<char> vis(sam.sz + 1);
    vector<int> ans(q);
    int u = 1;
    for (int r = 1; r <= n; r++)
    {
        u = sam.to[u][s[r - 1] - 'a'];
        int v = u;
        while (v != 1 && !vis[v])
        {
            vis[v] = 1;
            v = sam.link[v];
        }
        while (v != 1)
        {
            int pos = mx.query(in[v] - 1, out[v]);
            int old = top[pos];
            seg.apply(pos - sam.len[v], pos - sam.len[old], Tag{-1});
            swap(top[pos], v);
        }
        mx.set(in[u] - 1, r);
        top[r] = 1;
        for (auto [l, id] : que[r])
        {
            int z = r - l + 1;
            ans[id] = z * (z + 1) / 2 + seg.query(l - 1, r).sum;
        }
    }
    return ans;
}
