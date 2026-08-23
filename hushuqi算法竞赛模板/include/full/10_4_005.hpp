#pragma once

#include "10_4_003.hpp"
#include "10_4_004.hpp"

pair<Poly, Poly> bmStep(Poly q)
{
    // q 的首尾系数非零；返回 {q(-x),v(x)}，其中 q(x)q(-x)=v(x^2)。
    assert(!q.empty() && q[0] != Z(0) && q.back() != Z(0)); // 调试检查，可删。
    Poly neg = q;
    for (int i = 1; i < (int)neg.size(); i += 2)
    {
        neg[i] = -neg[i];
    }
    Poly a = conv(move(q), neg);
    Poly nq((a.size() + 1) / 2);
    for (int i = 0; i < (int)nq.size(); i++)
    {
        nq[i] = a[2 * i];
    }
    return {move(neg), move(nq)};
}

Z bostanMori(Poly p, Poly q, unsigned long long n)
{
    // p、q 是分子分母且 q[0] 非零，n 是所求下标；返回 [x^n]p(x)/q(x)。
    p.norm();
    q.norm();
    assert(!q.empty() && q[0] != Z(0)); // 调试检查，可删。
    while (n)
    {
        auto [neg, nq] = bmStep(move(q));
        Poly a = conv(move(p), move(neg));
        Poly np;
        np.reserve((a.size() + 1) / 2);
        for (int i = n & 1; i < (int)a.size(); i += 2)
        {
            np.push_back(a[i]);
        }
        p = move(np);
        q = move(nq);
        n >>= 1;
    }
    return p.empty() ? Z(0) : p[0] / q[0];
}
