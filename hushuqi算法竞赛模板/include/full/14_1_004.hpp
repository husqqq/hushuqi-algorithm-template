#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

array<int, 6> pokerScore(vector<int> r, vector<int> suit)
{
    // r、suit 对应五张不同实体牌；点数 2..14，花色 0..3；返回越大越强的六元评分。
    assert(r.size() == 5 && suit.size() == 5);
    array<array<bool, 4>, 15> seen{}; // seen[点数][花色] 标记实体牌是否已出现。
    array<int, 15> cnt{};             // cnt[x] 是点数 x 的张数，仅使用 2..14。
    for (int i = 0; i < 5; i++)
    {
        assert(2 <= r[i] && r[i] <= 14 && 0 <= suit[i] && suit[i] < 4);
        assert(!seen[r[i]][suit[i]]);
        seen[r[i]][suit[i]] = true;
        cnt[r[i]]++;
    }
    sort(r.begin(), r.end(), greater<>());
    bool flush = count(suit.begin(), suit.end(), suit[0]) == 5;
    int high = 0; // 顺子最高点数；A-2-3-4-5 记为 5，无顺子为 0。
    if (r == vector<int>{14, 5, 4, 3, 2}) high = 5;
    else if (r[0] - r[4] == 4 && adjacent_find(r.begin(), r.end()) == r.end()) high = r[0];
    if (high) return {flush ? 8 : 4, high, 0, 0, 0, 0};

    vector<array<int, 2>> g; // 每项 [0] 张数、[1] 点数，按二者降序排列。
    for (int x = 2; x <= 14; x++)
    {
        if (cnt[x]) g.push_back({cnt[x], x});
    }
    sort(g.begin(), g.end(), greater<>());
    array<int, 6> z{}; // [0] 牌型，后五维是依次比较的点数，未使用维为 0。
    if (g[0][0] == 4) z[0] = 7;
    else if (g[0][0] == 3 && g[1][0] == 2) z[0] = 6;
    else if (flush) z[0] = 5;
    else if (g[0][0] == 3) z[0] = 3;
    else if (g[0][0] == 2 && g[1][0] == 2) z[0] = 2;
    else if (g[0][0] == 2) z[0] = 1;
    for (int i = 0; i < (int)g.size(); i++)
    {
        z[i + 1] = g[i][1];
    }
    return z;
}

array<int, 6> pokerBest(const vector<int> &r, const vector<int> &suit)
{
    // r、suit 对应 n 张不同实体牌，5<=n<=52；返回任选五张的最大 pokerScore，花色不决胜。
    int n = (int)r.size();
    assert(5 <= n && n <= 52 && suit.size() == r.size());
    array<array<bool, 4>, 15> seen{}; // seen[点数][花色] 标记实体牌是否已出现。
    for (int i = 0; i < n; i++)
    {
        assert(2 <= r[i] && r[i] <= 14 && 0 <= suit[i] && suit[i] < 4);
        assert(!seen[r[i]][suit[i]]);
        seen[r[i]][suit[i]] = true;
    }
    vector<int> a(5), b(5); // 当前选五张的点数和花色。
    array<int, 6> ans{};    // 已枚举子集的最佳六元评分。
    auto dfs = [&](auto &&self, int pos, int k) -> void
    {
        // self 是递归自身；pos 是下一个可选下标，k 是已选张数；枚举五张子集并更新 ans。
        if (k == 5)
        {
            ans = max(ans, pokerScore(a, b));
            return;
        }
        for (int i = pos; i <= n - (5 - k); i++)
        {
            a[k] = r[i];
            b[k] = suit[i];
            self(self, i + 1, k + 1);
        }
    };
    dfs(dfs, 0, 0);
    return ans;
}
