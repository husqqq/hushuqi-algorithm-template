#pragma once
#include "1_1_008.hpp"
using namespace std;

template <class T> T fieldPow(T a, unsigned long long b)
{
    // a 是域元素，b 是非负指数；返回 a 的 b 次幂。
    T ans = 1;
    while (b)
    {
        if (b & 1)
        {
            ans *= a;
        }
        b >>= 1;
        if (b)
        {
            a *= a;
        }
    }
    return ans;
}

template <class T> T interpIota(const vector<T> &y, long long x)
{
    // y[i] 是多项式在 i 处的值，x 是非负查询点；返回该多项式在 x 处的值。
    int n = y.size();
    // 调试检查，可删。
    assert(n > 0 && x >= 0);
    if (x < n)
    {
        return y[x];
    }
    vector<T> fac(n, T(1)), ifac(n, T(1));
    vector<T> pre(n + 1, T(1)), suf(n + 1, T(1));
    for (int i = 1; i < n; i++)
    {
        fac[i] = fac[i - 1] * T(i);
    }
    ifac[n - 1] = T(1) / fac[n - 1];
    for (int i = n - 1; i; i--)
    {
        ifac[i - 1] = ifac[i] * T(i);
    }
    for (int i = 0; i < n; i++)
    {
        pre[i + 1] = pre[i] * T(x - i);
    }
    for (int i = n - 1; i >= 0; i--)
    {
        suf[i] = suf[i + 1] * T(x - i);
    }
    T ans = 0;
    for (int i = 0; i < n; i++)
    {
        T v = y[i] * pre[i] * suf[i + 1] * ifac[i] * ifac[n - 1 - i];
        ans += (n - 1 - i) & 1 ? -v : v;
    }
    return ans;
}

template <class T> T sumExpPLim(T r, vector<T> f)
{
    // f[i] 是多项式在 i 处的值且 r!=1；返回生成函数给出的形式无穷和。
    // 调试检查，可删。
    assert(!f.empty() && r != T(1));
    T q = r / (T(1) - r);
    T term = T(1) / (T(1) - r);
    T ans = 0;
    for (int k = 0; k < (int)f.size(); k++)
    {
        ans += f[0] * term;
        for (int i = 0; i + 1 < (int)f.size() - k; i++)
        {
            f[i] = f[i + 1] - f[i];
        }
        term *= q;
    }
    return ans;
}

template <class T> T sumExpPoly(T r, const vector<T> &f, long long n)
{
    // f[i] 是多项式在 i 处的值，n 是项数；返回前 n 项的指数加权和。
    // 调试检查，可删。
    assert(!f.empty() && n >= 0);
    if (n == 0)
    {
        return T(0);
    }
    if (r == T(0))
    {
        return f[0];
    }
    int d = f.size() - 1;
    vector<T> s(d + 2);
    T pw = 1;
    for (int i = 0; i <= d; i++)
    {
        s[i + 1] = s[i] + pw * f[i];
        pw *= r;
    }
    if (r == T(1))
    {
        return interpIota(s, n);
    }
    T limit = sumExpPLim(r, f);
    T inv = T(1) / r;
    T ipw = 1;
    for (int i = 0; i <= d + 1; i++)
    {
        s[i] = (s[i] - limit) * ipw;
        ipw *= inv;
    }
    return limit + fieldPow(r, (unsigned long long)n) * interpIota(s, n);
}

template <class T> vector<T> monoValues(int d, int n)
{
    // d 是非负指数，n 是值数；返回 0^d 到 (n-1)^d。
    vector<int> lpf(n);
    for (int i = 2; i < n; i++) lpf[i] = i;
    for (int p = 2; p < n; p++) if (lpf[p] == p)
    {
        for (int x = 2 * p; x < n; x += p) if (lpf[x] > p) lpf[x] = p;
    }
    vector<T> f(n);
    for (int i = 0; i < n; i++)
    {
        if (i <= 1 || lpf[i] == i) f[i] = fieldPow(T(i), d);
        else f[i] = f[lpf[i]] * f[i / lpf[i]];
    }
    return f;
}

template <class T> T sumExpMLim(T r, int d)
{
    // r!=1，d 是非负整数；返回形式幂级数 sum r^i i^d，O(d)。
    assert(r != T(1) && d >= 0);
    auto f = monoValues<T>(d, d + 1);
    vector<T> fac(d + 2, T(1)), ifac(d + 2, T(1)), pw(d + 1, T(1));
    for (int i = 1; i <= d + 1; i++) fac[i] = fac[i - 1] * T(i);
    ifac[d + 1] = T(1) / fac[d + 1];
    for (int i = d + 1; i; i--) ifac[i - 1] = ifac[i] * T(i);
    for (int i = 1; i <= d; i++) pw[i] = pw[i - 1] * r;
    T ans = 0, prefix = 0;
    for (int i = 0; i <= d; i++)
    {
        prefix += pw[i] * f[i];
        T term = ifac[d - i] * ifac[i + 1] * pw[d - i] * prefix;
        ans += (d - i) & 1 ? -term : term;
    }
    return ans * fac[d + 1] / fieldPow(T(1) - r, d + 1);
}

template <class T> T sumExpMono(T r, int d, long long n)
{
    // r 是域元素，d 是非负指数，n 是项数；返回 sum_{i=0}^{n-1} r^i i^d，O(d)。
    assert(d >= 0 && n >= 0);
    if (n == 0) return T(0);
    auto f = monoValues<T>(d, d + 1);
    if (r == T(0)) return f[0];
    vector<T> s(d + 2);
    T pw = 1;
    for (int i = 0; i <= d; i++)
    {
        s[i + 1] = s[i] + pw * f[i];
        pw *= r;
    }
    if (r == T(1)) return interpIota(s, n);
    T limit = sumExpMLim(r, d), inv = T(1) / r, ipw = 1;
    for (int i = 0; i <= d + 1; i++)
    {
        s[i] = (s[i] - limit) * ipw;
        ipw *= inv;
    }
    return limit + fieldPow(r, n) * interpIota(s, n);
}
