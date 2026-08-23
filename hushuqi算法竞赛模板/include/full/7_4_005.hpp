#pragma once
#include "7_4_001.hpp"
#include "7_4_003.hpp"

optional<int> kthRootBSGS(int a, int k, int p)
{
    // a 是目标剩余，k 是非负指数，p 是素数；返回任意一根，无解返回空。
    assert(k >= 0 && p >= 2); // 调试检查，可删
    a %= p;
    if (a < 0)
    {
        a += p;
    }
    if (k == 0)
    {
        return a == 1 ? optional<int>(0) : nullopt;
    }
    if (a == 0)
    {
        return 0;
    }
    int g = primRoot(p);
    auto b = bsgs(g, a, p);
    if (!b)
    {
        return nullopt;
    }
    int d = gcd(k, p - 1);
    if (*b % d)
    {
        return nullopt;
    }
    int mod = (p - 1) / d;
    if (mod == 1)
    {
        return 1;
    }
    auto inv = invMod(k / d, mod);
    if (!inv)
    {
        return nullopt;
    }
    int y = mulMod(*b / d, *inv, mod);
    return powerMod(g, y, p);
}

int ppRoot(int a, int prime, int exponent, int mod)
{
    int q = mod - 1;
    int ordExp = 0;
    while (q % prime == 0)
    {
        q /= prime;
        ordExp++;
    }
    int pe = powerMod(prime, exponent, mod);
    int iq = *invMod(q, pe);
    int ans = powerMod(a, ((pe - 1) * iq % pe * q + 1) / pe, mod);
    int c = 2;
    while (powerMod(c, (mod - 1) / prime, mod) == 1)
    {
        c++;
    }
    c = powerMod(c, q, mod);
    int width = sqrtl((long double)(ordExp - exponent) * prime) + 1;
    map<int, int> giant;
    int stepExp = width * powerMod(prime, ordExp - 1, mod - 1) % (mod - 1);
    int step = powerMod(c, stepExp, mod);
    int cur = 1;
    for (int i = 0; i <= width; i++)
    {
        giant[cur] = i;
        cur = mulMod(cur, step, mod);
    }
    int base = *invMod(powerMod(c, powerMod(prime, ordExp - 1, mod - 1), mod), mod);
    for (int i = exponent; i < ordExp; i++)
    {
        int err = mulMod(*invMod(powerMod(ans, pe, mod), mod), a, mod);
        int target = powerMod(err, powerMod(prime, ordExp - 1 - i, mod - 1), mod);
        for (int j = 0; j <= width; j++)
        {
            if (auto it = giant.find(target); it != giant.end())
            {
                int digit = j + width * it->second;
                int shift = digit * powerMod(prime, i - exponent, mod - 1) % (mod - 1);
                ans = mulMod(ans, powerMod(c, shift, mod), mod);
                break;
            }
            target = mulMod(target, base, mod);
            assert(j != width);
        }
    }
    return ans;
}

optional<int> kthResidue(int a, int k, int p)
{
    // p 是素数、k 非负；返回任意 x 使 x^k=a mod p，无解返回空。
    assert(k >= 0 && p >= 2); // 调试检查，可删
    a %= p;
    if (a < 0)
    {
        a += p;
    }
    if (k == 0)
    {
        return a == 1 ? optional<int>(0) : nullopt;
    }
    if (a == 0)
    {
        return 0;
    }
    k %= p - 1;
    int g = gcd(k, p - 1);
    if (powerMod(a, (p - 1) / g, p) != 1)
    {
        return nullopt;
    }
    int redMod = (p - 1) / g;
    if (redMod > 1)
    {
        a = powerMod(a, *invMod(k / g, redMod), p);
    }
    int left = g;
    for (int d = 2; d <= left / d; d++)
    {
        int exponent = 0;
        while (left % d == 0)
        {
            left /= d;
            exponent++;
        }
        if (exponent)
        {
            a = ppRoot(a, d, exponent, p);
        }
    }
    if (left > 1)
    {
        a = ppRoot(a, left, 1, p);
    }
    return a;
}
