#include "../include/supplement_pbds.hpp"
#include "../include/supplement_math.hpp"
#include "../include/supplement_dp_game.hpp"
#include "../include/supplement_misc.hpp"
#include "../include/supplement_string.hpp"

int naiveLast(int n)
{
    // n 是非负整数；用保留足够低位的阶乘模拟返回最后非零位。
    int x = 1;
    for (int i = 2; i <= n; i++)
    {
        x *= i;
        while (x % 10 == 0)
        {
            x /= 10;
        }
        x %= 100000000;
    }
    return x % 10;
}

int bruteBins(vector<int> a, int cap)
{
    // a 是物品体积，cap 是容量；用回溯返回最少箱数。
    sort(a.begin(), a.end(), greater<>());
    vector<int> rem;
    int ans = a.size();
    auto dfs = [&](auto &&self, int p) -> void
    {
        // self 是递归自身，p 是待放物品下标；更新 ans，无返回值。
        if (static_cast<int>(rem.size()) >= ans)
        {
            return;
        }
        if (p == static_cast<int>(a.size()))
        {
            ans = rem.size();
            return;
        }
        set<int> seen;
        for (int i = 0; i < static_cast<int>(rem.size()); i++)
        {
            if (rem[i] < a[p] || seen.contains(rem[i]))
            {
                continue;
            }
            seen.insert(rem[i]);
            rem[i] -= a[p];
            self(self, p + 1);
            rem[i] += a[p];
        }
        rem.push_back(cap - a[p]);
        self(self, p + 1);
        rem.pop_back();
    };
    if (a.empty())
    {
        return 0;
    }
    dfs(dfs, 0);
    return ans;
}

signed main()
{
    // 本测试使用固定样例和固定种子对拍全部补充实现；成功输出 OK。
    assert((pbds_ordered::rankInfo({5, 1, 5, 3}, 5) == array<int, 2>{2, 5}));
    assert(pbdsHeap::heapTop({2, 9, 1}) == 9);
    assert(pbdsMutHeap::editHeap() == 40);
    assert((pbdsHeapCmp::compareHeap({2, 9, 1}) == array<int, 2>{9, 9}));
    assert((pbdsTree::treeSizes({1, 1, 2}) == array<int, 3>{2, 2, 2}));
    pbdsSplit::Map ma, mb;
    ma[1] = 10, ma[3] = 30, ma[5] = 50;
    pbdsSplit::splitAt(ma, mb, 3);
    assert(ma.size() == 2 && mb.size() == 1);
    pbdsSplit::joinOrdered(ma, mb);
    assert(ma.size() == 3 && mb.empty());
    assert(pbdsMeta::rootSize({4, 1, 4, 2}) == 3);
    pbdsSubSum::Tree sumTree;
    for (int x : {1, 3, 5, 8})
    {
        sumTree.insert(x);
    }
    assert(pbdsSubSum::prefixSum(sumTree, 6) == 9);
    assert(pbds_compat::available());

    auto side = planar_graph::chordPlanar(6, {{0, 3}, {1, 4}, {2, 5}});
    assert(!side);
    side = planar_graph::chordPlanar(6, {{0, 2}, {3, 5}});
    assert(side && side->size() == 2);
    assert(planar_graph::faceCount(4, 4) == 2);
    mt19937_64 rng(0x20260723);
    for (int tc = 0; tc < 1000; tc++)
    {
        vector<vector<int>> col(6, vector<int>(6));
        for (int i = 0; i < 6; i++)
        {
            for (int j = i + 1; j < 6; j++)
            {
                col[i][j] = col[j][i] = rng() & 1;
            }
        }
        assert(ramsey::monoTriangle(col));
    }

    for (int x = -10000; x <= 10000; x++)
    {
        for (int m = 1; m <= 40; m++)
        {
            assert(fastDiv::divisible(to_string(x), m) == (x % m == 0));
        }
    }
    for (int lim = 1; lim <= 1000; lim++)
    {
        int best = 1, cnt = 1;
        for (int x = 1; x <= lim; x++)
        {
            int cur = 0;
            for (int d = 1; d * d <= x; d++)
            {
                if (x % d == 0)
                {
                    cur += 1 + (d * d != x);
                }
            }
            if (cur > cnt)
            {
                best = x, cnt = cur;
            }
        }
        assert((highlyComp::mostDivisors(lim) == array<int, 2>{best, cnt}));
    }
    for (int n = 0; n <= 500; n++)
    {
        assert(lastNzDigit::lastDigit(n) == naiveLast(n));
    }

    assert(incl_excl::countMult(100, {2, 3, 5}) == 74);
    for (int n = 1; n <= 8; n++)
    {
        vector<int> a(n);
        for (auto &x : a)
        {
            x = static_cast<int>(rng() % 21) - 10;
        }
        vector<int> mn(n + 1);
        for (int s = 1; s < (1LL << n); s++)
        {
            int v = numeric_limits<int>::max();
            for (int i = 0; i < n; i++)
            {
                if (s >> i & 1)
                {
                    v = min(v, a[i]);
                }
            }
            mn[popcount(static_cast<unsigned long long>(s))] += v;
        }
        sort(a.begin(), a.end(), greater<>());
        for (int k = 1; k <= n; k++)
        {
            assert(minmax_inv::kthMaxMinSum(mn, k) == a[k - 1]);
        }
    }
    vector<int> poly(30);
    for (auto &x : poly)
    {
        x = rng() % roots_unity::mod;
    }
    for (int m : {1, 2, 4, 8})
    {
        for (int r = 0; r < m; r++)
        {
            int want = 0;
            for (int i = r; i < static_cast<int>(poly.size()); i += m)
            {
                want = (want + poly[i]) % roots_unity::mod;
            }
            assert(roots_unity::rootFilter(poly, m, r) == want);
        }
    }
    assert(balls_boxes::unlabBoxDist(5, 3, false) == 21);
    assert(balls_boxes::unlabBoxDist(5, 3, true) == 6);
    assert((tree_count::cayley(4, 1000000007) == array<int, 2>{16, 64}));
    vector<vector<int>> rot = {{0, 1, 2, 3}, {1, 2, 3, 0}, {2, 3, 0, 1}, {3, 0, 1, 2}};
    assert(polya::burnside(rot, 2) == 6);
    assert(young_tab::youngTab({2, 1}) == 2);

    using woodbury::Mat;
    Mat ai = {{0.5L, 0}, {0, 1.0L / 3}}, u = {{1}, {2}}, ci = {{0.25L}}, v = {{3, 4}};
    auto wb = woodbury::woodbury(ai, u, ci, v);
    Mat aplus = {{14, 16}, {24, 35}};
    auto direct = woodbury::inv(aplus);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            assert(abs(wb[i][j] - direct[i][j]) < 1E-12L);
        }
    }
    assert(abs(frac_prog::maxRatio({3, 5, 10}, {2, 5, 4}, 2) - 13.0L / 6) < 1E-12L);

    assert((ogf::ogfProduct({1, 2}, {3, 4}) == vector<int>{3, 10, 8}));
    assert((egf::egfProduct({1, 1}, {1, 1}) == vector<int>{1, 2, 2}));
    assert((gauss_binom::qBinomial(4, 2) == vector<int>{1, 1, 2, 1, 1}));
    vector<int> seq = {2, -1, 4, 3, 7};
    assert(binom_trans::binomTrans(binom_trans::binomTrans(seq), true) == seq);
    for (int n = 1; n <= 20; n++)
    {
        assert(lagrange::lagrangeCoef({1, 1}, n) == 1);
    }
    auto post = prob_formula::bayes({0.5L, 0.5L}, {0.2L, 0.8L});
    assert(abs(post[0] - 0.2L) < 1E-15L);
    assert(abs(probDist::binomialPmf(4, 2, 0.5L) - 0.375L) < 1E-15L);
    auto mom = exp_var::moments({0, 2}, {0.25L, 0.75L});
    assert(abs(mom[0] - 1.5L) < 1E-15L && abs(mom[1] - 0.75L) < 1E-15L);
    assert((harmPow::powerSums(10) == array<__int128, 3>{55, 385, 3025}));
    auto tri = triForm::triangle(3, 4, 5);
    assert(abs(tri[0] - 6) < 1E-15L);

    vector<array<int, 4>> clauses = {{0, 1, 1, 1}, {0, 0, 1, 1}};
    auto lex = lex_two_sat::lexTwoSat(2, clauses);
    assert(lex && *lex == vector<int>({0, 1}));
    assert(bitmask_dp::tsp({{0, 10, 15, 20}, {10, 0, 35, 25}, {15, 35, 0, 30}, {20, 25, 30, 0}}) == 80);
    assert(board_part::splitBoard({{1, 2}, {3, 4}}, 1) == 100);
    assert(twoMachSched::twoMachines({2, 3, 5}) == 5);
    assert((coin_problem::coinChange({1, 2, 5}, 5, 1000000007) == array<int, 2>{1, 4}));

    for (int n = 1; n <= 10; n++)
    {
        unsigned long long total = (1ULL << n) - 1;
        vector<vector<int>> peg(3);
        for (int d = n; d >= 1; d--)
        {
            peg[0].push_back(d);
        }
        for (unsigned long long k = 1; k <= total; k++)
        {
            auto [d, u, v] = hanoi::kthMove(n, k);
            assert(!peg[u].empty() && peg[u].back() == d);
            assert(peg[v].empty() || peg[v].back() > d);
            peg[u].pop_back();
            peg[v].push_back(d);
        }
        assert(static_cast<int>(peg[2].size()) == n);
    }

    vector<int> pre(101);
    for (int i = 0; i <= 100; i++)
    {
        pre[i] = i * i;
    }
    auto layer = dc_dp::divideLayer(pre,
                                         100,
                                         [](int j, int i)
                                         {
                                             // j 是决策点，i 是终点；返回凸转移代价。
                                             return (i - j) * (i - j);
                                         });
    for (int i = 1; i <= 100; i++)
    {
        int want = numeric_limits<int>::max();
        for (int j = 0; j < i; j++)
        {
            want = min(want, pre[j] + (i - j) * (i - j));
        }
        assert(layer[i] == want);
    }
    for (auto [a, b] : vector<array<int, 2>>{{1, 2}, {3, 5}, {4, 7}, {6, 10}, {8, 13}})
    {
        assert(!wythoff_game::firstWins(a, b));
    }
    assert(fib_game::zeckMin(100) == 3);
    assert(edge_del::treeSg({{1}, {0, 2}, {1}}) == 2);
    assert((stone_game::subGameSG(5, {1, 3, 4}) == vector<int>{0, 1, 0, 1, 2, 3}));

    for (int tc = 0; tc < 200; tc++)
    {
        int n = rng() % 10;
        int cap = rng() % 15 + 1;
        vector<int> a(n);
        for (auto &x : a)
        {
            x = rng() % cap + 1;
        }
        assert(bin_packing::exactBins(a, cap) == bruteBins(a, cap));
    }
    assert(grid_count::gridPaths(3, 3, {}) == 6);
    assert(grid_count::gridPaths(3, 3, {{1, 1}}) == 2);

    using triFermat::Point;
    auto close = [](Point a, Point b)
    {
        return hypotl(a.x - b.x, a.y - b.y) < 1E-10L;
    };
    Point a{0, 0}, b{2, 0}, c{1, sqrtl(3.0L)};
    Point mid{1, sqrtl(3.0L) / 3};
    assert(close(*triFermat::incenter(a, b, c), mid));
    assert(close(*triFermat::circumcenter(a, b, c), mid));
    assert(close(*triFermat::orthocenter(a, b, c), mid));
    assert(close(*triFermat::fermat(a, b, c), mid));
    Point d{-2, 0.1L};
    assert(close(*triFermat::fermat(a, b, d), a));
    assert(!triFermat::circumcenter({0, 0}, {1, 0}, {2, 0}));

    for (int m = 1; m <= 15; m += 2)
    {
        auto z = m + 1;
        vector<vector<int>> full(z, vector<int>(z, -1));
        for (int col = 0; col < m; col++)
        {
            full[z - 1][col] = full[col][z - 1] = col;
            for (int d = 1; d <= (m - 1) / 2; d++)
            {
                auto x = (col + d) % m;
                auto y = (col - d + m) % m;
                full[x][y] = full[y][x] = col;
            }
        }
        for (int tc = 0; tc < 40; tc++)
        {
            vector<int> pv(z), pc(m);
            iota(pv.begin(), pv.end(), 0);
            iota(pc.begin(), pc.end(), 0);
            shuffle(pv.begin(), pv.end(), rng);
            shuffle(pc.begin(), pc.end(), rng);
            auto n = static_cast<int>(rng() % (z + 1));
            vector<vector<int>> pre(n, vector<int>(n, -1));
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    if (i != j)
                    {
                        pre[i][j] = pc[full[pv[i]][pv[j]]];
                    }
                }
            }
            auto got = factor_ext::extendFactor(pre, m);
            assert(got && static_cast<int>(got->size()) == z);
            for (int i = 0; i < z; i++)
            {
                vector<int> cnt(m);
                assert((*got)[i][i] == -1);
                for (int j = 0; j < z; j++)
                {
                    assert((*got)[i][j] == (*got)[j][i]);
                    if (i != j)
                    {
                        cnt[(*got)[i][j]]++;
                    }
                    if (i < n && j < n)
                    {
                        assert((*got)[i][j] == pre[i][j]);
                    }
                }
                assert(count(cnt.begin(), cnt.end(), 1) == m);
            }
        }
    }
    assert(!factor_ext::extendFactor({}, 4));

    string s = "banana";
    vector<int> sa = {5, 3, 1, 0, 4, 2};
    assert((suffix_lcp::lcpArray(s, sa) == vector<int>{1, 3, 0, 0, 2}));
    suffix_lcp::LcpRMQ lcp(s, sa);
    assert(lcp.query(1, 3) == 3);
    assert(lcp.query(0, 2) == 0);
    assert(lcp.query(4, 4) == 2);

    cout << "OK\n";
}
