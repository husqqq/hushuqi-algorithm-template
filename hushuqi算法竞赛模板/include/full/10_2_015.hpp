#pragma once

#include "7_1_001.hpp"
#include "10_2_020.hpp"

vector<pair<Poly, int>> squareFree(Poly f)
{
    // f 是长度不超过 2^22 的非零多项式；返回全部首一平方自由因子及其重数。
    f = polyMonic(move(f));
    assert(!f.empty() && (int)f.size() <= (1 << 22));
    if (f.size() == 1)
    {
        return {};
    }
    Poly d(f.size() - 1);
    for (int i = 1; i < (int)f.size(); i++)
    {
        d[i - 1] = f[i] * i;
    }
    Poly c = polyGcd(f, d);
    Poly w = polyDivmod(f, c).first;
    vector<pair<Poly, int>> ans;
    for (int k = 1; w.size() > 1; k++)
    {
        Poly y = polyGcd(w, c);
        Poly z = polyDivmod(w, y).first;
        if (z.size() > 1)
        {
            ans.push_back({polyMonic(move(z)), k});
        }
        w = move(y);
        c = polyDivmod(c, w).first;
    }
    return ans;
}

vector<pair<Poly, int>> distinctDeg(Poly f)
{
    // f 是首一平方自由多项式；返回 {同次数不可约因子乘积,不可约因子次数}。
    assert(!f.empty() && (int)f.size() <= (1 << 22)); // 调试检查，可删。
    vector<pair<Poly, int>> ans;
    Poly x{0, 1}, h = polyMod(x, f);
    for (int d = 1; 2 * d <= (int)f.size() - 1; d++)
    {
        h = polyPowMod(h, Z::mod(), f);
        Poly g = polyGcd(f, h - x);
        if (g.size() > 1)
        {
            ans.push_back({g, d});
            f = polyDivmod(f, g).first;
            if (f.size() == 1)
            {
                break;
            }
            h = polyMod(move(h), f);
        }
    }
    if (f.size() > 1)
    {
        ans.push_back({f, (int)f.size() - 1});
    }
    return ans;
}

void splitEqual(Poly f, int d, mt19937_64 &rng, vector<Poly> &out)
{
    // f 的不可约因子次数均为 d，rng 是随机源，out 接收结果；递归加入全部首一不可约因子，无返回值。
    if ((int)f.size() - 1 == d)
    {
        out.push_back(polyMonic(move(f)));
        return;
    }
    while (true)
    {
        Poly a(f.size() - 1);
        for (auto &x : a)
        {
            x = rng() % Z::mod();
        }
        Poly b{1}, y = move(a);
        for (int i = 0; i < d; i++)
        {
            b = polyMod(b * polyPowMod(y, (Z::mod() - 1) / 2, f), f);
            y = polyPowMod(move(y), Z::mod(), f);
        }
        b -= Poly{1};
        Poly g = polyGcd(f, b);
        if (g.size() == 1 || g.size() == f.size())
        {
            continue;
        }
        Poly q = polyDivmod(move(f), g).first;
        splitEqual(move(g), d, rng, out);
        splitEqual(move(q), d, rng, out);
        return;
    }
}

vector<pair<Poly, int>> factorPoly(Poly f, unsigned long long seed = 0)
{
    // f 是长度不超过 2^22 的非零多项式，seed 是随机种子；返回 F_mod 上全部首一不可约因子及重数。
    f.norm();
    assert(!f.empty() && (int)f.size() <= (1 << 22));
    mt19937_64 rng(seed);
    vector<pair<Poly, int>> ans;
    auto sf = squareFree(polyMonic(move(f)));
    for (auto &[g, k] : sf)
    {
        auto dd = distinctDeg(move(g));
        for (auto &[h, d] : dd)
        {
            vector<Poly> part;
            splitEqual(move(h), d, rng, part);
            for (auto &q : part)
            {
                ans.push_back({move(q), k});
            }
        }
    }
    sort(ans.begin(), ans.end(), [](const auto &a, const auto &b)
    {
        if (a.first.size() != b.first.size())
        {
            return a.first.size() < b.first.size();
        }
        for (int i = a.first.size() - 1; i >= 0; i--)
        {
            if (a.first[i] != b.first[i])
            {
                return a.first[i].val() < b.first[i].val();
            }
        }
        return a.second < b.second;
    });
    return ans;
}

vector<Z> finiteRoots(const Poly &f, unsigned long long seed = 0)
{
    // f 是非零多项式；用完整因式分解返回所有有限根，根按代数重数重复。
    vector<Z> ans;
    for (auto [g, k] : factorPoly(f, seed))
    {
        if (g.size() == 2)
        {
            Z x = -g[0] / g[1];
            for (int i = 0; i < k; i++)
            {
                ans.push_back(x);
            }
        }
    }
    sort(ans.begin(), ans.end(), [](Z a, Z b) { return a.val() < b.val(); });
    return ans;
}

void splitRoots(Poly f, mt19937_64 &rng, vector<Z> &ans)
{
    // f 是首一且全部因子互异的一次因子积；递归把所有不同根加入 ans。
    int n = f.size() - 1;
    if (n == 1)
    {
        ans.push_back(-f[0]);
        return;
    }
    while (true)
    {
        Poly a(n);
        for (auto &x : a)
        {
            x = rng() % mod;
        }
        Poly b = polyPowMod(move(a), (mod - 1) / 2, f);
        Poly g = polyGcd(f, b - Poly{1});
        if (g.size() == 1 || g.size() == f.size())
        {
            g = polyGcd(f, b);
        }
        if (g.size() == 1 || g.size() == f.size())
        {
            continue;
        }
        Poly q = polyDivmod(move(f), g).first;
        splitRoots(move(g), rng, ans);
        splitRoots(move(q), rng, ans);
        return;
    }
}

vector<Z> distinctRoot(Poly f, unsigned long long seed = 0)
{
    // f 是长度不超过 2^22 的非零多项式；返回 F_p 中所有不同有限根，按代表元升序排列且不重复。
    f.norm();
    assert(!f.empty() && (int)f.size() <= (1 << 22)); // 调试检查，可删。
    if (f.size() == 1)
    {
        return {};
    }
    Poly x{0, 1};
    Poly h = polyPowMod(x, mod, f);
    Poly g = polyGcd(f, h - x);
    if (g.size() == 1)
    {
        return {};
    }
    mt19937_64 rng(seed);
    vector<Z> ans;
    splitRoots(polyMonic(move(g)), rng, ans);
    sort(ans.begin(), ans.end(), [](Z a, Z b)
    {
        return a.val() < b.val();
    });
    ans.erase(unique(ans.begin(), ans.end()), ans.end());
    return ans;
}

namespace dfac
{
using DPoly = vector<int>;

// 公共 Z/NTT 固定模数，不适用于这里的运行时素数；次数至多百级，故保留朴素运算。
// p 是当前模数，state 是确定性随机状态；公开入口会重置二者，因此本实现不可重入。
int p;
uint64_t state;

int norm(int x)
{
    // x 是任意整数；返回其模 p 的最小非负代表。
    x %= p;
    if (x < 0)
    {
        x += p;
    }
    return x;
}

void trim(DPoly &a)
{
    // a 是低次在前的动态模多项式；原地删除末尾零系数。
    while (!a.empty() && a.back() == 0)
    {
        a.pop_back();
    }
}

int deg(const DPoly &a)
{
    // a 是动态模多项式；返回忽略末尾零项后的次数，零多项式返回 -1。
    int n = a.size();
    while (n && a[n - 1] == 0)
    {
        n--;
    }
    return n - 1;
}

DPoly monic(DPoly a)
{
    // a 是动态模多项式；返回首一化结果，零多项式仍为空。
    trim(a);
    if (a.empty())
    {
        return a;
    }
    int iv = powerMod(a.back(), p - 2, p);
    for (auto &x : a)
    {
        x = ::mulMod(x, iv, p);
    }
    return a;
}

DPoly add(DPoly a, const DPoly &b)
{
    // a、b 是动态模多项式；返回 a+b。
    a.resize(max(a.size(), b.size()));
    for (int i = 0; i < (int)b.size(); i++)
    {
        a[i] = a[i] >= p - b[i] ? a[i] - (p - b[i]) : a[i] + b[i];
    }
    trim(a);
    return a;
}

DPoly sub(DPoly a, const DPoly &b)
{
    // a、b 是动态模多项式；返回 a-b。
    a.resize(max(a.size(), b.size()));
    for (int i = 0; i < (int)b.size(); i++)
    {
        a[i] = a[i] >= b[i] ? a[i] - b[i] : p - (b[i] - a[i]);
    }
    trim(a);
    return a;
}

DPoly mul(const DPoly &a, const DPoly &b)
{
    // a、b 是动态模多项式；返回朴素乘积，适用于次数至多 100 的运行时模数题。
    if (a.empty() || b.empty())
    {
        return {};
    }
    DPoly c(a.size() + b.size() - 1);
    for (int i = 0; i < (int)a.size(); i++)
    {
        for (int j = 0; j < (int)b.size(); j++)
        {
            int v = ::mulMod(a[i], b[j], p);
            c[i + j] = c[i + j] >= p - v ? c[i + j] - (p - v) : c[i + j] + v;
        }
    }
    trim(c);
    return c;
}

pair<DPoly, DPoly> divmod(DPoly a, const DPoly &b)
{
    // a 是被除式，b 是非零动态模多项式；返回 {商,余式}。
    int n = deg(a), m = deg(b);
    assert(m >= 0); // 调试检查，可删。
    a.resize(n + 1);
    if (n < m)
    {
        return {{}, a};
    }
    DPoly q(n - m + 1);
    int iv = powerMod(b[m], p - 2, p);
    for (int i = n; i >= m; i--)
    {
        int x = ::mulMod(a[i], iv, p);
        q[i - m] = x;
        for (int j = 0; j <= m; j++)
        {
            int v = ::mulMod(x, b[j], p);
            int &y = a[i - m + j];
            y = y >= v ? y - v : p - (v - y);
        }
    }
    trim(q);
    trim(a);
    return {q, a};
}

DPoly gcd(DPoly a, DPoly b)
{
    // a、b 是动态模多项式；返回首一化 gcd。
    trim(a);
    trim(b);
    while (!b.empty())
    {
        DPoly r = divmod(move(a), b).second;
        a = move(b);
        b = move(r);
    }
    return monic(move(a));
}

DPoly mulMod(const DPoly &a, const DPoly &b, const DPoly &m)
{
    // a、b 是动态模多项式，m 是非零模多项式；返回 a*b mod m。
    return divmod(mul(a, b), m).second;
}

DPoly powMod(DPoly a, int e, const DPoly &m)
{
    // a 是动态模多项式，e 是非负指数，m 是非零模多项式；返回 a^e mod m。
    DPoly r = divmod({1}, m).second;
    a = divmod(move(a), m).second;
    while (e)
    {
        if (e & 1)
        {
            r = mulMod(r, a, m);
        }
        e >>= 1;
        if (e)
        {
            a = mulMod(a, a, m);
        }
    }
    return r;
}

uint64_t rnd()
{
    // 无参数；返回确定性 xorshift 伪随机数并更新 state。
    state ^= state << 7;
    state ^= state >> 9;
    return state;
}

DPoly rndPoly(int n)
{
    // n 是严格次数上界；返回次数小于 n 的随机动态模多项式。
    DPoly a(n);
    for (auto &x : a)
    {
        x = rnd() % p;
    }
    trim(a);
    return a;
}

DPoly edfOdd(const DPoly &a, int d, const DPoly &m)
{
    // a 是测试多项式，d 是不可约因子次数，m 是待拆多项式；返回奇素数域的 EDF 分裂值。
    DPoly x = powMod(a, (p - 1) / 2, m);
    DPoly r = divmod({1}, m).second;
    for (int i = 0; i < d; i++)
    {
        r = mulMod(r, x, m);
        if (i + 1 < d)
        {
            x = powMod(move(x), p, m);
        }
    }
    return r;
}

DPoly edfTwo(const DPoly &a, int d, const DPoly &m)
{
    // a 是测试多项式，d 是不可约因子次数，m 是待拆多项式；返回 F_2 的 EDF 迹值。
    DPoly x = divmod(a, m).second;
    DPoly r;
    for (int i = 0; i < d; i++)
    {
        r = add(move(r), x);
        if (i + 1 < d)
        {
            x = mulMod(x, x, m);
        }
    }
    return r;
}

void splitEq(DPoly f, int d, vector<DPoly> &ans)
{
    // f 是首一且全部不可约因子次数为 d 的多项式；递归把全部首一不可约因子加入 ans。
    int n = deg(f);
    if (n == d)
    {
        ans.push_back(move(f));
        return;
    }
    DPoly g;
    do
    {
        DPoly a = rndPoly(n);
        DPoly h = p == 2 ? edfTwo(a, d, f)
                          : sub(edfOdd(a, d, f), {1});
        g = gcd(f, move(h));
    } while (deg(g) <= 0 || deg(g) == n);
    DPoly q = divmod(move(f), g).first;
    splitEq(move(g), d, ans);
    splitEq(move(q), d, ans);
}

struct Fac
{
    // f 是首一不可约因子，e 是其代数重数。
    DPoly f;
    int e;
};

vector<Fac> factor(DPoly f)
{
    // f 是非零动态模多项式；返回全部首一不可约因子及其重数。
    f = monic(move(f));
    vector<Fac> ans;
    if (deg(f) <= 0)
    {
        return ans;
    }
    DPoly x{0, 1};
    DPoly h = x;
    for (int d = 1; deg(f) > 0 && 2 * d <= deg(f); d++)
    {
        h = powMod(move(h), p, f);
        DPoly g = gcd(f, sub(h, x));
        if (deg(g) <= 0)
        {
            continue;
        }
        vector<DPoly> part;
        splitEq(move(g), d, part);
        for (auto &q : part)
        {
            int e = 0;
            while (deg(f) >= deg(q))
            {
                auto [u, r] = divmod(f, q);
                if (!r.empty())
                {
                    break;
                }
                f = move(u);
                e++;
            }
            ans.push_back({move(q), e});
        }
    }
    if (deg(f) > 0)
    {
        ans.push_back({monic(move(f)), 1});
    }
    return ans;
}
}

struct DynFactor
{
    // f 是首一不可约因子，e 是其在输入多项式中的代数重数。
    vector<int> f;
    int e;
};

vector<DynFactor> factorPrime(vector<int> f, int p,
                                  uint64_t seed = 88172645463325252ULL)
{
    // f 是低次在前系数，p 是素数模数；返回 F_p 上的首一不可约分解及重数。
    assert(p >= 2); // 调试检查，可删。
    using namespace dfac;
    dfac::p = p;
    state = seed ? seed : 88172645463325252ULL;
    for (auto &x : f)
    {
        x = norm(x);
    }
    trim(f);
    assert(!f.empty()); // 调试检查，可删。
    auto part = factor(move(f));
    vector<DynFactor> ans;
    for (auto &x : part)
    {
        ans.push_back({move(x.f), x.e});
    }
    sort(ans.begin(), ans.end(), [](const auto &a, const auto &b)
    {
        if (a.f.size() != b.f.size())
        {
            return a.f.size() < b.f.size();
        }
        return a.f < b.f;
    });
    return ans;
}
