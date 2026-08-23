#include "../include/full/6_4_002_subtree_add.hpp"

signed main()
{
    mt19937 rng(20260822);
    for (int n = 2; n <= 35; n++)
    {
        for (int round = 0; round < 60; round++)
        {
            vector<long long> value(n);
            for (auto &x : value) x = rng() % 100;
            vector<pair<int, int>> edges;
            vector<vector<int>> g(n);
            for (int v = 1; v < n; v++)
            {
                int p = rng() % v;
                edges.push_back({v, p});
                g[v].push_back(p), g[p].push_back(v);
            }
            DynSubExact<long long> tree(value);
            for (auto [u, v] : edges) tree.link(u, v);
            for (int step = 0; step < 120; step++)
            {
                if ((rng() % 3) == 0)
                {
                    int i = rng() % (n - 1);
                    auto [u, v] = edges[i];
                    tree.cut(u, v);
                    vector<vector<int>> ng(n);
                    for (int j = 0; j < n - 1; j++) if (j != i)
                    {
                        ng[edges[j].first].push_back(edges[j].second);
                        ng[edges[j].second].push_back(edges[j].first);
                    }
                    vector<char> side(n);
                    vector<int> component{0}; side[0] = true;
                    for (int z = 0; z < (int)component.size(); z++) for (int y : ng[component[z]]) if (!side[y]) side[y] = true, component.push_back(y);
                    vector<int> other;
                    for (int u = 0; u < n; u++) if (!side[u]) other.push_back(u);
                    int w = component[rng() % component.size()];
                    int x = other[rng() % other.size()];
                    edges[i] = {w, x};
                    tree.link(w, x);
                    g = ng;
                    g[w].push_back(x), g[x].push_back(w);
                }
                else
                {
                    int v = rng() % n;
                    int p = g[v][rng() % g[v].size()];
                    vector<char> take(n);
                    vector<pair<int, int>> stack{{v, p}};
                    while (!stack.empty())
                    {
                        auto [u, parent] = stack.back(); stack.pop_back();
                        take[u] = true;
                        for (int y : g[u]) if (y != parent) stack.push_back({y, u});
                    }
                    tree.cut(v, p);
                    if (rng() & 1)
                    {
                        long long delta = rng() % 20;
                        tree.add(v, delta);
                        for (int u = 0; u < n; u++) if (take[u]) value[u] += delta;
                    }
                    else
                    {
                        long long expected = 0;
                        for (int u = 0; u < n; u++) if (take[u]) expected += value[u];
                        assert(tree.componentSum(v) == expected);
                    }
                    tree.link(v, p);
                }
            }
        }
    }
}
