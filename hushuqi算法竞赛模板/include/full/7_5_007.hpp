#pragma once
#include "7_1_014.hpp"
#include "7_2_004.hpp"
#include "7_4_004.hpp"

vector<pair<long long, long long>> circlePoints(long long n)
{
    // n 是非负半径平方；返回满足 x*x+y*y=n 的全部整数点，按字典序排列。
    assert(n >= 0); // 调试检查，可删
    set<pair<long long, long long>> s;
    for (long long x = 0; x == 0 || x <= n / x; x++)
    {
        long long y2 = n - x * x;
        long long y = kthRoot((unsigned long long)y2, 2);
        if (y * y != y2)
        {
            continue;
        }
        for (int sx : {-1, 1})
        {
            for (int sy : {-1, 1})
            {
                s.insert({sx * x, sy * y});
                s.insert({sx * y, sy * x});
            }
        }
    }
    return {s.begin(), s.end()};
}

vector<pair<long long, long long>> twoSqRep(long long n)
{
    // n 是非负整数；返回全部非负有序对 (x,y) 使 x^2+y^2=n。
    assert(n >= 0); // 调试检查，可删
    if (n == 0)
    {
        return {{0, 0}};
    }
    auto raw = factor64(n);
    vector<pair<long long, int>> factors;
    for (auto p : raw)
    {
        if (factors.empty() || factors.back().first != (long long)p)
        {
            factors.push_back({p, 1});
        }
        else
        {
            factors.back().second++;
        }
    }
    using G = pair<__int128, __int128>;
    auto mul = [](G a, G b)
    {
        return G{a.first * b.first - a.second * b.second,
                 a.first * b.second + a.second * b.first};
    };
    auto power = [&](G a, int e)
    {
        G r{1, 0};
        while (e)
        {
            if (e & 1)
            {
                r = mul(r, a);
            }
            a = mul(a, a);
            e >>= 1;
        }
        return r;
    };
    auto primeRep = [&](long long p)
    {
        if (p == 2)
        {
            return G{1, 1};
        }
        long long r = *sqrtMod(p - 1, p);
        long long a = p, b = r;
        while (b > p / b)
        {
            long long c = a % b;
            a = b;
            b = c;
        }
        long long y2 = p - b * b;
        long long y = kthRoot((unsigned long long)y2, 2);
        assert(y * y == y2);
        return G{b, y};
    };

    vector<G> reps{{1, 0}};
    for (auto [p, e] : factors)
    {
        if (p % 4 == 3)
        {
            if (e & 1)
            {
                return {};
            }
            __int128 scale = 1;
            for (int i = 0; i < e / 2; i++) scale *= p;
            for (auto &z : reps) z.first *= scale, z.second *= scale;
            continue;
        }
        G pi = primeRep(p);
        if (p == 2)
        {
            G z = power(pi, e);
            for (auto &v : reps) v = mul(v, z);
            continue;
        }
        vector<G> left(e + 1), right(e + 1);
        left[0] = right[0] = {1, 0};
        G conjugate{pi.first, -pi.second};
        for (int i = 0; i < e; i++)
        {
            left[i + 1] = mul(left[i], pi);
            right[i + 1] = mul(right[i], conjugate);
        }
        vector<G> next;
        for (G base : reps)
        {
            for (int j = 0; j <= e; j++)
            {
                next.push_back(mul(base, mul(left[j], right[e - j])));
            }
        }
        reps.swap(next);
    }
    set<pair<long long, long long>> answer;
    for (auto [x0, y0] : reps)
    {
        __int128 x = x0, y = y0;
        for (int i = 0; i < 4; i++)
        {
            __int128 nx = -y, ny = x;
            x = nx;
            y = ny;
            if (x >= 0 && y >= 0)
            {
                answer.insert({(long long)x, (long long)y});
            }
        }
    }
    return {answer.begin(), answer.end()};
}
