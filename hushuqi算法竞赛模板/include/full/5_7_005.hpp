#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Marriage
{
    // a[x]、b[y] 分别是左侧 x 与右侧 y 的配偶编号。
    vector<int> a, b;
};

Marriage stableMatch(const vector<vector<int>> &pa, const vector<vector<int>> &pb)
{
    // pa、pb 是左右两侧从优到劣的完整偏好表；返回两侧互逆的稳定配偶数组。
    int n = pa.size();
    assert((int)pb.size() == n); // 调试检查，可删。
    vector<vector<int>> rk(n, vector<int>(n));
    for (int y = 0; y < n; y++)
    {
        for (int i = 0; i < n; i++)
        {
            rk[y][pb[y][i]] = i;
        }
    }
    vector<int> a(n, -1), b(n, -1), at(n);
    queue<int> q;
    for (int x = 0; x < n; x++)
    {
        q.push(x);
    }
    while (!q.empty())
    {
        int x = q.front();
        q.pop();
        int y = pa[x][at[x]++];
        if (b[y] < 0)
        {
            a[x] = y;
            b[y] = x;
        }
        else if (rk[y][x] < rk[y][b[y]])
        {
            q.push(b[y]);
            a[b[y]] = -1;
            a[x] = y;
            b[y] = x;
        }
        else
        {
            q.push(x);
        }
    }
    return {a, b};
}
