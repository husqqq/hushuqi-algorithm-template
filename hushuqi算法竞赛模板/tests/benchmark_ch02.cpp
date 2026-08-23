#include "../include/ch01_ch02.hpp"

using Clock = chrono::steady_clock;

template <class F> int run(F f)
{
    auto st = Clock::now();
    f();
    return chrono::duration_cast<chrono::milliseconds>(Clock::now() - st).count();
}

signed main()
{
    mt19937_64 rng(20260723);
    constexpr int N = 1000000;
    vector<int> src(N);
    for (auto &x : src)
    {
        x = (int)rng();
    }

    auto a = src;
    auto sortMs = run([&]()
    {
        sort(a.begin(), a.end());
    });
    auto b = src;
    auto mergeMs = run([&]()
    {
        mergeSort(b);
    });
    auto c = src;
    auto radixMs = run([&]()
    {
        radixSort(c);
    });
    assert(a == b && a == c);

    constexpr int R = 100;
    vector<vector<int>> sel(R, vector<int>(100000));
    for (auto &v : sel)
    {
        for (auto &x : v)
        {
            x = (int)rng();
        }
    }
    int sink = 0;
    auto nthMs = run([&]()
    {
        for (auto v : sel)
        {
            auto k = v.size() / 2;
            sink ^= nthValue(move(v), k);
        }
    });
    auto quickMs = run([&]()
    {
        for (auto v : sel)
        {
            auto k = v.size() / 2;
            sink ^= quickSelect(move(v), k, rng);
        }
    });

    int xInv = 0, yInv = 0;
    auto invMergeMs = run([&]()
    {
        xInv = invMerge(src);
    });
    auto invBitMs = run([&]()
    {
        yInv = invBit(src);
    });
    assert(xInv == yInv);
    sink ^= xInv;

    cout << "sort_ms " << sortMs << '\n';
    cout << "merge_sort_ms " << mergeMs << '\n';
    cout << "radix_sort_ms " << radixMs << '\n';
    cout << "nth_element_ms " << nthMs << '\n';
    cout << "quick_select_ms " << quickMs << '\n';
    cout << "inversion_merge_ms " << invMergeMs << '\n';
    cout << "inversion_bit_ms " << invBitMs << '\n';
    cerr << "ignore " << sink << '\n';
}
