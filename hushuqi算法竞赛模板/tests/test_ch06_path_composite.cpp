#include "../include/full/6_3_004.hpp"

signed main()
{
    constexpr long long P = 998244353;
    using Func = TreePathComp<P>::Func;
    vector<vector<int>> g(7);
    for (auto [u, v] : vector<pair<int, int>>{{0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {5, 6}})
    {
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<Func> f{{2, 1}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}};
    TreePathComp<P> tr(g, f);
    auto brute = [&](int s, int t, long long x)
    {
        vector<int> p(7, -1), q{s};
        for (int i = 0; i < (int)q.size(); i++)
        {
            for (int v : g[q[i]]) if (v != p[q[i]]) p[v] = q[i], q.push_back(v);
        }
        vector<int> path;
        for (int u = t; u != -1; u = p[u])
        {
            path.push_back(u);
            if (u == s) break;
        }
        if (path.back() != s)
        {
            vector<int> a, b;
            for (int u = s; u != -1; u = p[u]) a.push_back(u);
            for (int u = t; u != -1; u = p[u]) b.push_back(u);
            while (a.back() == b.back()) a.pop_back(), b.pop_back();
            path = a;
            path.push_back(p[a.back()]);
            reverse(b.begin(), b.end());
            path.insert(path.end(), b.begin(), b.end());
        }
        else reverse(path.begin(), path.end());
        for (int u : path) x = (f[u].a * x + f[u].b) % P;
        return x;
    };
    for (int u = 0; u < 7; u++) for (int v = 0; v < 7; v++) assert(tr.apply(u, v, 17) == brute(u, v, 17));
    f[1] = {17, 19};
    tr.set(1, f[1]);
    for (int u = 0; u < 7; u++) for (int v = 0; v < 7; v++) assert(tr.apply(u, v, 23) == brute(u, v, 23));
}
