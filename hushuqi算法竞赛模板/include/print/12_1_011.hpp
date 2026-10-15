#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace interval_dp
{
struct Result
{
    __int128 cost = 0;
    int start = 0;
    vector<vector<int>> cut;
    // 后序返回每次合并的 [l,m)、[m,r)，叶区间不返回。
    vector<array<int, 3>> plan(int l, int r) const
    {
        vector<array<int, 3>> out;
        auto dfs = [&](auto &&self, int x, int y) -> void
        {
            if (y - x <= 1) return;
            int m = cut[x][y]; self(self, x, m); self(self, m, y);
            out.push_back({x, m, y});
        };
        dfs(dfs, l, r); return out;
    }
};
// 环形用双倍数组，扫描长度 n 的窗口；允许负代价，不能直接套 Knuth。
inline Result stones(vector<long long> a, bool ring = false)
{
    int n = a.size(); if (!n) return {};
    if (ring) { auto b = a; a.insert(a.end(), b.begin(), b.end()); }
    int m = a.size(); vector<__int128> pre(m + 1);
    for (int i = 0; i < m; ++i) pre[i + 1] = pre[i] + a[i];
    vector<vector<__int128>> dp(m, vector<__int128>(m + 1));
    Result out; out.cut.assign(m, vector<int>(m + 1, -1));
    for (int len = 2; len <= n; ++len) for (int l = 0; l + len <= m; ++l)
    {
        int r = l + len;
        for (int k = l + 1; k < r; ++k)
        {
            __int128 v = dp[l][k] + dp[k][r] + pre[r] - pre[l];
            if (k == l + 1 || v < dp[l][r]) dp[l][r] = v, out.cut[l][r] = k;
        }
    }
    out.cost = dp[0][n];
    if (ring) for (int l = 1; l < n; ++l)
        if (dp[l][l + n] < out.cost) out.cost = dp[l][l + n], out.start = l;
    return out;
}
// p 长度 n+1：第 i 个矩阵为 p[i]*p[i+1]；乘积和必须能装入 __int128。
inline Result matrixChain(const vector<long long> &p)
{
    if (p.empty()) throw invalid_argument("matrixChain: dimensions required");
    for (auto x : p) if (x <= 0) throw invalid_argument("positive dimensions required");
    int n = p.size() - 1; Result out;
    out.cut.assign(n, vector<int>(n + 1, -1));
    vector<vector<__int128>> dp(n, vector<__int128>(n + 1));
    for (int len = 2; len <= n; ++len) for (int l = 0; l + len <= n; ++l)
    {
        int r = l + len;
        for (int k = l + 1; k < r; ++k)
        {
            __int128 v = dp[l][k] + dp[k][r] + (__int128)p[l] * p[k] * p[r];
            if (k == l + 1 || v < dp[l][r]) dp[l][r] = v, out.cut[l][r] = k;
        }
    }
    if (n) out.cost = dp[0][n]; return out;
}
// 仅含 ()：最少插入字符使括号串合法。
inline int brackets(const string &s)
{
    int n = s.size(); vector<vector<int>> dp(n + 1, vector<int>(n + 1));
    for (int i = 0; i < n; ++i)
    {
        if (s[i] != '(' && s[i] != ')') throw invalid_argument("only parentheses supported");
        dp[i][i + 1] = 1;
    }
    for (int len = 2; len <= n; ++len) for (int l = 0; l + len <= n; ++l)
    {
        int r = l + len; dp[l][r] = len;
        if (s[l] == '(' && s[r - 1] == ')') dp[l][r] = dp[l + 1][r - 1];
        for (int k = l + 1; k < r; ++k) dp[l][r] = min(dp[l][r], dp[l][k] + dp[k][r]);
    }
    return dp[0][n];
}
}
