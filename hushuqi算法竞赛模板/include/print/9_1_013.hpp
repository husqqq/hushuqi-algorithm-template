#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

inline long long permanentRyser(const vector<vector<long long>> &a, long long mod)
{
    int n = a.size();
    if (mod <= 0 || n > 24) throw invalid_argument("permanent: mod>0 and n<=24 required");
    for (const auto &row : a) if ((int)row.size() != n) throw invalid_argument("square matrix required");
    if (!n) return 1 % mod;
    vector<long long> sum(n); long long ans = 0;
    unsigned long long last = 0;
    for (unsigned long long t = 1; t < (1ULL << n); ++t)
    {
        auto mask = t ^ (t >> 1), diff = mask ^ last;
        int b = countr_zero(diff); bool add = mask & diff;
        for (int i = 0; i < n; ++i)
        {
            __int128 v = (__int128)sum[i] + (add ? (__int128)a[i][b] : -(__int128)a[i][b]);
            sum[i] = v % mod; if (sum[i] < 0) sum[i] += mod;
        }
        long long prod = 1 % mod;
        for (auto x : sum) prod = (__int128)prod * x % mod;
        __int128 v = (__int128)ans + ((n - popcount(mask)) & 1 ? -(__int128)prod : prod);
        ans = v % mod; if (ans < 0) ans += mod; last = mask;
    }
    return ans;
}
