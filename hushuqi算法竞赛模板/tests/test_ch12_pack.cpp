#include "../include/full/12_1_002.hpp"
#include "../include/full/12_1_003.hpp"
#include "../include/full/12_1_004.hpp"
#include "../include/full/12_1_005.hpp"
#include "../include/full/12_1_006.hpp"
#include "../include/full/12_1_007.hpp"
#include "../include/full/12_1_008.hpp"
#include "../include/full/12_1_009.hpp"
#include "../include/full/12_1_010.hpp"

signed main()
{
    auto a = vector<pair<int, int>>{{2, 3}};
    assert(zeroOnePack(6, a)[6] == -INF);
    assert(completePack(6, a)[6] == 9);

    auto b = vector<array<int, 3>>{{2, 3, numeric_limits<int>::max()}};
    assert(boundedPack(6, b)[6] == 9);
    assert(mixedPack(6, {{2, 3, 0}})[6] == 9);
    assert(knapsack2(4, 5, {{2, 3, 6}, {2, 2, 5}, {3, 2, 7}}) == 11);

    vector<vector<pair<int, int>>> grp{{{2, 3}, {4, 9}}, {{2, 4}}};
    assert(groupPack(4, grp)[4] == 9);

    auto dep = treeDepPack(vector<int>{2, 2, 3}, vector<long long>{3, 5, 8}, vector<int>{-1, 0, 0}, 5);
    int bestDep = numeric_limits<int>::lowest();
    for (auto x : dep)
    {
        if (x)
        {
            bestDep = max(bestDep, *x);
        }
    }
    assert(bestDep == 11);
    vector<__int128> wideVal{3, 5, 8};
    auto wideDep = treeDepPack<__int128>(vector<int>{2, 2, 3}, wideVal, vector<int>{-1, 0, 0}, 5);
    assert(wideDep[5] && *wideDep[5] == 11);

    int mod = numeric_limits<int>::max() - 24;
    vector<pair<int, long long>> z(64, {1, 0});
    auto res = packPlan(64, z, mod);
    int want = (int)(((__int128)1 << 64) % mod);
    assert(res.val == 0 && res.ways == want);

    int n = 1000;
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++)
    {
        g[i - 1].push_back(i);
        g[i].push_back(i - 1);
    }
    vector<array<int, 3>> item(n, {1, 1, 1});
    assert(treePack(g, item, 0, 1)[1] == 1);

    assert(boundSubset({2, 1}, {3, 5}, 11));
    assert(!boundSubset({2, 1}, {3, 5}, 10));
    cout << "OK\n";
}
