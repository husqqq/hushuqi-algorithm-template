#pragma once

#include "13_1_001.hpp"

bool stairNimWin(const vector<unsigned long long> &a)
{
    // 参数 a 表示 从第 0 层开始记录的各阶梯石子数；返回阶梯 Nim 局面的先手是否必胜。
    unsigned long long x = 0;
    for (int i = 1; i < (int)a.size(); i += 2)
    {
        x ^= a[i];
    }
    return x != 0;
}
