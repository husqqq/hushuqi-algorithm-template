#include "../include/full/5_2_001.hpp"
#include "../include/full/5_2_003.hpp"
#include "../include/full/5_2_004.hpp"
#include "../include/full/5_2_005.hpp"
#include "../include/full/5_2_006.hpp"
#include "../include/full/5_2_007.hpp"
#include "../include/full/5_2_008.hpp"
#include "../include/full/5_2_009.hpp"
#include "../include/full/5_2_010.hpp"
#include "../include/full/5_2_012.hpp"

signed main()
{
    vector<vector<WEdge>> g = {{{1, LLONG_MAX}}, {{2, LLONG_MAX}}, {}};
    auto sparse = dijkstraSp<__int128>(g, 0);
    assert(*sparse[2] == (__int128)LLONG_MAX * 2);
    vector<vector<optional<int>>> mat(3, vector<optional<int>>(3));
    for (int i = 0; i < 3; i++)
    {
        mat[i][i] = 0;
    }
    mat[0][1] = mat[1][2] = LLONG_MAX;
    auto dense = dijkstraDn<__int128>(mat, 0);
    assert(dense == sparse);

    vector<DEdge> neg = {{0, 1, 2}, {1, 2, -5}};
    auto bf = bellmanFord(3, neg, 0);
    assert(bf.cycle.empty() && *bf.d[2] == -3);
    auto sf = spfa(3, neg, 0);
    assert(sf.has_value() && *(*sf)[2] == -3);
    vector<DEdge> cycle = {{0, 1, 0}, {1, 0, -1}};
    assert(!bellmanFord(2, cycle, 0).cycle.empty());
    assert(!spfa(2, cycle, 0).has_value());

    vector<vector<optional<__int128>>> f(3, vector<optional<__int128>>(3));
    for (int i = 0; i < 3; i++)
    {
        f[i][i] = 0;
    }
    f[0][1] = 2;
    f[1][2] = 3;
    assert(*floyd(f)[0][2] == 5);

    Johnson johnson(3);
    johnson.addEdge(0, 1, 2);
    johnson.addEdge(1, 2, -5);
    auto all = johnson.solve();
    assert(all.has_value() && *(*all)[0][2] == -3);

    IntGraph range(5);
    range.addPointRng(0, 1, 4, 7);
    range.addRngPoint(1, 3, 4, 2);
    range.addEdge(3, 4, 1);
    auto rd = range.dijkstra(0);
    assert(*rd[4] == 8);

    auto dual = dualShortest(3, {{0, 1, 4}, {1, 2, 5}, {0, 2, 12}}, 0, 2);
    assert(dual.has_value() && dual->dist == 9 && dual->path == vector<int>({0, 1, 2}));

    vector<vector<WEdge>> zero = {{{1, 0}}, {{0, 0}, {2, 1}}, {}};
    auto [dist, par] = shortestTree(zero, 0);
    assert(par[1] == 0 && par[2] == 1 && *dist[2] == 1);

    vector<vector<pair<int, int>>> countGraph = {{{1, 1}, {1, 1}, {2, 2}}, {{2, 1}}, {}};
    auto cnt = shortestCnt(countGraph, 0, 1000000007);
    assert(cnt.has_value() && *cnt->d[2][0] == 2 && cnt->cnt[2][0] == 3);
    assert(!shortestCnt({{{0, 0}}}, 0, 7).has_value());

    assert(kthWalk(2, {{0, 0, 1}, {0, 1, 1}}, 0, 1, 3) == optional<__int128>(3));
    vector<EppsteinEdge> walks = {{0, 0, 2}, {0, 1, 1}, {0, 1, 1}};
    assert(eppWalk(2, walks, 0, 1, 1) == optional<int>(1));
    assert(eppWalk(2, walks, 0, 1, 3) == optional<int>(3));
    assert(eppWalk(2, walks, 0, 0, 1) == optional<int>(0));
    assert(eppWalk(2, walks, 0, 0, 1, false) == optional<int>(2));
    assert(!eppWalk(2, vector<EppsteinEdge>{}, 0, 1, 1));
    puts("OK");
    return 0;
}
