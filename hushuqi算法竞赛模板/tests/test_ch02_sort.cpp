#include <bits/stdc++.h>
using namespace std;
#include "../include/sort_sequence.hpp"
#undef int
namespace topic
{
#include "../include/full/2_2_001.hpp"
}
#undef int

template <class Sort> void checkMerge(Sort sort)
{
    // sort 接收数组与比较器；检查空输入、等价键稳定性、无默认构造和只能移动的对象。
    vector<string> empty;
    sort(empty, less<>{});
    assert(empty.empty());
    vector<string> words{"bbb", "a", "cc", "dd", "e"};
    sort(words, [](const auto &x, const auto &y) { return x.size() < y.size(); });
    assert((words == vector<string>{"a", "e", "cc", "dd", "bbb"}));
    vector<unique_ptr<long long>> ptr;
    for (auto x : {3LL, 1LL, 3LL, -2LL})
    {
        ptr.push_back(make_unique<long long>(x));
    }
    auto first = ptr[0].get(), second = ptr[2].get();
    sort(ptr, [](const auto &x, const auto &y) { return *x > *y; });
    assert(ptr[0].get() == first && ptr[1].get() == second);
    assert(*ptr[2] == 1 && *ptr[3] == -2);
    struct Item
    {
        long long key;
        Item() = delete;
        explicit Item(long long x) : key(x) {}
    };
    vector<Item> items;
    items.emplace_back(2);
    items.emplace_back(1);
    sort(items, [](const auto &x, const auto &y) { return x.key < y.key; });
    assert(items[0].key == 1 && items[1].key == 2);
    for (int seed = 0; seed < 200; seed++)
    {
        mt19937_64 rng(20261017 + seed);
        vector<array<long long, 2>> a(rng() % 301);
        for (int i = 0; i < (int)a.size(); i++)
        {
            a[i] = {(long long)(rng() % 11) - 5, i};
        }
        auto cmp = [=](const auto &x, const auto &y)
        {
            return seed % 2 ? x[0] < y[0] : x[0] > y[0];
        };
        auto want = a;
        stable_sort(want.begin(), want.end(), cmp);
        sort(a, cmp);
        if (a != want)
        {
            cerr << "WA " << seed << '\n';
            abort();
        }
        cout << "AC " << seed << '\n';
    }
}

template <class T, class Sort> void checkRadix(Sort sort)
{
    // sort 按自然升序排序 T 数组；检查极值、重复值、反序、位段边界及固定种子对拍。
    vector<T> a;
    sort(a);
    assert(a.empty());
    for (auto x : {numeric_limits<T>::lowest(), (T)0, numeric_limits<T>::max()})
    {
        a = {x};
        sort(a);
        assert(a[0] == x);
    }
    for (int seed = 0; seed < 100; seed++)
    {
        mt19937_64 rng(20261017 + seed);
        a = {numeric_limits<T>::max(), (T)0, numeric_limits<T>::lowest(), (T)0};
        if constexpr (is_signed_v<T>)
        {
            a.push_back((T)-1);
        }
        using U = make_unsigned_t<T>;
        for (int k = 0; k < numeric_limits<U>::digits; k++)
        {
            U bit = (U)1 << k;
            if (bit <= (U)numeric_limits<T>::max())
            {
                a.push_back((T)(bit - 1));
                a.push_back((T)bit);
            }
        }
        for (int i = 0; i < 300; i++)
        {
            U bits = (U)rng();
            a.push_back(bit_cast<T>(bits));
        }
        if (seed % 3 == 0)
        {
            fill(a.begin(), a.end(), (T)1);
        }
        else if (seed % 3 == 1)
        {
            ranges::sort(a, greater<>{});
        }
        auto want = a;
        stable_sort(want.begin(), want.end());
        sort(a);
        if (a != want)
        {
            cerr << "WA " << seed << '\n';
            abort();
        }
        cout << "AC " << seed << '\n';
    }
}

template <class Sort> void checkTypes(Sort sort)
{
    // sort 是基数排序入口；覆盖 8/16/32/64 位的标准有符号与无符号整数。
    checkRadix<signed char>(sort);
    checkRadix<unsigned char>(sort);
    checkRadix<short>(sort);
    checkRadix<unsigned short>(sort);
    checkRadix<signed>(sort);
    checkRadix<unsigned>(sort);
    checkRadix<long long>(sort);
    checkRadix<unsigned long long>(sort);
}

signed main()
{
    checkMerge([](auto &a, auto cmp) { mergeSort(a, cmp); });
    checkMerge([](auto &a, auto cmp) { topic::mergeSort(a, cmp); });
    checkTypes([](auto &a) { radixSort(a); });
    checkTypes([](auto &a) { topic::radixSort(a); });
}
