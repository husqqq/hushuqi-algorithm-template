#pragma once

#include "13_1_001.hpp"

// 知识点 13.1.006：Fibonacci 取石的最优首取量。
namespace fib_game
{
int zeckMin(int n)
{
    // n 是正石子数；返回 Zeckendorf 分解中的最小 Fibonacci 项。
    assert(n > 0); // 调试检查，可删。
    vector<int> f = {1, 2};
    while (f.back() <= n - f[f.size() - 2])
    {
        f.push_back(f.back() + f[f.size() - 2]);
    }
    int last = 0;
    for (auto it = f.rbegin(); it != f.rend(); ++it)
    {
        if (*it <= n)
        {
            n -= *it;
            last = *it;
        }
    }
    return last;
}
}
