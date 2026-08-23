#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<vector<int>> blockCutTree(int n, const vector<vector<int>> &bcc)
{
    // n 是原图点数，bcc 是点双点集；返回块割树邻接表。
    vector<vector<int>> tr(n + bcc.size());
    for (int i = 0; i < (int)bcc.size(); i++)
    {
        int b = n + i;
        for (int u : bcc[i])
        {
            tr[u].push_back(b);
            tr[b].push_back(u);
        }
    }
    return tr;
}
