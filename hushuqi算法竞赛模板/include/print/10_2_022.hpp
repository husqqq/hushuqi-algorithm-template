#pragma once
#include "10_2_005.hpp"

// 固定模 998244353，非负整数权；一次返回所有 0<=s<=T 的子集计数。
inline vector<Z> subsetCount(const vector<int> &a, int T)
{
    if (T < 0 || T >= (int)Z::mod() || T >= (1LL << 22)) throw invalid_argument("subsetCount: 0<=T<2^22 required");
    vector<int> cnt(T + 1); int zeros = 0;
    for (int x : a) { if (x < 0) throw invalid_argument("nonnegative weights required"); if (!x) ++zeros; else if (x <= T) ++cnt[x]; }
    vector<Z> inv(T + 1); if (T) inv[1] = 1;
    for (int i = 2; i <= T; ++i) inv[i] = -Z((int)Z::mod() / i) * inv[(int)Z::mod() % i];
    Poly logarithm(T + 1);
    for (int x = 1; x <= T; ++x) if (cnt[x])
        for (int k = 1; k <= T / x; ++k) logarithm[k * x] += Z(cnt[x]) * inv[k] * Z(k & 1 ? 1 : -1);
    Poly p = fpsExp(logarithm, T + 1); Z scale = Z(2).pow(zeros);
    for (auto &x : p) x *= scale; return p;
}
