#include "../include/full/2_1_001.hpp"

void checkBinary(int l, int r, int p)
{
    // [l,r] 是非空闭区间，p 是其中的分界；检查两个方向的答案及谓词调用次数。
    int calls = 0;
    int limit = numeric_limits<int>::digits + 1;
    assert(firstTrue(l, r, [&](int x)
    {
        assert(l <= x && x <= r && ++calls <= limit);
        return x >= p;
    }) == p);
    calls = 0;
    assert(lastTrue(l, r, [&](int x)
    {
        assert(l <= x && x <= r && ++calls <= limit);
        return x <= p;
    }) == p);
}

signed main()
{
    for (int l = -12; l <= 12; l++)
    {
        for (int r = l; r <= 12; r++)
        {
            for (int p = l; p <= r; p++)
            {
                checkBinary(l, r, p);
            }
        }
    }
    int low = numeric_limits<int>::min(), high = numeric_limits<int>::max();
    for (int p : {low, low + 1, (int)-1, (int)0, (int)1, high - 1, high})
    {
        checkBinary(low, high, p);
        checkBinary(p, p, p);
    }
    assert(lastTrue(0, 10, [](int) { return true; }) == 10);
    assert(firstTrue(0, 10, [](int) { return true; }) == 0);
    long double top = numeric_limits<long double>::max(), target = top * .75L;
    auto real = firstReal(-top, top, [&](long double x)
    {
        return x >= target;
    }, 120);
    assert(isfinite(real) && fabsl((real - target) / target) < 1E-15L);
    assert(firstReal(-top, top, [](long double) { return true; }, 0) == 0);
    assert(firstReal(3, 3, [](long double) { return true; }) == 3);
    long double next = nextafterl(1, 2);
    assert(firstReal(1, next, [&](long double x) { return x >= next; }) >= 1);

    mt19937_64 rng(0);
    for (int n = 0; n <= 300; n++)
    {
        vector<int> a(n);
        for (auto &x : a)
        {
            x = rng() % 31 - 15;
        }
        sort(a.begin(), a.end());
        for (int x = -20; x <= 20; x++)
        {
            assert(fastLower(a.begin(), a.end(), x) == lower_bound(a.begin(), a.end(), x));
            assert(fastUpper(a.begin(), a.end(), x) == upper_bound(a.begin(), a.end(), x));
        }
    }
    vector<pair<int, int>> a{{1, 4}, {1, 2}, {3, 0}};
    sort(a.begin(), a.end());
    auto cmp = [](const pair<int, int> &x, int y)
    {
        return x.first < y;
    };
    assert(fastLower(a.begin(), a.end(), 2, cmp) == a.begin() + 2);
    auto upper = [](int x, const pair<int, int> &y)
    {
        return x < y.first;
    };
    assert(fastUpper(a.begin(), a.end(), 1, upper) == a.begin() + 2);
    vector<int> descending{9, 7, 7, 3};
    for (int x = 0; x <= 10; x++)
    {
        assert(fastLower(descending.begin(), descending.end(), x, greater<>()) ==
               lower_bound(descending.begin(), descending.end(), x, greater<>()));
        assert(fastUpper(descending.begin(), descending.end(), x, greater<>()) ==
               upper_bound(descending.begin(), descending.end(), x, greater<>()));
    }
    // 虚拟递增区间无需分配内存，可直接覆盖长度为 int 最大值的边界；按数学位置核对结果。
    for (int n : {(int)0, (int)1, (int)2, high - 1, high})
    {
        auto view = views::iota((int)0, n);
        for (int x : {low, (int)0, (int)1, n / 2, n - 1, n, high})
        {
            int lower = x <= 0 ? 0 : x >= n ? n : x;
            int upper = x < 0 ? 0 : x >= n - 1 ? n : x + 1;
            assert(fastLower(view.begin(), view.end(), x) - view.begin() == lower);
            assert(fastUpper(view.begin(), view.end(), x) - view.begin() == upper);
        }
    }
    cout << "OK\n";
}
