#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

// RerootEdge[0] 是邻点，RerootEdge[1] 是无向边编号。
using RerootEdge = array<int, 2>;

template <class T, class Merge, class Lift>
vector<T> rerooting(
    const vector<vector<RerootEdge>> &g, const vector<T> &base, const T &identity, Merge merge, Lift lift, int root = 0)
{
    // g 是带边号无向树，base 是各点自身贡献，identity 是 merge 单位元；merge 合并贡献，lift 令贡献跨边。
    int n = g.size();
    assert(n > 0 && (int)base.size() == n && 0 <= root && root < n); // 调试检查，可删
    vector<T> down(n, identity), fromPar(n, identity), ans(n, identity);

    auto dfsDown = [&](auto &&self, int u, int p) -> void
    {
        down[u] = base[u];
        for (auto [v, edgeId] : g[u])
        {
            if (v == p)
            {
                continue;
            }
            self(self, v, u);
            down[u] = merge(down[u], lift(down[v], v, u, edgeId));
        }
    };
    dfsDown(dfsDown, root, -1);

    auto dfsUp = [&](auto &&self, int u, int p) -> void
    {
        int degree = g[u].size();
        vector<T> part(degree, identity), prefix(degree + 1, identity), suffix(degree + 1, identity);
        for (int i = 0; i < degree; i++)
        {
            auto [v, edgeId] = g[u][i];
            part[i] = v == p ? fromPar[u] : lift(down[v], v, u, edgeId);
            prefix[i + 1] = merge(prefix[i], part[i]);
        }
        for (int i = degree - 1; i >= 0; i--)
        {
            suffix[i] = merge(part[i], suffix[i + 1]);
        }
        ans[u] = merge(base[u], prefix[degree]);
        for (int i = 0; i < degree; i++)
        {
            auto [v, edgeId] = g[u][i];
            if (v == p)
            {
                continue;
            }
            T withoutV = merge(base[u], merge(prefix[i], suffix[i + 1]));
            fromPar[v] = lift(withoutV, u, v, edgeId);
            self(self, v, u);
        }
    };
    dfsUp(dfsUp, root, -1);
    return ans;
}
