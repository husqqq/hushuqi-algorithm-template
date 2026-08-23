#include "../include/full/12_2_001.hpp"
#include "../include/full/12_2_002.hpp"
#include "../include/full/12_2_003.hpp"
#include "../include/full/12_2_004.hpp"
#include "../include/full/12_2_005.hpp"
#include "../include/full/12_2_006.hpp"
#include "../include/full/12_2_007.hpp"
#include "../include/full/12_2_008.hpp"
#include "../include/full/12_2_009.hpp"
#include "../include/full/12_2_010.hpp"
#include "../include/full/12_2_011.hpp"

signed main()
{
    auto same = [](int q, int d, int st)
    {
        (void)d;
        (void)st;
        return q;
    };
    auto all = [](int q, int st)
    {
        (void)q;
        (void)st;
        return true;
    };
    assert(digitDP(numeric_limits<long long>::max(), 1, 0, same, all) == (1ULL << 63));

    assert(bitmask_dp::tsp({{123}}) == 0);
    assert(bitmask_dp::tsp({{0, 2, 9}, {1, 0, 6}, {15, 7, 0}}) == 17);
    assert(board_part::splitBoard<__int128>({{numeric_limits<int>::max()}}, 1)
           == (__int128)numeric_limits<int>::max() * numeric_limits<int>::max());
    assert(twoMachSched::twoMachines({2, 3, 7}) == 7);
    assert((coin_problem::coinChange({1, 3, 4}, 6, numeric_limits<int>::max() - 24)
            == array<int, 2>{2, 4}));
    assert((hanoi::kthMove(3, 1) == array<int, 3>{1, 0, 2}));
    assert(lis(vector<int>{3, 1, 2, 2, 4}) == vector<int>({1, 2, 4}));

    SubsetSum ss(65);
    ss.add(1);
    ss.add(64);
    assert(ss.has(65));

    int n = 10000;
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++)
    {
        g[i - 1].push_back(i);
        g[i].push_back(i - 1);
    }
    vector<int> w(n, 1);
    assert(treeMWIS(g, w) == 5000);

    int mod = numeric_limits<int>::max() - 24;
    vector<long long> seq(64);
    iota(seq.begin(), seq.end(), 0);
    int want = (int)((((__int128)1 << 64) - 1) % mod);
    assert(distinctSub(seq, mod) == want);

    assert(PlugDP(vector<vector<int>>(4, vector<int>(4))).solve() == 6);
    assert(PlugDP(vector<vector<int>>(3, vector<int>(3))).solve() == 0);
    cout << "OK\n";
}
