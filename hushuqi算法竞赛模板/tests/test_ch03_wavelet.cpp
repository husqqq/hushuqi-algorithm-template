#ifndef WAVELET_HEADER
#define WAVELET_HEADER "../include/full/3_4_005.hpp"
#endif
#include WAVELET_HEADER

void check(bool ok)
{
    // ok 是结果是否符合独立预期；失败时终止，不受 NDEBUG 影响。
    if (!ok)
    {
        throw runtime_error("小波矩阵结果不符");
    }
}

template <int LOG> void test(vector<int> a, mt19937_64 &rng)
{
    // a 是总和可表示的合法序列，rng 产生查询；用排序和逐项扫描独立核对全部接口。
    Wavelet<LOG> w;
    KthWavelet<LOG> k;
    w.init({1});
    k.init({1});
    w.init({});
    k.init({});
    check(w.countSumLE(0, 0, numeric_limits<int>::max()) == make_pair((int)0, 0LL));
    w.init(a);
    k.init(a);
    int n = a.size();
    vector<int> bound{numeric_limits<int>::min(), -1, 0, 1, numeric_limits<int>::max()};
    if constexpr (LOG < numeric_limits<int>::digits)
    {
        bound.push_back((1LL << LOG) - 1);
        bound.push_back(1LL << LOG);
    }
    for (int q = 0; q < 180; q++)
    {
        int l = rng() % (n + 1), r = rng() % (n + 1);
        if (l > r)
        {
            swap(l, r);
        }
        if (q == 0)
        {
            l = 0;
            r = n;
        }
        vector<int> sorted(a.begin() + l, a.begin() + r);
        sort(sorted.begin(), sorted.end());
        for (int rank = 0; rank < r - l; rank++)
        {
            check(w.kth(l, r, rank) == sorted[rank]);
            check(k.kth(l, r, rank) == sorted[rank]);
        }
        vector<int> xs = bound;
        if (n)
        {
            int x = a[rng() % n];
            xs.push_back(x);
            xs.push_back(x - 1);
            if (x < numeric_limits<int>::max())
            {
                xs.push_back(x + 1);
            }
        }
        for (int x : xs)
        {
            int lt = 0, le = 0, eq = 0;
            long long sumLT = 0, sumLE = 0;
            for (int i = l; i < r; i++)
            {
                if (a[i] < x)
                {
                    lt++;
                    sumLT += a[i];
                }
                if (a[i] <= x)
                {
                    le++;
                    sumLE += a[i];
                }
                eq += a[i] == x;
            }
            check(w.countLess(l, r, x) == lt);
            check(w.frequency(l, r, x) == eq);
            check(w.countSumLT(l, r, x) == make_pair(lt, sumLT));
            check(w.countSumLE(l, r, x) == make_pair(le, sumLE));
        }
    }
}

signed main()
{
    for (int seed = 0; seed < 16; seed++)
    {
        try
        {
            mt19937_64 rng(seed);
            test<1>({}, rng);
            test<1>({0}, rng);
            test<1>({1, 0, 1, 1, 0}, rng);
            test<9>({0, 1, 255, 511, 511}, rng);
            test<9>(vector<int>(32, 511), rng);
            test<30>({1000000000, 1000000000, 1000000000, 0}, rng);
            vector<int> a(48);
            for (int &x : a)
            {
                x = rng() % 512;
            }
            test<9>(a, rng);
            for (int &x : a)
            {
                x = rng() % 1000000001;
            }
            test<30>(a, rng);
#ifdef int
            test<62>({0, 1LL << 60, (1LL << 62) - 1, 1, 2}, rng);
            test<62>({(1LL << 62) - 1, (1LL << 62) - 1, 1}, rng);
            // 只查第 k 小不累加，此处总和超过有符号 64 位。
            KthWavelet<62> k({(1LL << 62) - 1, 1LL << 61, (1LL << 62) - 1});
            check(k.kth(0, 3, 0) == (1LL << 61));
            check(k.kth(0, 3, 2) == (1LL << 62) - 1);
#endif
            cout << "AC " << seed << '\n';
        }
        catch (const exception &)
        {
            cout << "WA " << seed << '\n';
            return 1;
        }
    }
}
