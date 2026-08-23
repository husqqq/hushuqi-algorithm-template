#include "../include/full/5_1_001.hpp"
#include "../include/full/5_1_002.hpp"
#include "../include/full/5_1_003.hpp"
#include "../include/full/5_1_004.hpp"
#include "../include/full/5_1_005.hpp"
#include "../include/full/5_1_007.hpp"
#include "../include/full/5_5_006.hpp"

__int128 bruteChebyshev(const vector<CP> &p)
{
    int n = p.size();
    if (!n)
    {
        return 0;
    }
    vector<optional<__int128>> d(n);
    vector<bool> use(n);
    d[0] = 0;
    __int128 ans = 0;
    for (int z = 0; z < n; z++)
    {
        int u = -1;
        for (int i = 0; i < n; i++)
        {
            if (!use[i] && d[i].has_value() && (u < 0 || *d[i] < *d[u]))
            {
                u = i;
            }
        }
        use[u] = true;
        ans += *d[u];
        for (int v = 0; v < n; v++)
        {
            if (use[v])
            {
                continue;
            }
            __int128 dx = (__int128)p[u][0] - p[v][0];
            __int128 dy = (__int128)p[u][1] - p[v][1];
            if (dx < 0)
            {
                dx = -dx;
            }
            if (dy < 0)
            {
                dy = -dy;
            }
            __int128 w = max(dx, dy);
            if (!d[v].has_value() || w < *d[v])
            {
                d[v] = w;
            }
        }
    }
    return ans;
}

signed main()
{
    ForwardStar fs(2);
    int id = fs.addResEdge(0, 1, 7, 3);
    assert(id == 0 && (id ^ 1) == 1);
    assert(fs.edges()[id][0] == 1 && fs.edges()[id ^ 1][0] == 0);
    vector<vector<int>> path = {{1}, {0, 2}, {1}};
    assert(bfs(path, 0) == vector<int>({0, 1, 2}));
    assert(dfs(path, 0) == vector<int>({0, 1, 2}));
    assert(topo({}).has_value() && topo({})->empty());
    assert(!topo({{1}, {0}}).has_value());
    assert(bipartite(path).has_value());
    assert(!bipartite({{1, 2}, {0, 2}, {0, 1}}).has_value());
    vector<vector<array<int, 2>>> dag = {{{1, -2}, {2, 1}}, {{2, 5}}, {}};
    auto longest = dagLongest(dag, 0);
    assert(longest.has_value() && *(*longest)[2] == 3);

    auto empty = diffCons(0, {});
    assert(empty.has_value() && empty->empty());
    assert(diffCons(3, {{0, 1, 3}, {1, 2, 4}, {2, 0, -7}}).has_value());
    assert(!diffCons(3, {{0, 1, 3}, {1, 2, 4}, {2, 0, -8}}).has_value());

    auto mst = kruskal(3, {{0, 1, 4}, {1, 2, 7}, {0, 2, 20}});
    assert(mst.has_value() && mst->first == 11);
    assert(chebyshevMst({{0, 0}, {1, 0}, {1, 1}, {0, 1}}) == 3);
    auto ce = chebyMST({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
    assert(ce.weight == 3 && ce.edge.size() == 3);

    mt19937_64 rng(0);
    for (int t = 0; t < 1000; t++)
    {
        int n = rng() % 12;
        vector<CP> p(n);
        for (auto &a : p)
        {
            a = {(int)(rng() % 41) - 20, (int)(rng() % 41) - 20};
        }
        assert(chebyshevMst(p) == bruteChebyshev(p));
    }
    puts("OK");
    return 0;
}
