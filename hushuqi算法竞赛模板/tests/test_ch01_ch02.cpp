#include "../include/ch01_ch02.hpp"

mt19937_64 rng(712367821);

void testBase()
{
    int x = 5;
    assert(chmin(x, 3LL) && x == 3);
    assert(!chmin(x, 4LL) && chmax(x, 8LL) && x == 8);
    assert(sgn(-EPS * 2) == -1 && sgn(EPS / 2) == 0 && sgn(EPS * 2) == 1);

    vector<string> vals{
        "0",
        "-1",
        "170141183460469231731687303715884105727",
        "-170141183460469231731687303715884105728",
        "-999999999999999999999999999999999999"
    };
    for (auto &s : vals)
    {
        auto x = parseInt128(s);
        assert(x && toString(*x) == s);
    }
    assert((split("a,,bc,", ',') == vector<string>{"a", "", "bc", ""}));

    DynBitset a(130), b(130);
    for (auto p : {0LL, 63LL, 64LL, 129LL})
    {
        a.set(p);
    }
    b.set(64);
    b.set(65);
    assert(a.count() == 4 && a.test(129));
    auto c = a;
    c &= b;
    assert(c.count() == 1 && c.test(64));
    c = a;
    c |= b;
    assert(c.count() == 5 && c.test(65));
}

void testI128()
{
    mt19937_64 i128Rng(0);
    for (int it = 0; it < 10000; it++)
    {
        long long a = (long long)(i128Rng() & ((1ULL << 62) - 1));
        long long b = (long long)(i128Rng() & ((1ULL << 62) - 1)) + 1;
        if (i128Rng() & 1) a = -a;
        if (i128Rng() & 1) b = -b;
        i128 x(a), y(b);
        array<string, 5> got{
            (x + y).toString(), (x - y).toString(), (x * y).toString(),
            (x / y).toString(), (x % y).toString()
        };
        array<string, 5> want{
            toString((__int128)a + b), toString((__int128)a - b),
            toString((__int128)a * b), toString((__int128)a / b),
            toString((__int128)a % b)
        };
        assert(got == want);
    }

    i128 mn, mx, cross;
    mn.hi = 1ULL << 63;
    mx.hi = (1ULL << 63) - 1;
    mx.lo = ~0ULL;
    cross.hi = cross.lo = 1;
    assert(mn.toString() == "-170141183460469231731687303715884105728");
    assert(mx.toString() == "170141183460469231731687303715884105727");
    assert((mn / mx).toString() == "-1" && (mn % mx).toString() == "-1");
    assert((cross / i128((1LL << 32) + 1)).toString() == "4294967295");
    assert((cross % i128((1LL << 32) + 1)).toString() == "2");
}

void testSearch()
{
    assert(firstTrue(0, 10, [](int x)
    {
        return x * x >= 30;
    }) == 6);
    assert(lastTrue(0, 10, [](int x)
    {
        return x * x <= 30;
    }) == 5);
    auto rt = firstReal(0, 2, [](long double x)
    {
        return x * x >= 2;
    });
    assert(abs(rt - sqrtl(2)) < 1E-15L);
    assert(ternaryInt(-10, 10, [](int x)
    {
        return -(x - 3) * (x - 3);
    }) == 3);
    auto f = [](long double x)
    {
        return -(x - 1.25L) * (x - 1.25L);
    };
    assert(abs(ternaryReal(-10, 10, f) - 1.25L) < 1E-8L);
    assert(abs(goldenMax(-10, 10, f) - 1.25L) < 1E-8L);
    assert(ternaryInt(numeric_limits<int>::min() / 2,
                      numeric_limits<int>::max() / 2,
                      [](int x) -> __int128
                      {
                          return x <= 7 ? (__int128)x : 14 - (__int128)x;
                      }) == 7);
    auto hi = numeric_limits<long double>::max();
    auto target = hi * .75L;
    auto large = firstReal(hi / 2,
                           hi,
                           [&](long double x)
                           {
                               return x >= target;
                           });
    assert(isfinite(large) && abs((large - target) / target) < 1E-15L);

    int pc = 0;
    vector<int> perm{1, 1, 2};
    sort(perm.begin(), perm.end());
    do
    {
        pc++;
    } while (next_permutation(perm.begin(), perm.end()));
    assert(pc == 3);
    int sc = 0, kc = 0;
    for (auto s = 0ULL; s < (1ULL << 5); s++)
    {
        sc++;
    }
    for (auto s = (1ULL << 2) - 1; s < (1ULL << 5);)
    {
        assert(popcount(s) == 2);
        kc++;
        auto x = s & -s;
        auto y = s + x;
        s = y | (s ^ y) / x >> 2;
    }
    assert(sc == 32 && kc == 10);
    int emptySets = 0;
    int k = 0;
    if (k == 0)
    {
        emptySets++;
    }
    assert(emptySets == 1);
    for (auto x = 0ULL; x < 10000; x++)
    {
        auto g = x ^ (x >> 1);
        auto v = g;
        for (auto y = g >> 1; y; y >>= 1)
        {
            v ^= y;
        }
        assert(v == x);
    }

    vector<vector<int>> g{{1, 2}, {3}, {3}, {4}, {}};
    assert((bfs(g, 0) == vector<int>{0, 1, 1, 2, 3}));
    assert((dfsOrder(g, 0) == vector<int>{0, 1, 3, 4, 2}));
    vector<vector<int>> cross{{1, 2, 3}, {3}, {}, {}};
    assert((dfsOrder(cross, 0) == vector<int>{0, 1, 3, 2}));

    vector<vector<WEdge>> wg(4);
    wg[0] = {{1, 1}, {2, 2}, {3, 10}};
    wg[1] = {{2, 1}, {3, 4}};
    wg[2] = {{3, 1}};
    assert(kthShortest(wg, 0, 3, 1) == 3);
    assert(kthShortest(wg, 0, 3, 2) == 3);
    assert(kthShortest(wg, 0, 3, 3) == 5);
    assert(kthShortest(wg, 3, 3, 1, true) == 0);
    vector<vector<WEdge>> largeGraph(2);
    largeGraph[0].push_back({1, INF + 7});
    assert(kthShortest(largeGraph, 0, 1, 1) == INF + 7);

    auto next = [](long double x, auto &, long double)
    {
        return (x + 3) / 2;
    };
    auto score = [](long double x)
    {
        return (x - 3) * (x - 3);
    };
    auto [p, val] = anneal(100.0L, score, next, rng, 1, 1E-8L, .9L, 1);
    assert(abs(p - 3) < 1E-6L && val < 1E-12L);
    auto intNext = [](int, auto &, long double)
    {
        return 1LL;
    };
    auto intScore = [](int x)
    {
        return x ? numeric_limits<int>::min() : numeric_limits<int>::max();
    };
    auto [best, bestVal] = anneal(0LL, intScore, intNext, rng, 1, .5L, .5L, 1);
    assert(best == 1 && bestVal < 0);
}

void testSortAndSequence()
{
    struct Item
    {
        int x;

        Item() = delete;
        explicit Item(int v) : x(v) {}
    };
    vector<Item> items;
    items.emplace_back(3);
    items.emplace_back(1);
    items.emplace_back(2);
    mergeSort(items,
              [](const Item &x, const Item &y)
              {
                  return x.x < y.x;
              });
    assert(items[0].x == 1 && items[1].x == 2 && items[2].x == 3);

    vector<int> radixEdge{numeric_limits<int>::min(), -1, 0, numeric_limits<int>::max()};
    radixSort(radixEdge);
    assert((radixEdge == vector<int>{numeric_limits<int>::min(), -1, 0, numeric_limits<int>::max()}));

    for (int tc = 0; tc < 1000; tc++)
    {
        int n = rng() % 80;
        vector<int> a(n);
        for (auto &x : a)
        {
            x = (int)(rng() % 2001) - 1000;
        }
        auto want = a;
        sort(want.begin(), want.end());
        auto b = a, c = a;
        mergeSort(b);
        radixSort(c);
        assert(b == want && c == want);
        if (!a.empty())
        {
            auto k = rng() % n;
            assert(nthValue(a, k) == want[k]);
            assert(quickSelect(a, k, rng) == want[k]);
        }

        int inv = 0;
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                inv += a[i] > a[j];
            }
        }
        assert(invMerge(a) == inv);
        assert(invBit(a) == inv);
    }

    Discrete<int> dis({5, -1, 5, 8, 0});
    assert(dis.a == vector<int>({-1, 0, 5, 8}));
    assert(dis.id(5) == 2 && dis.lower(7) == 3);
    FracCascade<int> empty({});
    assert(empty.lowerBound(0).empty());

    for (int tc = 0; tc < 500; tc++)
    {
        int n = rng() % 25 + 1;
        vector<Point3> p(n);
        for (int i = 0; i < n; i++)
        {
            p[i] = {(int)(rng() % 8), (int)(rng() % 8), (int)(rng() % 8), i};
        }
        auto got = dom3(p);
        for (int i = 0; i < n; i++)
        {
            int want = 0;
            for (int j = 0; j < n; j++)
            {
                want += p[j][0] <= p[i][0] && p[j][1] <= p[i][1] && p[j][2] <= p[i][2];
            }
            assert(got[i] == want);
        }
    }

    for (int tc = 0; tc < 200; tc++)
    {
        int n = rng() % 12 + 1;
        vector<vector<int>> a(n);
        for (auto &v : a)
        {
            int m = rng() % 30;
            v.resize(m);
            for (auto &x : v)
            {
                x = (int)(rng() % 51) - 25;
            }
            sort(v.begin(), v.end());
        }
        FracCascade<int> fc(a);
        for (int x = -30; x <= 30; x++)
        {
            auto got = fc.lowerBound(x);
            for (int i = 0; i < n; i++)
            {
                auto want = lower_bound(a[i].begin(), a[i].end(), x) - a[i].begin();
                assert(got[i] == want);
            }
        }
    }

    assert(rectArea({{0, 0, 2, 2}, {1, 1, 3, 2}, {5, 0, 5, 3}}) == 5);
    for (int tc = 0; tc < 300; tc++)
    {
        int n = rng() % 8;
        vector<Rect> rs(n);
        for (auto &r : rs)
        {
            auto &[xl, yl, xr, yr] = r;
            xl = rng() % 8;
            xr = rng() % 8;
            yl = rng() % 8;
            yr = rng() % 8;
            if (xl > xr) swap(xl, xr);
            if (yl > yr) swap(yl, yr);
        }
        int want = 0;
        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                bool ok = false;
                for (auto r : rs)
                {
                    auto [xl, yl, xr, yr] = r;
                    ok |= xl <= x && x < xr && yl <= y && y < yr;
                }
                want += ok;
            }
        }
    assert(rectArea(rs) == want);
    }

    assert((prevLess(vector<int>{3, 1, 2, 2, 4}) == vector<int>{-1, -1, 1, 1, 3}));
    assert((windowMin(vector<int>{3, 1, 2, 5, 4}, 3) == vector<int>{1, 1, 2}));
    assert((windowMax(vector<int>{3, 1, 2, 5, 4}, 3) == vector<int>{3, 5, 5}));
    auto a = vector<int>{3, 1, 2, 5, 4, 6};
    auto ids = lis(a);
    assert(ids.size() == 4);
    for (int i = 1; i < (int)ids.size(); i++)
    {
        assert(a[ids[i - 1]] < a[ids[i]]);
    }
    assert(lis2D({{1, 1}, {1, 2}, {2, 2}, {3, 3}}) == 3);
    assert(lcis({3, 4, 9, 1}, {5, 3, 8, 9, 10, 2, 1}) == 2);
    assert(maxRect(vector<int>{2, 1, 5, 6, 2, 3}) == 10);
    assert(maxRect(vector<int>{1'000'000'000, 1'000'000'000}) == 2'000'000'000);
    assert(maxRect(vector<vector<int>>{{1, 0, 1, 1}, {1, 1, 1, 1}, {0, 1, 1, 1}}) == 6);
    int joseph = 0;
    for (int i = 2; i <= 5; i++)
    {
        joseph = (joseph + 2) % i;
    }
    assert(joseph == 2);
}

void testOffline()
{
    assert((xorPairs({0, 0, 0}, {{0, 3}, {1, 1}}, 0, 0) == vector<int>{3, 0}));
    for (int tc = 0; tc < 200; tc++)
    {
        int n = rng() % 80 + 1;
        int qn = rng() % 150 + 1;
        vector<int> a(n);
        for (auto &x : a)
        {
            x = (int)(rng() % 101) - 50;
        }
        vector<MoQuery> mq(qn);
        vector<pair<int, int>> qs(qn);
        for (int id = 0; id < qn; id++)
        {
            int l = rng() % (n + 1);
            int r = rng() % (n + 1);
            if (l > r) swap(l, r);
            mq[id] = {l, r, id};
            qs[id] = {l, r};
        }
        int cur = 0;
        auto got = mo<int>(n, mq,
                           [&](int p)
        {
            cur += a[p];
        },
        [&](int p)
        {
            cur += a[p];
        },
        [&](int p)
        {
            cur -= a[p];
        },
        [&](int p)
        {
            cur -= a[p];
        },
        [&]()
        {
            return cur;
        });
        for (int id = 0; id < qn; id++)
        {
            auto [l, r] = qs[id];
            assert(got[id] == accumulate(a.begin() + l, a.begin() + r, 0LL));
        }

        auto gotDistinct = distinct(a, qs);
        for (int id = 0; id < qn; id++)
        {
            auto [l, r] = qs[id];
            set<int> s(a.begin() + l, a.begin() + r);
            assert(gotDistinct[id] == (int)s.size());
        }

        vector<KthQuery> kq;
        vector<int> want;
        for (int id = 0; id < qn; id++)
        {
            auto [l, r] = qs[id];
            if (l == r) continue;
            int k = rng() % (r - l) + 1;
            kq.push_back({l, r, k});
            vector<int> v(a.begin() + l, a.begin() + r);
            sort(v.begin(), v.end());
            want.push_back(v[k - 1]);
        }
        assert(rangeKth(a, kq) == want);
    }

    for (int tc = 0; tc < 300; tc++)
    {
        int n = rng() % 30 + 1;
        int qn = rng() % 50 + 1;
        int k = rng() % 6;
        vector<int> a(n);
        for (auto &x : a)
        {
            x = rng() % 32;
        }
        vector<pair<int, int>> qs(qn);
        vector<int> want(qn);
        for (int id = 0; id < qn; id++)
        {
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r) swap(l, r);
            qs[id] = {l, r};
            for (int i = l; i < r; i++)
            {
                for (int j = i + 1; j < r; j++)
                {
                    want[id] += popcount((unsigned long long)(a[i] ^ a[j])) == k;
                }
            }
        }
        assert(xorPairs(a, qs, k, 5) == want);
    }

    vector<int> a{1, 2, 1, 3, 2, 4, 1};
    vector<MoQuery> qs{{0, 7, 0}, {1, 5, 1}, {2, 3, 2}, {3, 7, 3}, {0, 0, 4}};
    vector<int> cnt(5), hist;
    int kinds = 0;
    auto got = rollbackMo<int>(a.size(), qs,
                               [&](int p)
    {
        hist.push_back(a[p]);
        if (!cnt[a[p]]++) kinds++;
    },
    [&]()
    {
        return (int)hist.size();
    },
    [&](int s)
    {
        while ((int)hist.size() > s)
        {
            auto x = hist.back();
            hist.pop_back();
            if (!--cnt[x]) kinds--;
        }
    },
    [&]()
    {
        return kinds;
    });
    assert((got == vector<int>{4, 3, 1, 4, 0}));

    // Change[0..2] 依次表示修改位置、旧值、新值。
    using Change = array<int, 3>;
    vector<int> cur{1, 2, 1, 3};
    vector<Change> cs{{1, 2, 5}, {2, 1, -2}};
    vector<ModifyQuery> qm{{0, 4, 0, 0}, {0, 3, 1, 1}, {1, 4, 2, 2}};
    int sum = 0;
    auto mod = moModify<int>(cur.size(), qm,
                             [&](int p)
    {
        sum += cur[p];
    },
    [&](int p)
    {
        sum -= cur[p];
    },
    [&](int id, bool fwd, int l, int r)
    {
        auto [p, from, to] = cs[id];
        auto v = fwd ? to : from;
        if (l <= p && p < r) sum += v - cur[p];
        cur[p] = v;
    },
    [&]()
    {
        return sum;
    });
    assert((mod == vector<int>{7, 7, 6}));

    TreeMo tr(4);
    tr.addTreeEdge(0, 1);
    tr.addTreeEdge(0, 2);
    tr.addTreeEdge(1, 3);
    tr.col = {1, 2, 1, 3};
    tr.build(0);
    for (auto [u, v] : vector<pair<int, int>>{{3, 2}, {3, 1}, {2, 2}, {3, 0}})
    {
        tr.addQuery(u, v);
    }
    assert((tr.solve() == vector<int>{3, 2, 1, 3}));
    tr.build(3);
    assert(tr.qs.empty() && tr.dep[3] == 0);
    tr.addQuery(2, 3);
    assert((tr.solve() == vector<int>{3}));
}

void testPrefix()
{
    vector<int> base{2, -1, 4};
    vector<int> pre(4);
    partial_sum(base.begin(), base.end(), pre.begin() + 1);
    assert((pre == vector<int>{0, 2, 1, 5}));
    Prefix2<int> p({{1, 2, 3}, {4, 5, 6}});
    assert(p.sum(0, 1, 2, 3) == 16 && p.sum(1, 0, 2, 1) == 4);

    Diff<int> d(6);
    d.add(1, 4, 3);
    d.add(2, 6, -1);
    assert((d.build() == vector<int>{0, 3, 2, 2, -1, -1}));
    Diff2<int> d2(3, 4);
    d2.add(0, 1, 2, 3, 5);
    auto a2 = d2.build();
    assert((a2 == vector<vector<int>>{{0, 5, 5, 0}, {0, 5, 5, 0}, {0, 0, 0, 0}}));
    assert((kthDiff(vector<int>{0, 1, 4, 9, 16}, 2) == vector<int>{2, 2, 2}));
    assert(kthDiff(vector<int>{}, numeric_limits<int>::max()).empty());

    assert(maxSub(vector<int>{-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
    auto wide = (__int128)numeric_limits<int>::max() * 2;
    assert(maxSub(vector<__int128>{numeric_limits<int>::max(), numeric_limits<int>::max()}) == wide);
    assert(maxSubMat(vector<vector<__int128>>{{numeric_limits<int>::max()},
                                              {numeric_limits<int>::max()}}) == wide);
    vector<vector<int>> m{{1, -2, 3}, {-4, 5, 6}, {-1, 2, -3}};
    int brute = -INF;
    for (int x1 = 0; x1 < 3; x1++)
    {
        for (int x2 = x1 + 1; x2 <= 3; x2++)
        {
            for (int y1 = 0; y1 < 3; y1++)
            {
                for (int y2 = y1 + 1; y2 <= 3; y2++)
                {
                    int s = 0;
                    for (int x = x1; x < x2; x++)
                    {
                        for (int y = y1; y < y2; y++)
                        {
                            s += m[x][y];
                        }
                    }
                    chmax(brute, s);
                }
            }
        }
    }
    assert(maxSubMat(m) == brute);

    vector<int> a{6, 10, 15, 9, 3};
    GcdRanges<int> gr(a);
    for (int l = 0; l < (int)a.size(); l++)
    {
        int g = 0;
        for (int r = l; r < (int)a.size(); r++)
        {
            g = gcd(g, abs(a[r]));
            assert(gr.query(l, r) == (unsigned long long)g);
        }
    }
    GcdRanges<int> grMin({numeric_limits<int>::min()});
    assert(grMin.query(0, 0) == (1ULL << 63));
    GcdRanges<int> grZero({0, 0, 0});
    assert(grZero.query(0, 2) == 0);
    for (auto l = 0ULL; l < 100; l++)
    {
        for (auto r = l; r < 100; r++)
        {
            auto x = 0ULL;
            for (auto v = l; v <= r; v++)
            {
                x ^= v;
            }
            auto right = r;
            if ((r & 3) == 1) right = 1;
            else if ((r & 3) == 2) right = r + 1;
            else if ((r & 3) == 3) right = 0;
            auto left = 0ULL;
            if (l)
            {
                auto v = l - 1;
                left = v;
                if ((v & 3) == 1) left = 1;
                else if ((v & 3) == 2) left = v + 1;
                else if ((v & 3) == 3) left = 0;
            }
            assert((right ^ left) == x);
        }
    }
}

signed main()
{
    testBase();
    testI128();
    testSearch();
    testSortAndSequence();
    testOffline();
    testPrefix();
    cout << "OK\n";
}
