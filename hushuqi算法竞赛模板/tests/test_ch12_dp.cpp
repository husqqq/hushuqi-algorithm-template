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
    assert((coin_problem::coinChange({}, 0, 1) == array<int, 2>{0, 0}));
    assert((coin_problem::coinChange({3, 1}, 0, 6) == array<int, 2>{0, 1}));
    assert((coin_problem::coinChange({}, 3, 1) == array<int, 2>{-1, 0}));
    assert((hanoi::kthMove(63, (1ULL << 62) + 1) == array<int, 3>{1, 1, 0}));
    // 用真实递归移动序列核对公式，不复用其方向或次数公式。
    for (int n = 1; n <= 10; n++)
    {
        unsigned long long step = 0;
        auto rec = [&](auto &&self, int d, int from, int to, int aux) -> void
        {
            if (!d) return;
            self(self, d - 1, from, aux, to);
            assert((hanoi::kthMove(n, ++step) == array<int, 3>{d, from, to}));
            self(self, d - 1, aux, to, from);
        };
        rec(rec, n, 0, 2, 1);
    }
    for (int seed = 0; seed < 200; seed++)
    {
        mt19937 rng(seed);
        vector<int> coin{1, 2, 3, 4, 5, 6, 7};
        shuffle(coin.begin(), coin.end(), rng);
        coin.resize(rng() % 8);
        int target = rng() % 16, best = target + 1, ways = 0;
        auto dfs = [&](auto &&self, int i, int left, int count) -> void
        {
            if (i == (int)coin.size())
            {
                if (left == 0)
                {
                    best = min(best, count);
                    ways++;
                }
                return;
            }
            for (int c = 0; c * coin[i] <= left; c++)
            {
                self(self, i + 1, left - c * coin[i], count + c);
            }
        };
        dfs(dfs, 0, target, 0);
        if (ways == 0) best = -1;
        for (int mod : vector<int>{1, 2, 6, 1000000007, numeric_limits<int>::max() - 24})
        {
            auto got = coin_problem::coinChange(coin, target, mod);
            if (got != array<int, 2>{best, ways % mod})
            {
                cerr << "target=" << target << " mod=" << mod << " coins:";
                for (int c : coin)
                {
                    cerr << ' ' << c;
                }
                cerr << '\n';
                cout << "WA " << seed << '\n';
                return 1;
            }
        }
        cout << "AC " << seed << '\n';
    }
    cout << "OK\n";
}
