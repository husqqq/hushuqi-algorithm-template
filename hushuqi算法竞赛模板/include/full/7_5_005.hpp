#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<pair<char, long long>> sternPath(long long p, long long q)
{
    // p、q 是既约正分数的分子与分母；返回 L/R 分段路径编码。
    assert(p > 0 && q > 0 && gcd(p, q) == 1); // 调试检查，可删
    vector<pair<char, long long>> a;
    while (p != q)
    {
        char c;
        long long k;
        if (p < q)
        {
            c = 'L';
            k = (q - 1) / p;
            q -= k * p;
        }
        else
        {
            c = 'R';
            k = (p - 1) / q;
            p -= k * q;
        }
        a.push_back({c, k});
    }
    return a;
}

pair<long long, long long> sternValue(const vector<pair<char, long long>> &a)
{
    // a 是合法路径且结果可由 long long 表示；返回对应既约分数 {p,q}。
    long long p = 1;
    long long q = 1;
    for (auto it = a.rbegin(); it != a.rend(); it++)
    {
        auto [c, k] = *it;
        assert((c == 'L' || c == 'R') && k > 0); // 调试检查，可删
        if (c == 'L')
        {
            assert(k <= (numeric_limits<long long>::max() - q) / p); // 调试检查，可删
            q += k * p;
        }
        else
        {
            assert(k <= (numeric_limits<long long>::max() - p) / q); // 调试检查，可删
            p += k * q;
        }
    }
    return {p, q};
}

pair<long long, long long> sternDecode(const vector<pair<char, long long>> &a,
                                       long long p, long long q)
{
    // a 是合法分段路径；从初值 p/q 逆向解码。
    for (auto it = a.rbegin(); it != a.rend(); it++)
    {
        auto [c, k] = *it;
        if (c == 'L')
        {
            q += k * p;
        }
        else
        {
            p += k * q;
        }
    }
    return {p, q};
}

pair<pair<long long, long long>, pair<long long, long long>>
sternRange(long long p, long long q)
{
    auto path = sternPath(p, q);
    return {sternDecode(path, 0, 1), sternDecode(path, 1, 0)};
}

pair<pair<long long, long long>, pair<long long, long long>>
sternChild(long long p, long long q)
{
    auto path = sternPath(p, q);
    return {sternDecode(path, 1, 2), sternDecode(path, 2, 1)};
}

optional<pair<long long, long long>> sternAnc(long long k, long long p,
                                                   long long q)
{
    auto path = sternPath(p, q);
    vector<pair<char, long long>> prefix;
    for (auto [c, n] : path)
    {
        if (k <= n)
        {
            prefix.push_back({c, k});
            return sternValue(prefix);
        }
        prefix.push_back({c, n});
        k -= n;
    }
    return nullopt;
}

pair<long long, long long> sternLca(long long a, long long b,
                                    long long c, long long d)
{
    auto x = sternPath(a, b);
    auto y = sternPath(c, d);
    vector<pair<char, long long>> prefix;
    for (int i = 0; i < min(x.size(), y.size()); i++)
    {
        if (x[i].first != y[i].first)
        {
            break;
        }
        prefix.push_back({x[i].first, min(x[i].second, y[i].second)});
        if (x[i].second != y[i].second)
        {
            break;
        }
    }
    return sternValue(prefix);
}

pair<pair<long long, long long>, pair<long long, long long>>
ratApprox(long long n, long long x, long long y)
{
    // 返回分子分母均不超过 n 的最紧下界与上界，不存在时分别用 0/1、1/0。
    using F = pair<long long, long long>;
    F lo{0, 1}, hi{1, 0};
    auto add = [](F a, long long k, F b)
    {
        return F{a.first * k + b.first, a.second * k + b.second};
    };
    auto inside = [&](F a) { return a.first <= n && a.second <= n; };
    auto le = [](F a, F b)
    {
        return (__int128)a.first * b.second <= (__int128)a.second * b.first;
    };
    F target{x, y};
    while (true)
    {
        auto maximum = [&](auto pred)
        {
            long long ok = 0, ng = 1;
            while (pred(ng))
            {
                ng *= 2;
            }
            while (ng - ok > 1)
            {
                long long mid = midpoint(ok, ng);
                (pred(mid) ? ok : ng) = mid;
            }
            return ok;
        };
        long long kl = maximum([&](long long k)
        {
            F z = add(hi, k, lo);
            return inside(z) && le(z, target);
        });
        lo = add(hi, kl, lo);
        if ((__int128)lo.first * y == (__int128)lo.second * x)
        {
            hi = lo;
            break;
        }
        long long kr = maximum([&](long long k)
        {
            F z = add(lo, k, hi);
            return inside(z) && le(target, z);
        });
        hi = add(lo, kr, hi);
        if ((__int128)hi.first * y == (__int128)hi.second * x)
        {
            lo = hi;
            break;
        }
        if (kl == 0 && kr == 0)
        {
            break;
        }
    }
    return {lo, hi};
}

vector<pair<int, int>> farey(int n)
{
    // n 是正阶数；返回按值递增的 n 阶 Farey 序列。
    assert(n >= 1); // 调试检查，可删
    vector<pair<int, int>> a{{0, 1}};
    int p = 0;
    int q = 1;
    int r = 1;
    int s = n;
    while (r <= n)
    {
        a.push_back({r, s});
        __int128 k = ((__int128)n + q) / s;
        int x = (int)(k * r - p);
        int y = (int)(k * s - q);
        p = r;
        q = s;
        r = x;
        s = y;
    }
    return a;
}
