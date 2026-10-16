#ifndef INV_HEADER
#define INV_HEADER "../include/full/3_4_011.hpp"
#endif
#include INV_HEADER

void check(bool ok)
{
    // ok 表示独立预期是否成立；失败抛出异常，不受 NDEBUG 影响。
    if (!ok) throw runtime_error("静态区间逆序对结果不符");
}

void verify(const vector<int> &a, mt19937_64 &rng)
{
    // a 是任意整数序列，rng 用于打乱询问；逐个新增右端点直接扫描此前元素。
    int n = a.size();
    vector<pair<int, int>> qs;
    vector<long long> want;
    for (int l = 0; l <= n; l++)
    {
        long long cur = 0;
        qs.push_back({l, l});
        want.push_back(0);
        for (int r = l; r < n; r++)
        {
            for (int i = l; i < r; i++)
            {
                cur += a[i] > a[r];
            }
            qs.push_back({l, r + 1});
            want.push_back(cur);
        }
    }
    vector<int> ids(qs.size());
    iota(ids.begin(), ids.end(), 0);
    shuffle(ids.begin(), ids.end(), rng);
    vector<pair<int, int>> ask;
    vector<long long> expected;
    for (auto id : ids)
    {
        ask.push_back(qs[id]);
        expected.push_back(want[id]);
    }
    // 重复、乱序、重叠、互不相交和空区间都必须保持原询问顺序。
    ask.push_back({0, n});
    expected.push_back(want[n]);
    auto before = ask;
    auto copy = a;
    check(rangeInv(a, ask) == expected);
    check(a == copy && ask == before);
    check(rangeInv(a, {}).empty());
}

signed main()
{
    for (int seed = 0; seed < 16; seed++)
    {
        mt19937_64 rng(seed);
        for (int n : {0, 1, 2, 3, 7, 8, 15, 16, 31, 32, 63})
        {
            vector<int> a(n);
            iota(a.begin(), a.end(), -n);
            verify(a, rng);
            reverse(a.begin(), a.end());
            verify(a, rng);
            fill(a.begin(), a.end(), -7);
            verify(a, rng);
            for (auto &x : a)
            {
                x = (int)(rng() % 9) - 4;
            }
            verify(a, rng);
        }
        verify({numeric_limits<int>::min(), numeric_limits<int>::max(), 0,
                numeric_limits<int>::min(), numeric_limits<int>::max(), -1}, rng);
        if (!seed)
        {
            // 70000 的严格逆序序列有 2449965000 对，超过 32 位有符号整数上界。
            int n = 70000;
            vector<int> a(n);
            iota(a.rbegin(), a.rend(), 0);
            vector<pair<int, int>> qs{{0, n}, {123, n}, {n, n}, {0, 1}, {0, n}};
            auto f = [](long long k)
            {
                return k * (k - 1) / 2;
            };
            check((rangeInv(a, qs) == vector<long long>{f(n), f(n - 123), 0, 0, f(n)}));
            fill(a.begin(), a.end(), numeric_limits<int>::min());
            check((rangeInv(a, qs) == vector<long long>(qs.size())));
            check((rangeInv({}, {{0, 0}, {0, 0}}) == vector<long long>{0, 0}));
        }
        cout << "AC " << seed << '\n';
    }
}
