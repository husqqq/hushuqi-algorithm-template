#pragma once

#include "10_1_003.hpp"
#include "7_4_003.hpp"

vector<Z> mulPrimeConv(int p, const vector<Z> &a, const vector<Z> &b)
{
    // p 是不超过 NTT 长度限制的素数，a、b 按 0..p-1 编号；返回 c[k]=sum_{ij=k(mod p)}a[i]b[j]。
    assert(p >= 2 && (int)a.size() == p && (int)b.size() == p);
    if (p == 2)
    {
        return {a[0] * b[0] + a[0] * b[1] + a[1] * b[0], a[1] * b[1]};
    }
    int g = primRoot(p);
    vector<int> pw(p - 1), lg(p);
    for (int i = 0, x = 1; i < p - 1; i++)
    {
        pw[i] = x;
        lg[x] = i;
        x = x * g % p;
    }
    vector<Z> x(p - 1), y(p - 1);
    for (int i = 1; i < p; i++)
    {
        x[lg[i]] = a[i];
        y[lg[i]] = b[i];
    }
    auto z = conv(x, y);
    for (int i = p - 1; i < (int)z.size(); i++)
    {
        z[i - (p - 1)] += z[i];
    }
    z.resize(p - 1);
    vector<Z> ans(p);
    Z sa = 0, sb = 0;
    for (int i = 0; i < p; i++)
    {
        sa += a[i];
        sb += b[i];
    }
    ans[0] = sa * b[0] + (sb - b[0]) * a[0];
    for (int i = 0; i < p - 1; i++)
    {
        ans[pw[i]] = z[i];
    }
    return ans;
}
