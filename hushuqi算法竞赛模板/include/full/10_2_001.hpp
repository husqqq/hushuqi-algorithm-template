#pragma once

#include "10_1_003.hpp"

struct Poly : vector<Z>
{
    using vector<Z>::vector;
    Poly(const vector<Z> &a = {}) : vector<Z>(a)
    {
        // a 按低次到高次给出系数；构造同系数多项式，无返回值。
    }

    Poly(vector<Z> &&a) : vector<Z>(move(a))
    {
        // a 按低次到高次给出系数；移动构造同系数多项式。
    }

    void norm()
    {
        // 无参数；删除末尾多余零系数，无返回值。
        while (!empty() && back() == Z(0))
        {
            pop_back();
        }
    }

    Poly cut(int n) const
    {
        // n 是非负截断长度；返回恰含前 n 项的形式幂级数。
        assert(n >= 0); // 调试检查，可删。
        Poly a(begin(), begin() + min(n, (int)size()));
        a.resize(n);
        return a;
    }

    Poly &operator+=(const Poly &b)
    {
        // b 是要相加的多项式；返回更新后的当前多项式。
        resize(max(size(), b.size()));
        for (int i = 0; i < (int)b.size(); i++)
        {
            (*this)[i] += b[i];
        }
        return *this;
    }

    Poly &operator-=(const Poly &b)
    {
        // b 是要减去的多项式；返回更新后的当前多项式。
        resize(max(size(), b.size()));
        for (int i = 0; i < (int)b.size(); i++)
        {
            (*this)[i] -= b[i];
        }
        return *this;
    }

    Poly &operator*=(const Poly &b)
    {
        // b 是乘数多项式；返回卷积后的当前多项式。
        if (this == &b)
        {
            Poly a = b;
            return *this = conv(move(a), move(*this));
        }
        return *this = conv(move(*this), b);
    }

    Poly &operator*=(Z k)
    {
        // k 是标量系数；返回所有系数乘 k 后的当前多项式。
        for (auto &x : *this)
        {
            x *= k;
        }
        return *this;
    }

    friend Poly operator+(Poly a, const Poly &b)
    {
        // a、b 是两个多项式；返回系数和。
        return a += b;
    }

    friend Poly operator-(Poly a, const Poly &b)
    {
        // a、b 是两个多项式；返回系数差。
        return a -= b;
    }

    friend Poly operator*(Poly a, const Poly &b)
    {
        // a、b 是两个多项式；返回多项式乘积。
        return a *= b;
    }

    friend Poly operator*(Poly a, Z k)
    {
        // a 是多项式，k 是标量；返回缩放后的多项式。
        return a *= k;
    }

    friend Poly operator*(Z k, Poly a)
    {
        // k 是标量，a 是多项式；返回缩放后的多项式。
        return a *= k;
    }
};

Poly midProd(const Poly &a, const Poly &b)
{
    // a、b 满足 a.size()>=b.size()>0；返回 c[i]=sum_j b[j]*a[i+j] 的中段乘积。
    assert(!b.empty() && a.size() >= b.size()); // 调试检查，可删。
    int m = a.size() - b.size() + 1;
    if (min((int)b.size(), m) <= 64)
    {
        Poly c(m);
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < (int)b.size(); j++)
            {
                c[i] += b[j] * a[i + j];
            }
        }
        return c;
    }
    assert(nttFit(b.size(), m)); // 调试检查，可删。
    int n = bit_ceil((unsigned long long)a.size());
    Poly x = a;
    Poly y(b.rbegin(), b.rend());
    x.resize(n);
    y.resize(n);
    dft(x);
    dft(y);
    for (int i = 0; i < n; i++)
    {
        x[i] *= y[i];
    }
    idft(x);
    return Poly(x.begin() + b.size() - 1, x.begin() + a.size());
}
