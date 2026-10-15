#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

// 浮点 Gram-Schmidt 的实用 LLL；不是严格有理数认证版本。
inline optional<vector<vector<long long>>> lll(vector<vector<long long>> b,
                                               long double delta = 0.75L,
                                               int maxSteps = 100000)
{
    int n = b.size(); if (!n) return b; int m = b[0].size();
    if (delta <= 0.25L || delta >= 1 || maxSteps < 0) throw invalid_argument("invalid LLL parameters");
    for (const auto &row : b) if ((int)row.size() != m) throw invalid_argument("rectangular basis required");
    vector<vector<long double>> star(n, vector<long double>(m)), mu(n, vector<long double>(n));
    vector<long double> norm(n);
    auto gs = [&]() -> bool
    {
        for (int i = 0; i < n; ++i)
        {
            for (int t = 0; t < m; ++t) star[i][t] = b[i][t];
            for (int j = 0; j < i; ++j)
            {
                long double dot = 0; for (int t = 0; t < m; ++t) dot += (long double)b[i][t] * star[j][t];
                mu[i][j] = dot / norm[j];
                for (int t = 0; t < m; ++t) star[i][t] -= mu[i][j] * star[j][t];
            }
            norm[i] = 0; for (auto x : star[i]) norm[i] += x * x;
            if (!isfinite(norm[i]) || norm[i] <= 0) return false;
        }
        return true;
    };
    if (!gs()) return nullopt;
    int k = 1, steps = 0;
    while (k < n)
    {
        if (++steps > maxSteps) return nullopt;
        for (int j = k - 1; j >= 0; --j)
        {
            long double v = roundl(mu[k][j]);
            if (!isfinite(v) || v < -ldexpl(1.0L, 63) || v >= ldexpl(1.0L, 63)) return nullopt;
            long long q = (long long)v; if (!q) continue;
            for (int t = 0; t < m; ++t)
            {
                __int128 x = (__int128)b[k][t] - (__int128)q * b[j][t];
                if (x < LLONG_MIN || x > LLONG_MAX) return nullopt; b[k][t] = x;
            }
            if (!gs()) return nullopt;
        }
        if (norm[k] >= (delta - mu[k][k - 1] * mu[k][k - 1]) * norm[k - 1]) ++k;
        else { swap(b[k], b[k - 1]); if (!gs()) return nullopt; k = max(1LL, k - 1); }
    }
    return b;
}
