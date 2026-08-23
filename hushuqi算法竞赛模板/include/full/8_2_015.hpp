#pragma once

#include "10_2_003.hpp"

vector<Z> stirling2K(int n, int k)
{
    // n 是最大行号，k 是固定列号；返回 S(k,k) 到 S(n,k)，模数由 Z 决定。
    assert(0 <= k && k <= n); // 调试检查，可删。
    auto build = [&](auto &&self, int l, int r) -> Poly
    {
        // self 是递归自身，[l,r) 是因子下标；返回这一段 (1-ix) 的乘积。
        if (r - l == 1)
        {
            return Poly{1, -Z(l)};
        }
        int m = (l + r) / 2;
        return self(self, l, m) * self(self, m, r);
    };
    Poly den = k == 0 ? Poly{1} : build(build, 1, k + 1);
    Poly ans = fpsInv(den, n - k + 1);
    return vector<Z>(ans.begin(), ans.end());
}
