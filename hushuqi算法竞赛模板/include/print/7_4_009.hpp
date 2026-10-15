#pragma once
#include "7_2_003.hpp"

optional<int> hensel(const vector<int> &a, int x, int p, int k)
{
    // a 是整系数多项式 f，低次在前；p 是素数，x 是 f 模 p 的根；p^k 不超过 LLONG_MAX。
    // 当 x 是单根（f'(x) 模 p 非零）时返回 f 模 p^k 的唯一提升根 (在 [0,p^k) 内)，否则返回空。
    assert(isPrime(p) && k >= 1); // 调试检查，可删。
    auto eval = [&](int y, int mod, bool der)
    {
        // 返回 f(y) 或 f'(y) (der 为真) 模 mod 的非负值，秦九韶求值。
        __int128 s = 0;
        for (int i = (int)a.size() - 1; i >= der; i--)
        {
            s = (s * y + (__int128)a[i] * (der ? i : 1)) % mod;
        }
        return (int)((s + mod) % mod);
    };
    x = (x % p + p) % p;
    int d = eval(x, p, true);
    if (eval(x, p, false) || !d)
    {
        return nullopt;
    }
    // 牛顿迭代：若 f(x)≡0 (mod q)，则 x - f(x)/f'(x) 是模 qp 的根，且 f'(x) 的逆只需模 p。
    int inv = pow64(d, p - 2, p), q = p;
    for (int i = 1; i < k; i++)
    {
        assert((__int128)q * p <= LLONG_MAX); // 调试检查，可删。
        int nq = q * p, f = eval(x, nq, false);
        int t = (__int128)(p - f / q) * inv % p;
        x += t * q;
        q = nq;
    }
    return x;
}
