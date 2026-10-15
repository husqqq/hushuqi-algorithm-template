#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

int permanent(const vector<vector<int>> &a, int mod)
{
    // a 是 n<=24 的方阵，mod>0；返回积和式 perm(a) 模 mod 的非负值。
    // Ryser 公式：perm = sum_{S} (-1)^{n-|S|} prod_i sum_{j in S} a[i][j]，按格雷码每次改一列。
    int n = a.size();
    assert(mod > 0 && n <= 24); // 调试检查，可删。
    vector<int> row(n); // row[i] 是当前列集合 S 在第 i 行上的元素和 (模 mod)。
    int ans = 0;
    for (int t = 1; t < (1LL << n); t++)
    {
        int s = t ^ (t >> 1), b = __builtin_ctzll(t), sg = s >> b & 1 ? 1 : -1;
        int prod = 1 % mod;
        for (int i = 0; i < n; i++)
        {
            row[i] = (((__int128)row[i] + (__int128)sg * a[i][b]) % mod + mod) % mod;
            prod = (__int128)prod * row[i] % mod;
        }
        if ((n - __builtin_popcountll(s)) & 1)
        {
            prod = (mod - prod) % mod;
        }
        ans = ((__int128)ans + prod) % mod;
    }
    return n ? ans : 1 % mod;
}
