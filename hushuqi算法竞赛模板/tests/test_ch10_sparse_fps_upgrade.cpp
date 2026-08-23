#include "../include/full/10_2_005.hpp"
#include "../include/full/10_2_006.hpp"
#include "../include/full/10_2_014.hpp"

Poly denseSparse(int n, const vector<pair<int, Z>> &f)
{
    Poly a(n);
    for (auto [i, x] : f)
    {
        if (i < n)
        {
            a[i] += x;
        }
    }
    return a;
}

vector<pair<int, Z>> makeSparse(const Poly &a)
{
    vector<pair<int, Z>> f;
    for (int i = 0; i < (int)a.size(); i++)
    {
        if (a[i] != Z(0))
        {
            f.push_back({i, a[i]});
        }
    }
    return f;
}

signed main()
{
    assert(sparseDiv({}, {}, 0).empty());
    assert(sparseInv(0, {}).empty());
    assert(sparsePow(0, {}, 0).empty());
    assert((sparsePow(1, {}, 0) == vector<Z>{1}));
    assert((sparsePow(5, {}, 1) == vector<Z>(5)));
    vector<pair<int, Z>> mixed{{3, 4}, {1, 2}, {3, -1}, {1, -2}, {2, 5}, {2, 0}};
    assert((spNorm(mixed) == vector<pair<int, Z>>{{2, 5}, {3, 3}}));
    assert(sparseExp(12, mixed) == fpsExp(denseSparse(12, mixed), 12));
    vector<pair<int, Z>> unit{{3, 4}, {0, 1}, {2, 5}, {3, -1}, {2, -2}};
    assert(sparseLog(12, unit) == fpsLog(denseSparse(12, unit), 12));

    vector<pair<int, Z>> shifted{{2, 3}, {3, 6}, {5, 4}};
    for (long long k : {1LL, 2LL, 5LL, 1000000000000000000LL})
    {
        int n = 18;
        assert(sparsePow(n, shifted, k) ==
               vector<Z>(fpsPow(denseSparse(n, shifted), k, n)));
    }
    auto inside = sparsePow(6, vector<pair<int, Z>>{{2, 3}}, 2);
    assert(inside[4] == Z(9));
    assert(sparsePow(6, vector<pair<int, Z>>{{2, 3}}, 3) == vector<Z>(6));

    assert(sparseSqrt(0, {}, Z(0)) == optional<vector<Z>>(vector<Z>{}));
    assert(sparseSqrt(7, {}, Z(0)) == optional<vector<Z>>(vector<Z>(7)));
    assert(!sparseSqrt(5, vector<pair<int, Z>>{{1, 1}}, Z(1)));
    assert(!sparseSqrt(5, vector<pair<int, Z>>{{0, 4}}, Z(3)));
    assert(!sparseSqrt(5, vector<pair<int, Z>>{{0, 3}}));

    vector<pair<int, Z>> square{{4, 9}, {5, 18}, {6, 9}, {8, 6}, {9, 6}, {12, 1}};
    int squareN = 14;
    Poly squareDense = denseSparse(squareN, square);
    auto sparseRoot = sparseSqrt(squareN, square, Z(3));
    auto denseRoot = fpsSqrt(squareDense, squareN, Z(3));
    assert(sparseRoot && denseRoot);
    for (int i = 0; i < squareN - 2; i++)
    {
        assert((*sparseRoot)[i] == (*denseRoot)[i]);
    }
    assert((Poly(*sparseRoot) * Poly(*sparseRoot)).cut(squareN) == squareDense);
    assert((Poly(*denseRoot) * Poly(*denseRoot)).cut(squareN) == squareDense);
    auto automaticSparseRoot = sparseSqrt(squareN, square);
    auto automaticDenseRoot = fpsSqrt(squareDense, squareN);
    assert(automaticSparseRoot && automaticDenseRoot);
    for (int i = 0; i < squareN - 2; i++)
    {
        assert((*automaticSparseRoot)[i] == (*automaticDenseRoot)[i]);
    }

    mt19937_64 rng(20260816);
    for (int round = 0; round < 200; round++)
    {
        int n = rng() % 35 + 1;
        Poly a(n);
        int first = rng() % n;
        a[first] = rng() % (mod - 1) + 1;
        for (int i = first + 1; i < n; i++)
        {
            if (rng() % 5 == 0)
            {
                a[i] = rng() % mod;
            }
        }
        auto f = makeSparse(a);
        long long k = rng() % 8;
        assert(sparsePow(n, f, k) == vector<Z>(fpsPow(a, k, n)));

        auto shuffled = f;
        if (!shuffled.empty())
        {
            int i = rng() % shuffled.size();
            auto [d, x] = shuffled[i];
            shuffled[i].second -= x;
            shuffled.push_back({d, x});
            shuffle(shuffled.begin(), shuffled.end(), rng);
        }
        assert(sparsePow(n, shuffled, k) == vector<Z>(fpsPow(a, k, n)));

        Poly h(n);
        int shift = rng() % ((n + 1) / 2);
        h[shift] = rng() % (mod - 1) + 1;
        for (int i = shift + 1; i < n; i++)
        {
            if (rng() % 6 == 0)
            {
                h[i] = rng() % mod;
            }
        }
        Poly sq = (h * h).cut(n);
        auto sf = makeSparse(sq);
        auto sr = sparseSqrt(n, sf, h[shift]);
        auto dr = fpsSqrt(sq, n, h[shift]);
        assert(sr && dr);
        for (int i = 0; i < n - shift; i++)
        {
            assert((*sr)[i] == (*dr)[i]);
        }
        assert((Poly(*sr) * Poly(*sr)).cut(n) == sq);
        assert((Poly(*dr) * Poly(*dr)).cut(n) == sq);
        assert((Poly(*sr) * Poly(*sr)).cut(n) == sq);
    }

    for (int round = 0; round < 200; round++)
    {
        int n = rng() % 80 + 1;
        vector<pair<int, Z>> b{{0, Z(rng() % (mod - 1) + 1)}};
        for (int i = 1; i < n; i++)
        {
            if (rng() % 7 == 0)
            {
                b.push_back({i, Z(rng() % (mod - 1) + 1)});
            }
        }
        assert(sparseInv(n, b) == fpsInv(denseSparse(n, b), n));
    }

    for (int round = 0; round < 200; round++)
    {
        int n = rng() % 60 + 1;
        Poly a(n);
        for (int i = 1; i < n; i++)
        {
            if (rng() % 6 == 0)
            {
                a[i] = rng() % mod;
            }
        }
        auto f = makeSparse(a);
        if (!f.empty())
        {
            int i = rng() % f.size();
            auto [d, x] = f[i];
            f[i].second -= x;
            f.push_back({d, x});
        }
        shuffle(f.begin(), f.end(), rng);
        assert(sparseExp(n, f) == fpsExp(a, n));

        a[0] = 1;
        f = makeSparse(a);
        if (f.size() > 1)
        {
            int i = rng() % (f.size() - 1) + 1;
            auto [d, x] = f[i];
            f[i].second -= x;
            f.push_back({d, x});
        }
        shuffle(f.begin(), f.end(), rng);
        assert(sparseLog(n, f) == fpsLog(a, n));
    }

    cout << "OK\n";
    return 0;
}
