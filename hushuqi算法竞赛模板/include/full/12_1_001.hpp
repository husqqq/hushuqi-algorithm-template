#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

int addMod(int a, int b, int mod)
{
    return a >= mod - b ? a - (mod - b) : a + b;
}

vector<int> zeroOnePack(int m, const vector<pair<int, int>> &a)
{
    assert(m >= 0);
    vector<int> f(m + 1, -INF);
    f[0] = 0;
    for (auto [w, v] : a)
    {
        assert(w > 0);
        for (int j = m; j >= w; j--)
        {
            if (f[j - w] != -INF)
            {
                chmax(f[j], f[j - w] + v);
            }
        }
    }
    return f;
}
