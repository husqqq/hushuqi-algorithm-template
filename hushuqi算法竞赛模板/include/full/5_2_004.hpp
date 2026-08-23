#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class Dist = long long>
vector<vector<optional<Dist>>> floyd(vector<vector<optional<Dist>>> d)
{
    // d 是初始距离矩阵，空值表示没有路径；返回全点对最短距离。
    int n = d.size();
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            if (!d[i][k].has_value())
            {
                continue;
            }
            for (int j = 0; j < n; j++)
            {
                if (d[k][j].has_value())
                {
                    Dist nd = *d[i][k] + *d[k][j];
                    if (!d[i][j].has_value() || nd < *d[i][j])
                    {
                        d[i][j] = nd;
                    }
                }
            }
        }
    }
    return d;
}

template <class Dist = long long>
optional<Dist> minCycle(const vector<vector<optional<Dist>>> &a)
{
    // a 是非负权无向简单图矩阵；返回最小环权，无环时返回空。
    int n = a.size();
    auto d = a;
    optional<Dist> ans;
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < k; i++)
        {
            for (int j = i + 1; j < k; j++)
            {
                if (d[i][j].has_value() && a[i][k].has_value() && a[k][j].has_value())
                {
                    Dist cur = *d[i][j] + *a[i][k] + *a[k][j];
                    if (!ans.has_value() || cur < *ans)
                    {
                        ans = cur;
                    }
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (d[i][k].has_value() && d[k][j].has_value())
                {
                    Dist nd = *d[i][k] + *d[k][j];
                    if (!d[i][j].has_value() || nd < *d[i][j])
                    {
                        d[i][j] = nd;
                    }
                }
            }
        }
    }
    return ans;
}
