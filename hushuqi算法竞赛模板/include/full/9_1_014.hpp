#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace gf2
{
using Row = vector<unsigned long long>;
inline bool bit(const Row &r, int i) { return r[i / 64] >> (i % 64) & 1; }
inline void flip(Row &r, int i) { r[i / 64] ^= 1ULL << (i % 64); }
struct Result { bool consistent; int rank, free; Row solution; vector<Row> basis; };
// 增广行的 0..n-1 为变量、n 为右端；每行长度须为 ceil((n+1)/64)。
inline Result solve(vector<Row> a, int n)
{
    if (n < 0) throw invalid_argument("negative variable count");
    for (auto &r : a) if ((int)r.size() != (n + 64) / 64) throw invalid_argument("row width mismatch");
    int rank = 0, m = a.size(); vector<int> where(n, -1);
    for (int c = 0; c < n; ++c)
    {
        int p = rank; while (p < m && !bit(a[p], c)) ++p; if (p == m) continue;
        swap(a[p], a[rank]); where[c] = rank;
        for (int i = 0; i < m; ++i) if (i != rank && bit(a[i], c))
            for (int j = 0; j < (int)a[i].size(); ++j) a[i][j] ^= a[rank][j];
        ++rank;
    }
    Result out{true, rank, n - rank, Row((n + 63) / 64), {}};
    for (int i = rank; i < m; ++i) if (bit(a[i], n)) { out.consistent = false; return out; }
    for (int c = 0; c < n; ++c) if (where[c] >= 0 && bit(a[where[c]], n)) flip(out.solution, c);
    for (int f = 0; f < n; ++f) if (where[f] < 0)
    {
        Row v((n + 63) / 64); flip(v, f);
        for (int c = 0; c < n; ++c) if (where[c] >= 0 && bit(a[where[c]], f)) flip(v, c);
        out.basis.push_back(move(v));
    }
    return out;
}
}
