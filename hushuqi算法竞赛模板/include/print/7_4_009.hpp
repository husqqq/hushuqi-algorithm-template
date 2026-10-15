#pragma once
#include "7_2_003.hpp"

// 多项式系数低次在前；仅提升简单根，p 为素数，目标 p^k <= LLONG_MAX。
inline optional<long long> hensel(const vector<long long> &a, long long root,
                                   long long p, int k)
{
    if (p < 2 || !isPrime(p) || k < 1) throw invalid_argument("Hensel needs prime p and k>=1");
    auto eval = [&](long long x, long long mod, bool derivative)
    {
        __int128 s = 0;
        for (int i = a.size() - 1; i >= (derivative ? 1 : 0); --i)
        {
            __int128 c = derivative ? (__int128)a[i] * i : a[i];
            s = (s * x + c) % mod;
        }
        if (s < 0) s += mod; return (long long)s;
    };
    root %= p; if (root < 0) root += p;
    if (eval(root, p, false)) return nullopt;
    long long d = eval(root, p, true); if (!d) return nullopt;
    long long inv = pow64(d, p - 2, p), mod = p;
    for (int i = 1; i < k; ++i)
    {
        if ((__int128)mod * p > LLONG_MAX) return nullopt;
        long long next = mod * p, f = eval(root, next, false);
        long long t = (__int128)(p - f / mod) * inv % p;
        root = (__int128)root + (__int128)t * mod; mod = next;
    }
    return root;
}
