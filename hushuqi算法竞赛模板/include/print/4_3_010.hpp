#pragma once
#include "3_3_001.hpp"
#include "3_3_002.hpp"
#include "4_3_002.hpp"
#include "4_3_004.hpp"
#include "support/supplement_string.hpp" // 正文依赖 4.3.003

inline long long subDistinct(const string &s, int l, int r)
{
    // s 是原字符串，l、r 是半开端点；返回 s[l,r) 的本质不同子串数量。
    assert(0 <= l && l <= r && r <= (int)s.size()); // 调试检查，可删
    string t = s.substr(l, r - l);
    auto sa = suffixArray(t);
    auto h = suffix_lcp::lcpArray(t, sa);
    long long n = t.size(); // 子串数可达平方级，三角数乘积使用 64 位。
    return n * (n + 1) / 2 - accumulate(h.begin(), h.end(), 0LL);
}

inline vector<long long> rangeSubs(const string &s, const vector<array<int, 2>> &qs)
{
    // s 是小写字母串；qs 每项依次为左端、右端，区间左闭右开，允许空串与空区间。
    // 返回与 qs 同序的本质不同非空子串数；计数可达平方级，关闭整数宏仍用 long long。
    int n = 0, q = qs.size();
    vector<long long> ans(q);
    for (auto [l, r] : qs)
    {
        assert(0 <= l && l <= r && r <= (int)s.size());
        if (l < r) n = max(n, r);
    }
    if (!n) return ans;
    // at[r] 的记录依次为左端、原查询编号。
    vector<vector<array<int, 2>>> at(n + 1);
    for (int i = 0; i < q; i++)
    {
        auto [l, r] = qs[i];
        if (l < r) at[r].push_back({l, i});
    }
    SAM<26, 'a'> sam;
    for (int i = 0; i < n; i++)
    {
        sam.add(s[i]);
    }
    int states = sam.t.size();
    // sz[u] 是后缀链接树的子树大小；子树占编号区间 [in[u],in[u]+sz[u])。
    vector<int> sz(states, 1), in(states);
    {
        // deg 是尚未处理的儿子数，ord 按叶到根拓扑排序，pos 是各子树的下一空位。
        vector<int> deg(states), ord, pos(states);
        for (int v = 1; v < states; v++)
        {
            deg[sam.t[v].link]++;
        }
        for (int v = 0; v < states; v++)
        {
            if (!deg[v]) ord.push_back(v);
        }
        for (int i = 0; i < (int)ord.size(); i++)
        {
            int v = ord[i], p = sam.t[v].link;
            if (p < 0) continue;
            sz[p] += sz[v];
            if (!--deg[p]) ord.push_back(p);
        }
        pos[0] = 1;
        for (int i = states - 2; i >= 0; i--)
        {
            int v = ord[i], p = sam.t[v].link;
            in[v] = pos[p];
            pos[p] += sz[v];
            pos[v] = in[v] + 1;
        }
    }
    LazySeg<Info, Tag> seg(n); // 各起点已被后续相同子串替代的出现次数取负。
    Seg<MaxInfo> mx(vector<MaxInfo>(states, MaxInfo{0})); // 子树中最近的已扫描前缀末位置。
    vector<int> top(n + 1); // top[p] 是颜色 p 所在链段的上边界，不含该状态。
    vector<unsigned char> seen(states); // 标记已出现过的后缀链接树状态。
    int u = 0;
    for (int r = 1; r <= n; r++)
    {
        u = sam.t[u].next[s[r - 1] - 'a'];
        int v = u;
        while (v && !seen[v])
        {
            seen[v] = 1;
            v = sam.t[v].link;
        }
        while (v)
        {
            int p = mx.query(in[v], in[v] + sz[v]).val;
            int old = top[p];
            seg.apply(p - sam.t[v].len, p - sam.t[old].len, Tag{-1});
            swap(top[p], v);
        }
        mx.set(in[u], MaxInfo{r}); // r 严格递增，直接覆盖等价于单点取最大。
        top[r] = 0;
        for (auto [l, id] : at[r])
        {
            long long len = r - l; // 区间长度乘积必须在 64 位中计算。
            ans[id] = len * (len + 1) / 2 + seg.query(l, r).sum;
        }
    }
    return ans;
}
