#pragma once
#include <bits/stdc++.h>
using namespace std;

inline optional<vector<long double>> cubicRoots(long double a, long double b,
                                                 long double c, long double d)
{
    // a、b、c、d 是有限实系数；返回升序互异实根，恒等式返回空 optional，无实根返回存在的空向量。
    constexpr long double eps = 1E-12L;
    // 调试检查，可删。
    assert(isfinite(a) && isfinite(b) && isfinite(c) && isfinite(d));
    bool hasA = a != 0;
    bool hasB = b != 0;
    bool hasC = c != 0;
    long double sc = max({fabsl(a), fabsl(b), fabsl(c), fabsl(d)});
    if (sc == 0)
    {
        return nullopt;
    }
    a /= sc;
    b /= sc;
    c /= sc;
    d /= sc;
    assert((!hasA || a != 0) && (!hasB || b != 0) && (!hasC || c != 0)); // 调试检查，可删。
    if (!hasA)
    {
        if (!hasB)
        {
            if (!hasC)
            {
                return vector<long double>{};
            }
            return vector<long double>{-d / c};
        }
        long double det = c * c - 4 * b * d;
        if (det < -eps)
        {
            return vector<long double>{};
        }
        det = max(0.L, det);
        if (det == 0)
        {
            return vector<long double>{-c / (2 * b)};
        }
        long double s = sqrtl(det);
        long double q = -0.5L * (c + copysignl(s, c));
        vector<long double> ans{q / b, d / q};
        sort(ans.begin(), ans.end());
        if (ans[0] == ans[1])
        {
            ans.pop_back();
        }
        return ans;
    }
    auto f = [&](long double x)
    {
        return ((a * x + b) * x + c) * x + d;
    };
    long double lim = 1 + max({fabsl(b), fabsl(c), fabsl(d)}) / fabsl(a);
    vector<long double> key{-lim, lim};
    long double det = b * b - 3 * a * c;
    if (det >= -eps)
    {
        det = sqrtl(max(0.L, det));
        long double x = (-b - det) / (3 * a);
        long double y = (-b + det) / (3 * a);
        if (-lim < x && x < lim)
        {
            key.push_back(x);
        }
        if (-lim < y && y < lim)
        {
            key.push_back(y);
        }
    }
    sort(key.begin(), key.end());
    vector<long double> ans;
    for (auto x : key)
    {
        if (fabsl(f(x)) <= eps)
        {
            ans.push_back(x);
        }
    }
    for (int i = 0; i + 1 < (int)key.size(); i++)
    {
        long double l = key[i];
        long double r = key[i + 1];
        long double fl = f(l);
        long double fr = f(r);
        if (fl == 0 || fr == 0 || (fl < 0) == (fr < 0))
        {
            continue;
        }
        for (int t = 0; t < 100; t++)
        {
            long double m = (l + r) / 2;
            long double fm = f(m);
            if ((fl < 0) == (fm < 0))
            {
                l = m;
                fl = fm;
            }
            else
            {
                r = m;
                fr = fm;
            }
        }
        ans.push_back((l + r) / 2);
    }
    sort(ans.begin(), ans.end());
    ans.erase(unique(ans.begin(), ans.end(), [&](auto x, auto y)
                     {
                         return fabsl(x - y) <= 100 * eps * max({1.L, fabsl(x), fabsl(y)});
                     }),
              ans.end());
    return ans;
}
