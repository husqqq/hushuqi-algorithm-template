#pragma once

#include "13_1_001.hpp"

namespace wythoff_game
{
bool firstWins(int a, int b)
{
    assert(a >= 0 && b >= 0);
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
