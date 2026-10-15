#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace edit_distance
{
inline int dp(const string &a, const string &b)
{
    vector<int> row(b.size() + 1); iota(row.begin(), row.end(), 0);
    for (int i = 0; i < (int)a.size(); ++i)
    {
        int prev = row[0]; row[0] = i + 1;
        for (int j = 0; j < (int)b.size(); ++j) { int old = row[j + 1]; row[j + 1] = min({row[j + 1] + 1, row[j] + 1, prev + (a[i] != b[j])}); prev = old; }
    }
    return row.back();
}
// 单字 Myers，模式长度 <=64；全局编辑距离，字节而非 Unicode 字符。
inline int myers(const string &pattern, const string &text)
{
    int n = pattern.size(); if (!n) return text.size(); if (n > 64) throw invalid_argument("Myers pattern length<=64");
    array<unsigned long long, 256> eq{};
    for (int i = 0; i < n; ++i) eq[(unsigned char)pattern[i]] |= 1ULL << i;
    unsigned long long vp = ~0ULL, vn = 0, high = 1ULL << (n - 1); int score = n;
    for (unsigned char c : text)
    {
        auto x = eq[c] | vn, d = (((eq[c] & vp) + vp) ^ vp) | eq[c] | vn;
        auto hp = vn | ~(d | vp), hn = vp & d;
        if (hp & high) ++score; if (hn & high) --score;
        hp = (hp << 1) | 1; hn <<= 1; vp = hn | ~(x | hp); vn = d & hp;
    }
    return score;
}
}
