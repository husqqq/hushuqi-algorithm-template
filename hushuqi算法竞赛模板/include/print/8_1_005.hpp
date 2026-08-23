#pragma once

#include "8_1_004.hpp"

int binomAnyMod(int n, int k, int mod)
{
    // n、k 是组合数参数，mod 是正模数且每个质数幂分量不超过 5e6；返回 C(n,k) mod mod。
    return BinomMod(mod).C(n, k);
}
