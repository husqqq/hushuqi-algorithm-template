#pragma once

#include "10_2_001.hpp"

pair<Poly, Poly> recFraction(const vector<Z> &init, const vector<Z> &rec)
{
    // init 是至少 k 项初值，rec 定义 a[n]=sum(rec[i-1]*a[n-i])；返回生成函数 P(x)/Q(x)。
    int k = rec.size();
    assert(k > 0 && (int)init.size() >= k); // 调试检查，可删。
    Poly q(k + 1);
    q[0] = 1;
    for (int i = 0; i < k; i++)
    {
        q[i + 1] = -rec[i];
    }
    Poly a(init.begin(), init.begin() + k);
    Poly p = (a * q).cut(k);
    p.norm();
    return {p, q};
}
