#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

pair<vector<int>, vector<int>>
minVtxCover(const vector<vector<int>> &g, const vector<int> &ml, const vector<int> &mr)
{
    // g 是左部邻接表，ml、mr 是一份最大匹配；返回最小点覆盖中的左点表与右点表。
    vector<bool> vl(g.size()), vr(mr.size());
    queue<int> q;
    for (int u = 0; u < (int)g.size(); u++)
    {
        if (ml[u] == -1)
        {
            vl[u] = true;
            q.push(u);
        }
    }
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (auto v : g[u])
        {
            if (ml[u] == v || vr[v])
            {
                continue;
            }
            vr[v] = true;
            if (mr[v] != -1 && !vl[mr[v]])
            {
                vl[mr[v]] = true;
                q.push(mr[v]);
            }
        }
    }
    vector<int> l, r;
    for (int u = 0; u < (int)g.size(); u++)
    {
        if (!vl[u])
        {
            l.push_back(u);
        }
    }
    for (int v = 0; v < (int)mr.size(); v++)
    {
        if (vr[v])
        {
            r.push_back(v);
        }
    }
    return {l, r};
}

optional<vector<array<int, 2>>> minEdgeCover(const vector<vector<int>> &g, const vector<int> &ml, const vector<int> &mr)
{
    // g 是左部邻接表，ml、mr 是最大匹配；返回最小边覆盖，存在孤立点时返回空。
    vector<array<int, 2>> ans;
    vector<bool> usedR(mr.size());
    for (int u = 0; u < (int)g.size(); u++)
    {
        if (g[u].empty())
        {
            return nullopt;
        }
        if (ml[u] != -1)
        {
            ans.push_back({u, ml[u]});
            usedR[ml[u]] = true;
        }
    }
    for (int u = 0; u < (int)g.size(); u++)
    {
        if (ml[u] == -1)
        {
            ans.push_back({u, g[u][0]});
            usedR[g[u][0]] = true;
        }
    }
    vector<int> pre(mr.size(), -1);
    for (int u = 0; u < (int)g.size(); u++)
    {
        for (auto v : g[u])
        {
            pre[v] = u;
        }
    }
    for (int v = 0; v < (int)mr.size(); v++)
    {
        if (pre[v] == -1)
        {
            return nullopt;
        }
        if (!usedR[v])
        {
            ans.push_back({pre[v], v});
        }
    }
    return ans;
}
