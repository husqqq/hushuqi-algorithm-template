#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct FMIndex
{
    // 字节编码为 1..256，唯一终止符为 0；保留完整 SA，locate 无采样开销。
    vector<int> sa, bwt;
    array<int, 258> c{};
    array<vector<int>, 257> at;
    explicit FMIndex(const string &text)
    {
        int n = text.size() + 1; vector<int> s(n), rank(n), next(n);
        for (int i = 0; i + 1 < n; ++i) s[i] = (unsigned char)text[i] + 1;
        rank = s; sa.resize(n); iota(sa.begin(), sa.end(), 0);
        for (int k = 1; k < n; k *= 2)
        {
            auto key = [&](int i) { return pair{rank[i], i + k < n ? rank[i + k] : -1LL}; };
            sort(sa.begin(), sa.end(), [&](int x, int y) { return key(x) < key(y); });
            next[sa[0]] = 0;
            for (int i = 1; i < n; ++i) next[sa[i]] = next[sa[i - 1]] + (key(sa[i]) != key(sa[i - 1]));
            rank.swap(next); if (rank[sa.back()] == n - 1) break;
        }
        bwt.resize(n);
        for (int i = 0; i < n; ++i) { bwt[i] = s[(sa[i] + n - 1) % n]; at[bwt[i]].push_back(i); ++c[bwt[i] + 1]; }
        for (int x = 1; x < 258; ++x) c[x] += c[x - 1];
    }
    int rank(int symbol, int end) const { return lower_bound(at[symbol].begin(), at[symbol].end(), end) - at[symbol].begin(); }
    int lf(int row) const { return c[bwt[row]] + rank(bwt[row], row); }
    pair<int, int> search(const string &pattern) const
    {
        int l = 0, r = sa.size();
        for (auto it = pattern.rbegin(); it != pattern.rend(); ++it)
        {
            int x = (unsigned char)*it + 1;
            l = c[x] + rank(x, l); r = c[x] + rank(x, r);
            if (l == r) break;
        }
        return {l, r};
    }
    vector<int> locate(const string &pattern) const
    {
        auto [l, r] = search(pattern); vector<int> ans(sa.begin() + l, sa.begin() + r);
        sort(ans.begin(), ans.end()); return ans;
    }
    string restore() const
    {
        int row = at[0][0], n = bwt.size(); string s(n - 1, '\0');
        for (int i = n - 2; i >= 0; --i) { row = lf(row); s[i] = (char)(bwt[row] - 1); }
        return s;
    }
};
