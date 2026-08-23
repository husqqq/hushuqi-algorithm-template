#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<int> bitsetSCC(const vector<vector<int>> &g)
{
    // g 是小规模稠密有向图；返回 SCC 编号。
    int n = g.size(), w = (n + 63) / 64;
    vector reach(n, vector<unsigned long long>(w));
    for (int u = 0; u < n; u++)
    {
        reach[u][u / 64] |= 1ULL << (u % 64);
        for (int v : g[u])
        {
            reach[u][v / 64] |= 1ULL << (v % 64);
        }
    }
    for (int k = 0; k < n; k++)
    {
        for (int u = 0; u < n; u++)
        {
            if (!(reach[u][k / 64] >> (k % 64) & 1))
            {
                continue;
            }
            for (int b = 0; b < w; b++)
            {
                reach[u][b] |= reach[k][b];
            }
        }
    }
    vector<int> bel(n, -1);
    int cc = 0;
    for (int u = 0; u < n; u++)
    {
        if (bel[u] != -1)
        {
            continue;
        }
        for (int v = u; v < n; v++)
        {
            bool uv = reach[u][v / 64] >> (v % 64) & 1;
            bool vu = reach[v][u / 64] >> (u % 64) & 1;
            if (uv && vu)
            {
                bel[v] = cc;
            }
        }
        cc++;
    }
    return bel;
}
