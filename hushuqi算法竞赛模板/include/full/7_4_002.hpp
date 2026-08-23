#pragma once
#include "7_4_001.hpp"

optional<int> PH(int g, int h, int p)
{
    // g 是模素数 p 的原根，h 是非零剩余；返回最小非负离散对数。
    assert(p >= 2); // 调试检查，可删
    int left = p - 1;
    int x = 0;
    int curMod = 1;
    vector<int> fac;
    for (int q = 2; q <= left / q; q++)
    {
        if (left % q)
        {
            continue;
        }
        int pe = 1;
        while (left % q == 0)
        {
            left /= q;
            pe *= q;
        }
        fac.push_back(pe);
    }
    if (left > 1)
    {
        fac.push_back(left);
    }
    for (int pe : fac)
    {
        int a = powerMod(g, (p - 1) / pe, p);
        int b = powerMod(h, (p - 1) / pe, p);
        auto y = bsgs(a, b, p, pe);
        if (!y)
        {
            return nullopt;
        }
        int t = (*y - x) % pe;
        if (t < 0)
        {
            t += pe;
        }
        auto inv = invMod(curMod % pe, pe);
        if (!inv)
        {
            return nullopt;
        }
        t = mulMod(t, *inv, pe);
        x += curMod * t;
        curMod *= pe;
        x %= curMod;
    }
    h %= p;
    if (h < 0)
    {
        h += p;
    }
    return powerMod(g, x, p) == h ? optional<int>(x) : nullopt;
}
