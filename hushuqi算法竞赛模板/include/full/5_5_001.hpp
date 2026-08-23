#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct DSU
{
    // f 是并查集父亲，s 仅在根处保存连通块大小。
    vector<int> f, s;

    DSU(int n = 0)
    {
        // n 是元素个数；建立 n 个单点集合。
        init(n);
    }

    void init(int n)
    {
        // n 是元素个数；清空旧状态并建立 n 个单点集合，无返回值。
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        s.assign(n, 1);
    }

    int find(int x)
    {
        // x 是元素编号；返回所在集合的根并压缩路径。
        return x == f[x] ? x : f[x] = find(f[x]);
    }

    bool merge(int x, int y)
    {
        // x、y 是元素编号；合并所在集合，原本不连通时返回 true。
        x = find(x);
        y = find(y);
        if (x == y)
        {
            return false;
        }
        if (s[x] < s[y])
        {
            swap(x, y);
        }
        f[y] = x;
        s[x] += s[y];
        return true;
    }

    bool same(int x, int y)
    {
        // x、y 是元素编号；返回二者是否属于同一集合。
        return find(x) == find(y);
    }

    int size(int x)
    {
        // x 是元素编号；返回所在集合的元素个数。
        return s[find(x)];
    }
};

struct MEdge
{
    // x、y 是无向边端点，w 是边权。
    int x, y, w;
};

optional<pair<int, vector<MEdge>>> kruskal(int n, vector<MEdge> e)
{
    // n 是点数，e 是无向边集；返回 MST 总权与所选边，不连通时返回空。
    sort(e.begin(), e.end(), [](const MEdge &a, const MEdge &b)
         { return a.w < b.w; });
    DSU d(n);
    int ans = 0;
    vector<MEdge> use;
    for (auto v : e)
    {
        if (!d.merge(v.x, v.y))
        {
            continue;
        }
        ans += v.w;
        use.push_back(v);
    }
    if ((int)use.size() + 1 != n && n)
    {
        return nullopt;
    }
    return pair{ans, use};
}
