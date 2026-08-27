// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 16b 最短路 2


#include <bits/stdc++.h>
using namespace std;
#define int long long

struct DEdge
{
    // x、y 是有向边端点，w 是边权。
    int x, y, w;
};

template <class Dist = long long> struct Bellman
{
    // d 是最短距离，空值表示不可达；cycle 是一组可达负环顶点。
    vector<optional<Dist>> d;
    vector<int> cycle;
};

template <class Dist = long long>
Bellman<Dist> bellmanFord(int n, const vector<DEdge> &e, int s)
{
    // n 是点数，e 是边表，s 是源点；返回最短距离及一组可达负环。
    vector<optional<Dist>> d(n);
    vector<int> p(n, -1);
    d[s] = 0;
    int x = -1;
    for (int z = 0; z < n; z++)
    {
        x = -1;
        for (auto [u, v, w] : e)
        {
            if (!d[u].has_value())
            {
                continue;
            }
            Dist nd = *d[u] + (Dist)w;
            if (!d[v].has_value() || nd < *d[v])
            {
                d[v] = nd;
                p[v] = u;
                x = v;
            }
        }
    }
    vector<int> cycle;
    if (x != -1)
    {
        for (int i = 0; i < n; i++)
        {
            x = p[x];
        }
        int y = x;
        do
        {
            cycle.push_back(y);
            y = p[y];
        } while (y != x);
        reverse(cycle.begin(), cycle.end());
    }
    return {d, cycle};
}
