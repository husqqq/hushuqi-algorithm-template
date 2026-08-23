#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct EulerTrail
{
    // 依次经过的点与原边编号。
    vector<int> v, id;
};

optional<EulerTrail> dirEuler(int n, const vector<pair<int, int>> &edges, bool lex = false)
{
    // n 是点数，edges 是有向边，lex 控制是否取最小点序列；无欧拉迹时返回 nullopt。
    if (!n)
    {
        return edges.empty() ? optional<EulerTrail>(EulerTrail{}) : nullopt;
    }
    int m = edges.size(), s = m ? edges[0].first : 0, plus = 0, minus = 0;
    vector<int> in(n), out(n);
    vector<vector<pair<int, int>>> g(n);
    for (int i = 0; i < m; i++)
    {
        auto [u, v] = edges[i];
        g[u].push_back({v, i});
        out[u]++;
        in[v]++;
    }
    for (int u = 0; u < n; u++)
    {
        if (out[u] - in[u] == 1)
        {
            s = u;
            plus++;
        }
        else if (in[u] - out[u] == 1)
        {
            minus++;
        }
        else if (in[u] != out[u])
        {
            return nullopt;
        }
    }
    if (!((plus == 1 && minus == 1) || (!plus && !minus)))
    {
        return nullopt;
    }
    if (!plus)
    {
        for (int u = 0; u < n; u++)
        {
            if (out[u])
            {
                s = u;
                break;
            }
        }
    }
    if (lex)
    {
        for (auto &a : g)
        {
            sort(a.begin(), a.end());
        }
    }
    vector<int> it(n), rv, re, st{s}, enter{-1};
    while (!st.empty())
    {
        int u = st.back();
        if (it[u] < (int)g[u].size())
        {
            auto [v, id] = g[u][it[u]++];
            st.push_back(v);
            enter.push_back(id);
        }
        else
        {
            rv.push_back(u);
            if (enter.back() != -1) re.push_back(enter.back());
            st.pop_back();
            enter.pop_back();
        }
    }
    if ((int)rv.size() != m + 1)
    {
        return nullopt;
    }
    reverse(rv.begin(), rv.end());
    reverse(re.begin(), re.end());
    return EulerTrail{rv, re};
}

optional<EulerTrail> undirEuler(int n, const vector<pair<int, int>> &edges, bool lex = false)
{
    // n 是点数，edges 是无向边，lex 控制是否取最小点序列；无欧拉迹时返回 nullopt。
    if (!n)
    {
        return edges.empty() ? optional<EulerTrail>(EulerTrail{}) : nullopt;
    }
    int m = edges.size(), s = 0;
    vector<vector<pair<int, int>>> g(n);
    vector<int> deg(n);
    for (int i = 0; i < m; i++)
    {
        auto [u, v] = edges[i];
        g[u].push_back({v, i});
        g[v].push_back({u, i});
        deg[u]++;
        deg[v]++;
    }
    vector<int> odd;
    for (int u = 0; u < n; u++)
    {
        if (deg[u] & 1)
        {
            odd.push_back(u);
        }
    }
    if (odd.size() != 0 && odd.size() != 2)
    {
        return nullopt;
    }
    if (odd.size() == 2)
    {
        s = odd[0];
    }
    else
    {
        for (int u = 0; u < n; u++)
        {
            if (deg[u])
            {
                s = u;
                break;
            }
        }
    }
    if (lex)
    {
        for (auto &a : g)
        {
            sort(a.begin(), a.end());
        }
    }
    vector<int> it(n), rv, re, st{s}, enter{-1};
    vector<bool> used(m);
    while (!st.empty())
    {
        int u = st.back();
        while (it[u] < (int)g[u].size() && used[g[u][it[u]].second])
        {
            it[u]++;
        }
        if (it[u] < (int)g[u].size())
        {
            auto [v, id] = g[u][it[u]++];
            used[id] = true;
            st.push_back(v);
            enter.push_back(id);
        }
        else
        {
            rv.push_back(u);
            if (enter.back() != -1) re.push_back(enter.back());
            st.pop_back();
            enter.pop_back();
        }
    }
    if ((int)rv.size() != m + 1)
    {
        return nullopt;
    }
    reverse(rv.begin(), rv.end());
    reverse(re.begin(), re.end());
    return EulerTrail{rv, re};
}
