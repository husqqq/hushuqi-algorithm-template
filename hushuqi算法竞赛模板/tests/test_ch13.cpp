#include "../include/full/13_1_001.hpp"
#include "../include/full/13_1_002.hpp"
#include "../include/full/13_1_003.hpp"
#include "../include/full/13_1_004.hpp"
#include "../include/full/13_1_005.hpp"
#include "../include/full/13_1_006.hpp"
#include "../include/full/13_1_007.hpp"
#include "../include/full/13_1_008.hpp"
#include "../include/full/13_2_001.hpp"
#include "../include/full/13_2_002.hpp"
#include "../include/full/13_2_003.hpp"
#include "../include/full/13_2_004.hpp"
#include "../include/full/13_3_001.hpp"
#include "../include/full/13_3_002.hpp"
#include "../include/full/13_3_003.hpp"
#include "../include/full/13_3_004.hpp"
#include "../include/full/13_3_006.hpp"

signed main()
{
    assert(bashWin(numeric_limits<long long>::max(), numeric_limits<long long>::max()));
    assert(!bashWin(0, 1));
    assert(!antiNimWin({}));
    assert(!antiNimWin({0, 1}));
    assert(antiNimWin({1, 1}));
    assert(antiNimWin({2, 0}));
    assert(stairNimWin({0, 1}));

    int n = 100000;
    vector<vector<int>> to(n);
    for (int i = 1; i < n; i++)
    {
        to[i].push_back(i - 1);
    }
    auto f = sg(to);
    assert(f[n - 1] == (n - 1) % 2);

    assert(!wythoff_game::firstWins(0, 0));
    assert(!wythoff_game::firstWins(1, 2));
    assert(wythoff_game::firstWins(1, 1));
    assert(wythoff_game::firstWins(numeric_limits<int>::max(),
                                  numeric_limits<int>::max()));

    vector<vector<int>> tree(10000);
    for (int i = 1; i < (int)tree.size(); i++)
    {
        tree[i - 1].push_back(i);
        tree[i].push_back(i - 1);
    }
    assert(edge_del::treeSg(tree) == 9999);
    assert(stone_game::subGameSG(10, {1, 1, 3})[10] == 0);

    auto post = prob_formula::bayes({0.5L, 0.5L}, {1, 0});
    assert(fabsl(post[0] - 1) < 1E-18L && post[1] == 0);
    assert(binomialPmf(1000000000, 500000000, 0.5L) > 0);
    auto mv = exp_var::moments({1E18L, 1E18L + 1}, {0.5L, 0.5L});
    assert(fabsl(mv[0] - (1E18L + 0.5L)) < 0.1L);
    assert(fabsl(mv[1] - 0.25L) < 0.01L);

    RNG rng(1);
    assert(rng(1) == 0);
    assert(rng(100) < 100);

    auto ps = powerSums(5000000000LL);
    assert(ps[0] == (__int128)5000000000LL * 5000000001LL / 2);
    auto tr = triangle(3, 4, 5);
    assert(fabsl(tr[0] - 6) < 1E-18L);
    assert(roundBodies(1, 1, 1)[0] > 3.14L);
    cout << "OK\n";
}
