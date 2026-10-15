#pragma once

#include "13_1_001.hpp"

// 知识点 13.1.005：Wythoff 两堆取石博弈。
namespace wythoff_game
{
bool firstWins(int a, int b)
{
    // a、b 是非负石子数；返回先手是否必胜。
    assert(a >= 0 && b >= 0); // 调试检查，可删。
    if (a > b)
    {
        swap(a, b);
    }
    int k = b - a;
    __int128 x = a;
    __int128 d = k;
    __int128 z = d * d;
    bool cold = x * x - x * d <= z
                && z < (x + 1) * (x + 1) - (x + 1) * d;
    return !cold;
}
}
