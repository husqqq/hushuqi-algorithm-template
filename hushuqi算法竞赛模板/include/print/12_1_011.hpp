#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace interval_dp
{
struct Result
{
    __int128 cost = 0;       // cost 是最优总代价。
    int start = 0;           // start 是环形时最优的断环起点，链形为 0。
    vector<vector<int>> cut; // cut[l][r] 是区间 [l,r) 最优的分割点，长度为 1 的区间为 -1。

    vector<array<int, 3>> plan(int l, int r) const
    {
        // 按后序返回区间 [l,r) 的所有合并 {x,m,y}，表示把 [x,m) 与 [m,y) 合并。
        vector<array<int, 3>> res;
        auto dfs = [&](auto &&self, int x, int y) -> void
        {
            if (y - x <= 1)
            {
                return;
            }
            int m = cut[x][y];
            self(self, x, m);
            self(self, m, y);
            res.push_back({x, m, y});
        };
        dfs(dfs, l, r);
        return res;
    }
};

Result stones(vector<int> a, bool ring = false)
{
    // a 是各堆石子数（可为负），ring 表示首尾相邻；每次合并相邻两堆、代价为两堆之和，
    // 返回最小总代价与方案。环形把数组复制一倍后取所有长度为 n 的窗口。O(n^3)。
    int n = a.size();
    if (!n)
    {
        return {};
    }
    if (ring)
    {
        auto b = a;
        a.insert(a.end(), b.begin(), b.end());
    }
    int m = a.size();
    vector<__int128> pre(m + 1);
    for (int i = 0; i < m; i++)
    {
        pre[i + 1] = pre[i] + a[i];
    }
    vector<vector<__int128>> dp(m, vector<__int128>(m + 1)); // dp[l][r] 是合并 [l,r) 的最小代价。
    Result res;
    res.cut.assign(m, vector<int>(m + 1, -1));
    for (int len = 2; len <= n; len++)
    {
        for (int l = 0; l + len <= m; l++)
        {
            int r = l + len;
            for (int k = l + 1; k < r; k++)
            {
                __int128 v = dp[l][k] + dp[k][r] + pre[r] - pre[l];
                // 求最大总代价：把这里的 v < dp[l][r] 和下面环形窗口的 dp[l][l + n] < res.cost 都改成 >。
                if (k == l + 1 || v < dp[l][r])
                {
                    dp[l][r] = v;
                    res.cut[l][r] = k;
                }
            }
        }
    }
    res.cost = dp[0][n];
    for (int l = 1; ring && l < n; l++)
    {
        if (dp[l][l + n] < res.cost)
        {
            res.cost = dp[l][l + n];
            res.start = l;
        }
    }
    return res;
}

Result matrixChain(const vector<int> &p)
{
    // p 有 n+1 个正整数，第 i 个矩阵是 p[i] x p[i+1]；返回最少标量乘法次数与加括号方案。O(n^3)。
    assert(!p.empty()); // 调试检查，可删。
    int n = p.size() - 1;
    Result res;
    res.cut.assign(n, vector<int>(n + 1, -1));
    vector<vector<__int128>> dp(n, vector<__int128>(n + 1));
    for (int len = 2; len <= n; len++)
    {
        for (int l = 0; l + len <= n; l++)
        {
            int r = l + len;
            for (int k = l + 1; k < r; k++)
            {
                __int128 v = dp[l][k] + dp[k][r] + (__int128)p[l] * p[k] * p[r];
                if (k == l + 1 || v < dp[l][r])
                {
                    dp[l][r] = v;
                    res.cut[l][r] = k;
                }
            }
        }
    }
    if (n)
    {
        res.cost = dp[0][n];
    }
    return res;
}

int brackets(const string &s)
{
    // s 只含 ()[]{}；返回最少插入多少个括号能使其合法。O(n^3)。
    // 只有一种括号时用贪心即可：未匹配的右括号数 + 剩余左括号数，O(n)。
    int n = s.size();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1)); // dp[l][r] 是区间 [l,r) 的答案。
    for (int i = 0; i < n; i++)
    {
        dp[i][i + 1] = 1;
    }
    for (int len = 2; len <= n; len++)
    {
        for (int l = 0; l + len <= n; l++)
        {
            int r = l + len;
            dp[l][r] = len;
            int i = string("()[]{}").find(s[l]); // 左括号在偶数位，其右括号紧随其后；找不到为 -1。
            if (i >= 0 && i % 2 == 0 && "()[]{}"[i + 1] == s[r - 1])
            {
                dp[l][r] = dp[l + 1][r - 1];
            }
            for (int k = l + 1; k < r; k++)
            {
                dp[l][r] = min(dp[l][r], dp[l][k] + dp[k][r]);
            }
        }
    }
    return dp[0][n];
}
}
