#pragma once
#include "3_2_001.hpp"

vector<int> prefixSubLCS(const string &s, const string &t, const vector<array<int, 3>> &ask)
{
    // s、t 是任意字节串；ask 每项依次为首串前缀长度、次串左端、次串右端。
    // 返回与 ask 同序的 LCS 长度，所有区间左闭右开，允许空前缀与空子串。
    int end = 0, n = 0, q = ask.size();
    vector<int> ans(q);
    for (auto [a, l, r] : ask)
    {
        assert(0 <= a && a <= (int)s.size() && 0 <= l && l <= r && r <= (int)t.size());
        if (a && l < r)
        {
            end = max(end, a);
            n = max(n, r);
        }
    }
    if (!end) return ans;
    // byA[a] 的记录依次为次串左端、右端、原查询编号。
    vector<vector<array<int, 3>>> byA(end);
    for (int i = 0; i < q; i++)
    {
        auto [a, l, r] = ask[i];
        if (a && l < r) byA[a - 1].push_back({l, r, i});
    }
    // h[j] 是从次串第 j 列下边界离开的路径起点；所有负起点合并为 -1。
    vector<int> h(n);
    iota(h.begin(), h.end(), 0);
    for (int a = 0; a < end; a++)
    {
        int cur = -1; // 当前行左侧进入的路径起点。
        for (int j = 0; j < n; j++)
        {
            if (s[a] == t[j] || h[j] < cur) swap(h[j], cur);
        }
        if (byA[a].empty()) continue;
        // byR[r] 的记录依次为次串左端、原查询编号。
        vector<vector<array<int, 2>>> byR(n);
        for (auto [l, r, id] : byA[a])
        {
            byR[r - 1].push_back({l, id});
        }
        BIT<int> bit(n);
        int cnt = 0; // 已扫描列中非负起点的个数。
        for (int r = 0; r < n; r++)
        {
            if (h[r] != -1)
            {
                bit.add(h[r], 1);
                cnt++;
            }
            for (auto [l, id] : byR[r])
            {
                ans[id] = r + 1 - l - cnt + bit.sum(l);
            }
        }
    }
    return ans;
}
