#include "../include/full/5_9_003.hpp"
#include "../include/full/5_9_004.hpp"
#include "../include/full/5_9_006.hpp"
#include "../include/full/5_9_007.hpp"
#include "../include/full/5_9_008.hpp"

signed main()
{
    vector<pair<int, int>> two = {{0, 1}, {0, 1}, {1, 0}, {1, 0}};
    assert(bestTheorem(2, 0, two, true).val() == 2);
    assert(bestTheorem(2, 0, two).val() == 4);
    assert(bestTheorem(3, 2, {{0, 1}, {1, 0}}).val() == 0);
    assert(bestTheorem(3, 0, {{0, 1}, {1, 0}}).val() == 1);
    assert(bestTheorem(3, 2, {}).val() == 1);
    assert(bestTheorem(2, 0, {{0, 0}, {1, 1}}).val() == 0);

    using O = optional<long long>;
    vector<vector<O>> a = {
        {O{}, 2, 4, O{}},
        {2, O{}, 3, 10},
        {4, 3, O{}, 1},
        {O{}, 10, 1, O{}}};
    assert(minCycle(a).has_value() && *minCycle(a) == 9);
    vector<vector<O>> tree = {
        {O{}, 1, O{}},
        {1, O{}, 2},
        {O{}, 2, O{}}};
    assert(!minCycle(tree).has_value());

    vector<pair<int, int>> k4;
    for (int u = 0; u < 4; u++)
    {
        for (int v = u + 1; v < 4; v++)
        {
            k4.push_back({u, v});
        }
    }
    assert(triangles(4, k4) == 4);
    assert(weightedTri(4, {1, 2, 3, 4}, k4, 998244353) == 50);
    assert(fourCycles(4, k4) == 3);
    assert(fourCycleCnt(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}}) == vector<long long>({1, 1, 1, 1}));
    assert(fourCycleCnt(4, {{0, 1}, {0, 1}, {1, 2}, {2, 3}, {3, 0}}) == vector<long long>({1, 1, 2, 2, 2}));

    assert(dirCycle({{1}, {2}, {}}).empty());
    assert(dirCycle({{0}}) == vector<int>{0});
    auto cyc = dirCycle({{1}, {2}, {0}});
    assert(cyc == vector<int>({0, 1, 2}));
    assert(findDirCycle(3, {{0, 1}, {1, 2}, {2, 0}}) == vector<int>({0, 1, 2}));
    assert(findDirCycle(3, {{0, 1}, {1, 2}}).empty());
    assert(findUndirCyc(2, {{0, 1}, {0, 1}}) == vector<int>({0, 1}));
    vector<pair<int, int>> longCycle;
    for (int i = 0; i < 500000; i++) longCycle.push_back({i, (i + 1) % 500000});
    assert(findUndirCyc(500000, longCycle).size() == 500000);
    vector<unsigned long long> mask(4);
    for (auto [u, v] : k4)
    {
        mask[u] |= 1ULL << v;
        mask[v] |= 1ULL << u;
    }
    assert(simpleCycles(mask) == 7);

    Unicycle uc(5);
    uc.add(0, 1);
    uc.add(1, 2);
    uc.add(2, 0);
    uc.add(2, 3);
    uc.add(3, 4);
    assert(uc.build());
    assert(uc.on[0] && uc.on[1] && uc.on[2]);
    assert(!uc.on[3] && !uc.on[4]);
    assert(uc.root[3] == 2 && uc.root[4] == 2);
    assert(uc.parent[3] == 2 && uc.parent[4] == 3);
    assert(uc.dep[3] == 1 && uc.dep[4] == 2);

    Unicycle bad(3);
    bad.add(0, 1);
    bad.add(1, 2);
    assert(!bad.build());

    puts("OK");
    return 0;
}
