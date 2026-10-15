#pragma once

#include "7_1_014.hpp"

struct PrimePrefix
{
    int x; // 商集中的 floor(n/i)。
    int cnt; // 不超过 x 的素数个数。
    __int128 sum; // 不超过 x 的素数和，初始化的三角和需要宽整数。
};

vector<PrimePrefix> lucyPrime(int n)
{
    // n 是非负上界；返回商集内的素数个数与素数和，按 x 升序；n=0 返回空表。
    assert(n >= 0);
    if (n == 0) return {};
    int rt = kthRoot(n, 2);
    vector<PrimePrefix> a;
    for (int l = 1; l <= n; )
    {
        int x = n / l;
        a.push_back({x, x - 1, (__int128)x * ((__int128)x + 1) / 2 - 1});
        if (x == 1) break;
        l = n / x + 1;
    }
    reverse(a.begin(), a.end());
    int m = a.size();
    auto id = [&](int x)
    {
        // x 必须属于 n 的商集；返回其在升序表中的下标。
        return x <= rt ? x - 1 : m - n / x;
    };
    for (int p = 2; p <= rt; p++)
    {
        if (a[id(p)].cnt == a[id(p - 1)].cnt) continue;
        auto pre = a[id(p - 1)];
        for (int i = m - 1; i >= 0 && a[i].x >= p * p; i--)
        {
            const auto &v = a[id(a[i].x / p)];
            a[i].cnt -= v.cnt - pre.cnt;
            a[i].sum -= (__int128)p * (v.sum - pre.sum);
        }
    }
    return a;
}
