#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr long double pi = 3.141592653589793238462643383279502884L;
constexpr long double e = 2.718281828459045235360287471352662498L;
constexpr long double phi = 1.618033988749894848204586834365638118L;
// ntt[i] 三维依次是质数、原根、可用的最大二次幂指数。
// 最大模数超过有符号 32 位，显式加宽以支持关闭 int 宏。
constexpr array<array<long long, 3>, 7> ntt = {{{998244353, 3, 23},
                                          {1004535809, 3, 21},
                                          {469762049, 3, 26},
                                          {167772161, 3, 25},
                                          {1224736769, 3, 24},
                                          {2013265921, 31, 27},
                                          {2281701377, 3, 27}}};
