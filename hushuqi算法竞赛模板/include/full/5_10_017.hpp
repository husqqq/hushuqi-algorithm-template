#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

inline optional<vector<pair<int, int>>> havelHakimi(const vector<int> &d)
{
    int n = d.size(); priority_queue<pair<int, int>> q; vector<pair<int, int>> e;
    for (int i = 0; i < n; ++i) { if (d[i] < 0 || d[i] >= n) return nullopt; if (d[i]) q.push({d[i], i}); }
    while (!q.empty())
    {
        auto [k, u] = q.top(); q.pop();
        if (k > (int)q.size()) return nullopt;
        vector<pair<int, int>> tmp;
        while (k--) { auto [s, v] = q.top(); q.pop(); e.push_back({u, v}); if (s > 1) tmp.push_back({s - 1, v}); }
        for (auto x : tmp) q.push(x);
    }
    return e;
}
inline bool galeRyser(vector<int> a, vector<int> b)
{
    __int128 sa = 0, sb = 0;
    for (int x : a) { if (x < 0 || x > (int)b.size()) return false; sa += x; }
    for (int x : b) { if (x < 0 || x > (int)a.size()) return false; sb += x; }
    if (sa != sb) return false;
    sort(a.rbegin(), a.rend()); sort(b.begin(), b.end());
    __int128 left = 0, small = 0; int j = 0;
    for (int k = 1; k <= (int)a.size(); ++k)
    {
        left += a[k - 1]; while (j < (int)b.size() && b[j] < k) small += b[j++];
        if (left > small + (__int128)k * (b.size() - j)) return false;
    }
    return true;
}
inline optional<vector<pair<int, int>>> bipDegree(const vector<int> &a, const vector<int> &b)
{
    if (!galeRyser(a, b)) return nullopt;
    vector<pair<int, int>> left, e; priority_queue<pair<int, int>> q;
    for (int i = 0; i < (int)a.size(); ++i) left.push_back({a[i], i});
    for (int j = 0; j < (int)b.size(); ++j) if (b[j]) q.push({b[j], j});
    sort(left.rbegin(), left.rend());
    for (auto [k, u] : left)
    {
        vector<pair<int, int>> tmp;
        if (k > (int)q.size()) return nullopt;
        while (k--) { auto [s, v] = q.top(); q.pop(); e.push_back({u, v}); if (s > 1) tmp.push_back({s - 1, v}); }
        for (auto x : tmp) q.push(x);
    }
    return e;
}
