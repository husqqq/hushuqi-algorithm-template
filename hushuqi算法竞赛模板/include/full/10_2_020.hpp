#pragma once

#include "10_2_002.hpp"

Poly polyMonic(Poly a)
{
    // a 是输入多项式；返回删除高次零项后的首一化结果，零多项式返回空。
    a.norm();
    if (!a.empty())
    {
        a *= a.back().inv();
    }
    return a;
}

Poly polyMod(Poly a, const Poly &m)
{
    // a 是被除式，m 是非零模多项式；返回 a 除以 m 的余式。
    return polyDivmod(move(a), m).second;
}

Poly polyPowMod(Poly a, long long e, const Poly &m)
{
    // a 是底数多项式，e 是非负指数，m 是次数至少为 1 的模多项式；返回 a^e mod m。
    assert(e >= 0 && m.size() > 1); // 调试检查，可删。
    Poly r{1};
    a = polyMod(move(a), m);
    while (e)
    {
        if (e & 1)
        {
            r = polyMod(r * a, m);
        }
        e >>= 1;
        if (e)
        {
            a = polyMod(a * a, m);
        }
    }
    return r;
}

namespace hg
{
using Pair = array<Poly, 2>;
using Mat = array<Poly, 4>;

Poly add(Poly a, const Poly &b)
{
    // a、b 是两个多项式；返回去除高次零项后的 a+b。
    a += b;
    a.norm();
    return a;
}

Poly sub(Poly a, const Poly &b)
{
    // a、b 是两个多项式；返回去除高次零项后的 a-b。
    a -= b;
    a.norm();
    return a;
}

int mulLen(const Poly &a, const Poly &b)
{
    // a、b 是两个多项式；返回乘积系数数，任一多项式为零时返回 0。
    return a.empty() || b.empty() ? 0 : a.size() + b.size() - 1;
}

Mat mul(const Mat &a, const Mat &b)
{
    // a、b 是按行展开的 2×2 多项式矩阵；返回矩阵乘积。
    array<int, 4> s{
        max(mulLen(a[0], b[0]), mulLen(a[1], b[2])),
        max(mulLen(a[0], b[1]), mulLen(a[1], b[3])),
        max(mulLen(a[2], b[0]), mulLen(a[3], b[2])),
        max(mulLen(a[2], b[1]), mulLen(a[3], b[3]))}; // s[i] 是结果第 i 项在消去高次零前的长度。
    int n = bit_ceil((unsigned long long)max({1LL, s[0], s[1], s[2], s[3]}));
    bool zero = false; // zero 表示存在零输入，此时逐项卷积更适合稀疏矩阵。
    for (const auto &x : a)
    {
        zero |= x.empty();
    }
    for (const auto &x : b)
    {
        zero |= x.empty();
    }
    if (zero || n < 64)
    {
        return {add(a[0] * b[0], a[1] * b[2]),
                add(a[0] * b[1], a[1] * b[3]),
                add(a[2] * b[0], a[3] * b[2]),
                add(a[2] * b[1], a[3] * b[3])};
    }
    assert(n <= (1 << 23)); // 调试检查，可删。
    array<Poly, 8> f{a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3]};
    for (auto &x : f)
    {
        x.resize(n);
        dft(x);
    }
    Mat c{Poly(n), Poly(n), Poly(n), Poly(n)};
    for (int i = 0; i < n; i++)
    {
        c[0][i] = f[0][i] * f[4][i] + f[1][i] * f[6][i];
        c[1][i] = f[0][i] * f[5][i] + f[1][i] * f[7][i];
        c[2][i] = f[2][i] * f[4][i] + f[3][i] * f[6][i];
        c[3][i] = f[2][i] * f[5][i] + f[3][i] * f[7][i];
    }
    for (int i = 0; i < 4; i++)
    {
        idft(c[i]);
        c[i].resize(s[i]);
        c[i].norm();
    }
    return c;
}

Pair mul(const Mat &a, const Pair &b)
{
    // a 是按行展开的 2×2 多项式矩阵，b 是两维列向量；返回矩阵与列向量的乘积。
    int s0 = max(mulLen(a[0], b[0]), mulLen(a[1], b[1]));
    int s1 = max(mulLen(a[2], b[0]), mulLen(a[3], b[1]));
    int n = bit_ceil((unsigned long long)max({1LL, s0, s1}));
    bool zero = b[0].empty() || b[1].empty();
    for (const auto &x : a)
    {
        zero |= x.empty();
    }
    if (zero || n < 128)
    {
        return {add(a[0] * b[0], a[1] * b[1]),
                add(a[2] * b[0], a[3] * b[1])};
    }
    assert(n <= (1 << 23)); // 调试检查，可删。
    array<Poly, 6> f{a[0], a[1], a[2], a[3], b[0], b[1]};
    for (auto &x : f)
    {
        x.resize(n);
        dft(x);
    }
    Pair c{Poly(n), Poly(n)};
    for (int i = 0; i < n; i++)
    {
        c[0][i] = f[0][i] * f[4][i] + f[1][i] * f[5][i];
        c[1][i] = f[2][i] * f[4][i] + f[3][i] * f[5][i];
    }
    idft(c[0]);
    idft(c[1]);
    c[0].resize(s0);
    c[1].resize(s1);
    c[0].norm();
    c[1].norm();
    return c;
}

Mat id()
{
    // 无参数；返回 2×2 多项式单位矩阵。
    Mat a;
    a[0] = {1};
    a[3] = {1};
    return a;
}

Mat step(const Poly &q)
{
    // q 是一次欧几里得除法的商；返回把 (a,b) 变为 (b,a-qb) 的矩阵。
    Mat a;
    a[1] = {1};
    a[2] = {1};
    a[3] = sub({}, q);
    return a;
}

Mat stepMul(Mat a, const Poly &q)
{
    // a 是变换矩阵，q 是欧几里得商；返回 step(q)*a，并利用 step(q) 的稀疏结构。
    Poly x = q * a[2];
    Poly y = q * a[3];
    return {move(a[2]), move(a[3]),
            sub(move(a[0]), x), sub(move(a[1]), y)};
}

Mat halfSmall(Pair a)
{
    // a 是次数递减且第一项长度不超过 128 的余式对；用普通 Euclid 返回半降矩阵。
    int m = a[0].size() / 2;
    Mat ans = id();
    while (!a[1].empty() && (int)a[1].size() > m)
    {
        auto [q, r] = polyDivmod(move(a[0]), a[1]);
        ans = stepMul(move(ans), q);
        a = {move(a[1]), move(r)};
    }
    return ans;
}

Mat half(const Pair &a)
{
    // a[0]、a[1] 是次数递减的余式对；返回把第二项次数降到第一项一半以下的变换矩阵。
    int m = a[0].size() / 2;
    if ((int)a[1].size() <= m)
    {
        return id();
    }
    if ((int)a[0].size() <= 128)
    {
        return halfSmall(a);
    }
    Mat ans = half({Poly(a[0].begin() + m, a[0].end()),
                    Poly(a[1].begin() + m, a[1].end())});
    Pair b = mul(ans, a);
    if ((int)b[1].size() <= m)
    {
        return ans;
    }
    auto [q, r] = polyDivmod(move(b[0]), b[1]);
    ans = stepMul(move(ans), q);
    b = {move(b[1]), move(r)};
    if ((int)b[1].size() <= m)
    {
        return ans;
    }
    int cut = 2 * m + 1 - b[0].size();
    Mat suf = half({Poly(b[0].begin() + cut, b[0].end()),
                    Poly(b[1].begin() + cut, b[1].end())});
    return mul(suf, ans);
}

Mat all(const Pair &a)
{
    // a[0]、a[1] 是次数递减的余式对；返回把第二项降为零的完整变换矩阵。
    Mat ans = half(a);
    Pair b = mul(ans, a);
    if (b[1].empty())
    {
        return ans;
    }
    auto [q, r] = polyDivmod(move(b[0]), b[1]);
    ans = stepMul(move(ans), q);
    b = {move(b[1]), move(r)};
    if (b[1].empty())
    {
        return ans;
    }
    return mul(all(b), ans);
}
}

Poly polyGcd(Poly f, Poly g)
{
    // f、g 是不全为零的多项式；两者非空时长度均不超过 2^22，返回首一 gcd，不计算最终 Bézout 系数。
    f.norm();
    g.norm();
    assert(!f.empty() || !g.empty()); // 调试检查，可删。
    if (f.empty())
    {
        return polyMonic(move(g));
    }
    if (g.empty())
    {
        return polyMonic(move(f));
    }
    assert(max(f.size(), g.size()) <= (1 << 22)); // 调试检查，可删。
    if (f.size() < g.size())
    {
        swap(f, g);
    }
    hg::Pair a{move(f), move(g)};
    while (!a[1].empty())
    {
        if (a[1].size() < a[0].size() &&
            (int)a[1].size() > (int)a[0].size() / 2)
        {
            hg::Mat t = hg::half(a);
            a = hg::mul(t, a);
            if (a[1].empty())
            {
                break;
            }
        }
        Poly r = polyDivmod(move(a[0]), a[1]).second;
        a = {move(a[1]), move(r)};
    }
    return polyMonic(move(a[0]));
}

tuple<Poly, Poly, Poly> polyExtGcd(Poly f, Poly g)
{
    // f、g 是不全为零的多项式；两者非空时长度均不超过 2^22，返回 {d,x,y}，满足 d=xf+yg=gcd(f,g)，d 未首一化。
    f.norm();
    g.norm();
    assert(!f.empty() || !g.empty()); // 调试检查，可删。
    if (g.empty())
    {
        return {f, Poly{1}, Poly{}};
    }
    if (f.empty())
    {
        return {g, Poly{}, Poly{1}};
    }
    assert(max(f.size(), g.size()) <= (1 << 22)); // 调试检查，可删。
    if (f.size() < g.size())
    {
        auto [d, y, x] = polyExtGcd(move(g), move(f));
        return {move(d), move(x), move(y)};
    }
    using namespace hg;
    auto [q, r] = polyDivmod(f, g);
    Mat ans = step(q);
    Pair rem{g, move(r)};
    if (!rem[1].empty())
    {
        ans = mul(all(rem), ans);
    }
    Poly d = add(f * ans[0], g * ans[1]);
    return {move(d), move(ans[0]), move(ans[1])};
}

optional<Poly> polyInvMod(Poly f, Poly m)
{
    // f 是待求逆多项式，m 是非零模多项式；两者非空且 m 非常数时长度均不超过 2^22，可逆时返回次数小于 m 的逆元，否则返回空。
    f.norm();
    m.norm();
    assert(!m.empty()); // 调试检查，可删。
    if (m.size() == 1)
    {
        return Poly{};
    }
    if (!f.empty())
    {
        assert(max(f.size(), m.size()) <= (1 << 22)); // 调试检查，可删。
    }
    auto [d, x, y] = polyExtGcd(move(f), m);
    if (d.size() != 1)
    {
        return nullopt;
    }
    x *= d[0].inv();
    return polyDivmod(move(x), move(m)).second;
}
