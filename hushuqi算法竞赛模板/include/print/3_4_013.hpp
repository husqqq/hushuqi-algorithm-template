#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

// Cartesian tree -> ±1 Euler RMQ；微块按 ±1 模式共享，块间 ST 仅 O(n) 空间。
struct FischerHeun
{
    vector<long long> a;
    vector<int> first, euler, dep, sig;
    vector<vector<int>> table;
    map<int, vector<int>> micro;
    int b = 1;
    explicit FischerHeun(vector<long long> values) : a(move(values)), first(a.size(), -1)
    {
        int n = a.size(); if (!n) return;
        vector<int> left(n, -1), right(n, -1), st;
        for (int i = 0; i < n; ++i)
        {
            int last = -1; while (!st.empty() && a[i] < a[st.back()]) last = st.back(), st.pop_back();
            if (!st.empty()) right[st.back()] = i; left[i] = last; st.push_back(i);
        }
        struct Frame { int v, d, stage; }; vector<Frame> dfs{{st[0], 0, 0}};
        auto emit = [&](int v, int d) { if (first[v] < 0) first[v] = euler.size(); euler.push_back(v); dep.push_back(d); };
        while (!dfs.empty())
        {
            auto &f = dfs.back();
            if (!f.stage) { emit(f.v, f.d); f.stage = 1; if (left[f.v] >= 0) { dfs.push_back({left[f.v], f.d + 1, 0}); continue; } }
            if (f.stage == 1) { if (left[f.v] >= 0) emit(f.v, f.d); f.stage = 2; if (right[f.v] >= 0) { dfs.push_back({right[f.v], f.d + 1, 0}); continue; } }
            if (right[f.v] >= 0) emit(f.v, f.d); dfs.pop_back();
        }
        int m = dep.size(); b = max(1LL, (int)bit_width((unsigned long long)m) / 2);
        int blocks = (m + b - 1) / b; sig.resize(blocks); vector<int> mins(blocks);
        for (int k = 0; k < blocks; ++k)
        {
            int start = k * b, end = min(m, start + b), code = 0; mins[k] = start;
            for (int i = start + 1; i < end; ++i) { if (dep[i] > dep[i - 1]) code |= 1LL << (i - start - 1); if (dep[i] < dep[mins[k]]) mins[k] = i; }
            sig[k] = code;
            if (!micro.count(code))
            {
                vector<int> d(b), t(b * b);
                for (int i = 1; i < b; ++i) d[i] = d[i - 1] + ((code >> (i - 1) & 1) ? 1 : -1);
                for (int i = 0; i < b; ++i) { int best = i; for (int j = i; j < b; ++j) { if (d[j] < d[best]) best = j; t[i * b + j] = best; } }
                micro.emplace(code, move(t));
            }
        }
        table.push_back(move(mins));
        for (int k = 1; (1LL << k) <= blocks; ++k)
        {
            table.emplace_back(blocks - (1LL << k) + 1);
            for (int i = 0; i < (int)table[k].size(); ++i) table[k][i] = better(table[k - 1][i], table[k - 1][i + (1LL << (k - 1))]);
        }
        // 查询微块需 O(1)；把模式表映射改为块索引数组，避免查询 map。
        for (int k = 0; k < blocks; ++k) blockTable.push_back(&micro.at(sig[k]));
    }
    vector<const vector<int> *> blockTable;
    FischerHeun(const FischerHeun &) = delete;
    FischerHeun &operator=(const FischerHeun &) = delete;
    int better(int x, int y) const { return dep[x] <= dep[y] ? x : y; }
    int inside(int block, int l, int r) const { return block * b + (*blockTable[block])[l * b + r]; }
    int argmin(int l, int r) const
    {
        if (l < 0 || l >= r || r > (int)a.size()) throw out_of_range("RMQ needs nonempty [l,r)");
        int x = first[l], y = first[r - 1]; if (x > y) swap(x, y);
        int u = x / b, v = y / b;
        if (u == v) return euler[inside(u, x % b, y % b)];
        int ans = better(inside(u, x % b, b - 1), inside(v, 0, y % b));
        if (v > u + 1) { int k = bit_width((unsigned long long)(v - u - 1)) - 1; ans = better(ans, better(table[k][u + 1], table[k][v - (1LL << k)])); }
        return euler[ans];
    }
    long long query(int l, int r) const { return a[argmin(l, r)]; }
};
