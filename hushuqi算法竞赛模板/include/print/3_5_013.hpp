#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T> class RadixHeap
{
    using Key = unsigned long long; // 键依赖 64 位无符号异或与位宽。
    // b[i] 保存与 last 的最高不同位落在第 i 桶的键值对。
    array<vector<pair<Key, T>>, 65> b;
    // last 是最近弹出的键，n 是当前元素数。
    Key last = 0;
    int n = 0;

    int id(Key x) const
    {
        // x 是不小于 last 的键；返回它当前所属的桶编号。
        return x == last ? 0 : bit_width(x ^ last);
    }

    void pull()
    {
        // 无参数；把首个非空高桶按其中最小键重新分桶，使 0 号桶非空。
        if (!b[0].empty())
        {
            return;
        }
        int p = 1;
        while (b[p].empty())
        {
            p++;
        }
        last = min_element(b[p].begin(), b[p].end(),
                           [](const auto &x, const auto &y)
                           {
                               return x.first < y.first;
                           })->first;
        for (auto x : b[p])
        {
            b[id(x.first)].push_back(move(x));
        }
        b[p].clear();
    }

  public:
    bool empty() const
    {
        // 无参数；返回堆是否为空。
        return n == 0;
    }

    int size() const
    {
        // 无参数；返回当前元素数。
        return n;
    }

    void push(Key k, const T &v)
    {
        // k 是单调键，v 是关联值；插入一项，要求 k 不小于最近弹出的键。
        assert(k >= last); // 调试检查，可删
        b[id(k)].push_back({k, v});
        n++;
    }

    const pair<Key, T> &top()
    {
        // 无参数；返回键最小的一项，要求堆非空。
        assert(n > 0); // 调试检查，可删
        pull();
        return b[0].back();
    }

    pair<Key, T> pop()
    {
        // 无参数；删除并返回键最小的一项，要求堆非空。
        auto ans = top();
        b[0].pop_back();
        n--;
        return ans;
    }
};
