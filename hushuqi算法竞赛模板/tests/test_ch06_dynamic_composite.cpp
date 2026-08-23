#include "../include/full/6_4_001_composite.hpp"

signed main()
{
    constexpr long long P = 998244353;
    using Func = DynPathComp<P>::Func;
    vector<Func> f{{2, 1}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}};
    DynPathComp<P> tr(f);
    vector<set<int>> g(6);
    auto link = [&](int u, int v) { assert(tr.link(u, v)); g[u].insert(v); g[v].insert(u); };
    auto cut = [&](int u, int v) { assert(tr.cut(u, v)); g[u].erase(v); g[v].erase(u); };
    link(0, 1); link(1, 2); link(1, 3); link(3, 4); link(3, 5);
    auto brute = [&](int s, int t, long long x)
    {
        vector<int> p(6, -1), q{s};
        for (int i = 0; i < (int)q.size(); i++) for (int v : g[q[i]]) if (v != p[q[i]]) p[v] = q[i], q.push_back(v);
        vector<int> a;
        for (int u = t; u != s; u = p[u]) a.push_back(u);
        a.push_back(s);
        reverse(a.begin(), a.end());
        for (int u : a) x = (f[u].a * x + f[u].b) % P;
        return x;
    };
    for (int u = 0; u < 6; u++) for (int v = 0; v < 6; v++) assert(tr.apply(u, v, 17) == brute(u, v, 17));
    cut(1, 3); link(2, 4);
    f[3] = {17, 19}; tr.set(3, f[3]);
    for (int u = 0; u < 6; u++) for (int v = 0; v < 6; v++) assert(tr.apply(u, v, 23) == brute(u, v, 23));
}
