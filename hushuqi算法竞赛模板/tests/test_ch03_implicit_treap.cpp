#include "../include/full/7_1_007.hpp"
#include "../include/full/3_5_011.hpp"

unsigned long long testExact()
{
    // 用普通 64 位整数随机核对插入、删除、翻转、仿射和区间和。
    mt19937_64 rng(0);
    unsigned long long acc = 0;
    for (int tc = 0; tc < 200; tc++)
    {
        vector<long long> a;
        ImpTreap<long long> tr;
        for (int q = 0; q < 100; q++)
        {
            int n = (int)a.size();
            int op = n ? rng() % 5 : 0;
            if (op == 0)
            {
                int k = rng() % (n + 1);
                long long x = (long long)(rng() % 101) - 50;
                tr.insert(k, x);
                a.insert(a.begin() + k, x);
            }
            else if (op == 1)
            {
                int k = rng() % n;
                tr.erase(k);
                a.erase(a.begin() + k);
            }
            else
            {
                int l = rng() % (n + 1), r = rng() % (n + 1);
                if (l > r)
                {
                    swap(l, r);
                }
                if (op == 2)
                {
                    tr.reverse(l, r);
                    reverse(a.begin() + l, a.begin() + r);
                }
                else if (op == 3)
                {
                    long long mul = (long long)(rng() % 3) - 1;
                    long long add = (long long)(rng() % 19) - 9;
                    tr.affine(l, r, mul, add);
                    for (int i = l; i < r; i++)
                    {
                        a[i] = a[i] * mul + add;
                    }
                }
                else
                {
                    long long want = 0;
                    for (int i = l; i < r; i++)
                    {
                        want += a[i];
                    }
                    assert(tr.sum(l, r) == want);
                    acc = acc * 1'000'003 + (unsigned long long)want;
                }
            }
            assert(tr.values() == a);
        }
    }
    return acc;
}

unsigned long long testMod()
{
    // 用 998244353 模整数随机核对仿射标记复合与动态序列操作。
    mt19937_64 rng(1);
    unsigned long long acc = 0;
    for (int tc = 0; tc < 200; tc++)
    {
        vector<Z> a;
        ImpTreap<Z> tr;
        for (int q = 0; q < 100; q++)
        {
            int n = (int)a.size();
            int op = n ? rng() % 5 : 0;
            if (op == 0)
            {
                int k = rng() % (n + 1);
                Z x((long long)(rng() % mod));
                tr.insert(k, x);
                a.insert(a.begin() + k, x);
            }
            else if (op == 1)
            {
                int k = rng() % n;
                tr.erase(k);
                a.erase(a.begin() + k);
            }
            else
            {
                int l = rng() % (n + 1), r = rng() % (n + 1);
                if (l > r)
                {
                    swap(l, r);
                }
                if (op == 2)
                {
                    tr.reverse(l, r);
                    reverse(a.begin() + l, a.begin() + r);
                }
                else if (op == 3)
                {
                    Z mul((long long)(rng() % mod));
                    Z add((long long)(rng() % mod));
                    tr.affine(l, r, mul, add);
                    for (int i = l; i < r; i++)
                    {
                        a[i] = a[i] * mul + add;
                    }
                }
                else
                {
                    Z want;
                    for (int i = l; i < r; i++)
                    {
                        want += a[i];
                    }
                    assert(tr.sum(l, r) == want);
                    acc = acc * 1'000'003 + want.val();
                }
            }
            assert(tr.values() == a);
        }
    }
    return acc;
}

void testLibraryCheckerSamples()
{
    // 按 dynamic_sequence_range_affine_range_sum 官方样例核对五类操作与模和。
    ImpTreap<Z> dynamic(vector<Z>{1, 10, 100, 1000, 10000}, 16);
    assert(dynamic.sum(1, 4) == Z(1110));
    dynamic.insert(2, Z(100000));
    dynamic.erase(3);
    assert(dynamic.sum(2, 5) == Z(111000));
    dynamic.reverse(2, 5);
    assert(dynamic.sum(0, 4) == Z(11011));

    // 按 range_reverse_range_sum 官方样例核对翻转、普通整数和与空区间。
    ImpTreap<long long> reverseSum(vector<long long>{1, 2, 3, 4}, 8);
    assert(reverseSum.sum(1, 3) == 5);
    reverseSum.reverse(2, 4);
    assert(reverseSum.sum(0, 3) == 7);
    reverseSum.reverse(4, 4);
    assert(reverseSum.sum(0, 0) == 0);
}

signed main()
{
    ImpTreap<long long> empty(8);
    assert(empty.empty() && empty.size() == 0 && empty.sum(0, 0) == 0);
    empty.reverse(0, 0);
    empty.affine(0, 0, 7, -3);
    empty.build({9, -3});
    assert(!empty.empty() && empty.values() == vector<long long>({9, -3}));

    ImpTreap<long long> fixed(vector<long long>{1, 2, 3, 4}, 16);
    fixed.reverse(1, 4);
    fixed.affine(1, 3, 2, 1);
    fixed.insert(2, 5);
    fixed.erase(1);
    assert(fixed.values() == vector<long long>({1, 5, 7, 2}));
    assert(fixed.sum(1, 4) == 14 && fixed.sum(2, 2) == 0);
    testLibraryCheckerSamples();
    cout << testExact() << ' ' << testMod() << '\n';
    return 0;
}
