#pragma once

#include "10_2_003.hpp"

pair<Poly, Poly> polyDivmod(Poly a, Poly b)
{
    // a 是被除式，b 是非零除式；返回 {商,余式}，余式次数严格小于 b。
    a.norm();
    b.norm();
    assert(!b.empty()); // 调试检查，可删。
    if (a.size() < b.size())
    {
        return {{}, a};
    }
    int n = a.size() - b.size() + 1;
    if (a.size() <= 256 || b.size() <= 256)
    {
        Poly q(n);
        Z iv = b.back().inv();
        for (int i = n - 1; i >= 0; i--)
        {
            q[i] = a[i + b.size() - 1] * iv;
            for (int j = 0; j < (int)b.size(); j++)
            {
                a[i + j] -= q[i] * b[j];
            }
        }
        a.resize(b.size() - 1);
        q.norm();
        a.norm();
        return {move(q), move(a)};
    }
    assert(convFit(n, n) && convFit(b.size(), n)); // 调试检查，可删。
    Poly x = a;
    Poly y = b;
    reverse(x.begin(), x.end());
    reverse(y.begin(), y.end());
    Poly q = (x.cut(n) * fpsInvFast(y, n)).cut(n);
    reverse(q.begin(), q.end());
    Poly r = a - b * q;
    r.resize(b.size() - 1);
    q.norm();
    r.norm();
    return {q, r};
}
