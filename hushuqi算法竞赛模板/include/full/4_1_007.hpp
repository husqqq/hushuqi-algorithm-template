#pragma once
#include "3_3_001.hpp"
#include "4_3_002.hpp"
#include "../supplement_string.hpp" // 正文依赖 4.3.003

vector<array<int, 3>> runs(const string &s)
{
    // s 可含任意字节；返回按字典序排列的 {最小周期 p, 左端点 l, 右端点 r}，区间为 [l,r)。
    int n = s.size();
    if (n < 2) return {};
    auto make = [&](const string &text)
    {
        // text 是原串或反串；返回后缀排名及负 LCP 的最大值线段树。
        auto sa = suffixArray(text), h = suffix_lcp::lcpArray(text, sa);
        vector<int> rank(n);
        vector<MaxInfo> v(n - 1);
        for (int i = 0; i < n; i++)
        {
            rank[sa[i]] = i;
            if (i < n - 1) v[i] = MaxInfo{-h[i]};
        }
        return pair{move(rank), Seg<MaxInfo>(v)};
    };
    string rev(s.rbegin(), s.rend());
    auto [rk, pre] = make(s);
    auto [rr, suf] = make(rev);
    auto lce = [&](bool back, int i, int j) -> int
    {
        // back 选反串，否则选原串；i、j 是合法后缀起点，返回其 LCP 长度。
        const auto &text = back ? rev : s;
        if (i == j) return n - i;
        if (text[i] != text[j]) return 0;
        const auto &rank = back ? rr : rk;
        const auto &seg = back ? suf : pre;
        int l = rank[i], r = rank[j];
        if (l > r) swap(l, r);
        // 非空区间上 max(-h) 取反即 min(h)；不对空区间的负无穷单位元取反。
        return -seg.query(l, r).val;
    };
    // 每项依次为 {最小周期, 左端点, 右端点}。
    vector<array<int, 3>> ans;
    for (int dir = 0; dir < 2; dir++)
    {
        vector<int> rank = rk, st;
        st.reserve(n);
        if (dir)
        {
            string opp = s;
            for (char &c : opp)
            {
                c = (char)(255 - (unsigned char)c);
            }
            auto sa = suffixArray(opp);
            for (int i = 0; i < n; i++)
            {
                rank[sa[i]] = i;
            }
        }
        // st 顶端给出右侧第一个后缀排名更小的位置，即当前最长 Lyndon 根的右端点。
        for (int i = n - 1; i >= 0; i--)
        {
            while (!st.empty() && rank[st.back()] > rank[i])
            {
                st.pop_back();
            }
            int j = st.empty() ? n : st.back();
            st.push_back(i);
            if (j == n) continue;
            int p = j - i, x = i ? lce(true, n - i, n - j) : 0;
            if (x >= p) continue;
            int y = lce(false, i, j);
            if (x + y >= p) ans.push_back({p, i - x, j + y});
        }
    }
    sort(ans.begin(), ans.end());
    ans.erase(unique(ans.begin(), ans.end()), ans.end());
    return ans;
}
