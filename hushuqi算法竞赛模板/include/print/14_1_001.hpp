#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

struct BigInt
{
    // B 是每个低位块使用的进制 10^9。
    static constexpr int B = 1000000000;
    // a 按低位在前保存绝对值的 10^9 进制块。
    vector<int> a;
    // s 保存符号：-1、0、1 分别表示负数、零、正数。
    int s = 0;

    BigInt(long long x = 0)
    {
        // x 是待转换的 64 位整数；构造同值高精度整数。
        *this = x;
    }
    BigInt(const string &x)
    {
        // x 是带可选单个正负号的十进制整数串；构造同值高精度整数。
        assign(x);
    }

    BigInt &operator=(long long x)
    {
        // x 是待转换的 64 位整数；返回赋值后的当前对象。
        a.clear();
        s = (x > 0) - (x < 0);
        unsigned long long y = x < 0 ? (unsigned long long)(-(x + 1)) + 1 : (unsigned long long)x;
        while (y)
        {
            a.push_back(y % B);
            y /= B;
        }
        return *this;
    }

    BigInt &assign(const string &x)
    {
        // x 是带可选单个正负号且至少含一位数字的十进制整数串；返回赋值后的当前对象。
        a.clear();
        int p = 0;
        s = 1;
        if (!x.empty() && (x[0] == '-' || x[0] == '+'))
        {
            s = x[0] == '-' ? -1 : 1;
            p = 1;
        }
        if (p == (int)x.size())
        {
            throw invalid_argument("BigInt: empty digits");
        }
        for (int i = p; i < (int)x.size(); i++)
        {
            if (x[i] < '0' || x[i] > '9')
            {
                throw invalid_argument("BigInt: invalid digit");
            }
        }
        for (int r = (int)x.size(); r > p; r -= 9)
        {
            int l = max(p, r - 9);
            int v = 0;
            for (int i = l; i < r; i++)
            {
                v = v * 10 + x[i] - '0';
            }
            a.push_back(v);
        }
        norm();
        return *this;
    }

    string str() const
    {
        // 无参数；返回当前高精度整数的十进制字符串。
        if (!s)
        {
            return "0";
        }
        string x = s < 0 ? "-" : "";
        x += to_string(a.back());
        for (int i = (int)a.size() - 2; i >= 0; i--)
        {
            string t = to_string(a[i]);
            x += string(9 - t.size(), '0') + t;
        }
        return x;
    }

    void norm()
    {
        // 无参数；删除多项式末尾多余的零系数，无返回值。
        while (!a.empty() && !a.back())
        {
            a.pop_back();
        }
        if (a.empty())
        {
            s = 0;
        }
    }

    static int cmpAbs(const BigInt &x, const BigInt &y)
    {
        // x、y 是待比较整数；按 |x| 与 |y| 的大小返回 -1、0 或 1。
        if (x.a.size() != y.a.size())
        {
            return x.a.size() < y.a.size() ? -1 : 1;
        }
        for (int i = (int)(x.a.size()) - 1; i >= 0; i--)
        {
            if (x.a[i] != y.a[i])
            {
                return x.a[i] < y.a[i] ? -1 : 1;
            }
        }
        return 0;
    }

    friend bool operator==(const BigInt &x, const BigInt &y)
    {
        // x、y 是待比较整数；返回二者是否相等。
        return x.s == y.s && x.a == y.a;
    }
    friend bool operator<(const BigInt &x, const BigInt &y)
    {
        // x、y 是待比较整数；返回 x 是否小于 y。
        if (x.s != y.s)
        {
            return x.s < y.s;
        }
        if (!x.s)
        {
            return false;
        }
        int c = cmpAbs(x, y);
        return x.s > 0 ? c < 0 : c > 0;
    }
    friend bool operator!=(const BigInt &x, const BigInt &y)
    {
        // x、y 是待比较整数；返回二者是否不相等。
        return !(x == y);
    }
    friend bool operator>(const BigInt &x, const BigInt &y)
    {
        // x、y 是待比较整数；返回 x 是否大于 y。
        return y < x;
    }
    friend bool operator<=(const BigInt &x, const BigInt &y)
    {
        // x、y 是待比较整数；返回 x 是否不大于 y。
        return !(y < x);
    }
    friend bool operator>=(const BigInt &x, const BigInt &y)
    {
        // x、y 是待比较整数；返回 x 是否不小于 y。
        return !(x < y);
    }

    BigInt operator-() const
    {
        // 无参数；返回当前整数的相反数。
        BigInt x = *this;
        x.s = -x.s;
        return x;
    }

    static BigInt addAbs(const BigInt &x, const BigInt &y)
    {
        // x、y 是两个整数；返回 |x|+|y|。
        BigInt z;
        z.s = 1;
        int n = max(x.a.size(), y.a.size());
        z.a.resize(n);
        long long c = 0;
        for (int i = 0; i < n; i++)
        {
            c += (i < (int)(x.a.size()) ? x.a[i] : 0);
            c += (i < (int)(y.a.size()) ? y.a[i] : 0);
            z.a[i] = c % B;
            c /= B;
        }
        if (c)
        {
            z.a.push_back(c);
        }
        z.norm();
        return z;
    }

    // 要求 |x| >= |y|。
    static BigInt subAbs(const BigInt &x, const BigInt &y)
    {
        // x、y 满足 |x|>=|y|；返回 |x|-|y|。
        BigInt z;
        z.s = 1;
        z.a.resize(x.a.size());
        long long c = 0;
        for (int i = 0; i < (int)(x.a.size()); i++)
        {
            c = (long long)(x.a[i]) - c - (i < (int)(y.a.size()) ? y.a[i] : 0);
            if (c < 0)
            {
                c += B;
                z.a[i] = c;
                c = 1;
            }
            else
            {
                z.a[i] = c;
                c = 0;
            }
        }
        z.norm();
        return z;
    }

    BigInt &operator+=(const BigInt &y)
    {
        // y 是加数；把 y 加到当前整数并返回当前对象。
        if (!y.s)
        {
            return *this;
        }
        if (!s)
        {
            return *this = y;
        }
        if (s == y.s)
        {
            int t = s;
            *this = addAbs(*this, y);
            s = t;
        }
        else if (cmpAbs(*this, y) >= 0)
        {
            int t = s;
            *this = subAbs(*this, y);
            if (a.size())
            {
                s = t;
            }
        }
        else
        {
            int t = y.s;
            *this = subAbs(y, *this);
            if (a.size())
            {
                s = t;
            }
        }
        return *this;
    }
    BigInt &operator-=(const BigInt &y)
    {
        // y 是减数；从当前整数减去 y 并返回当前对象。
        return *this += -y;
    }

    BigInt &operator*=(const BigInt &y)
    {
        // y 是乘数；把当前整数乘以 y 并返回当前对象。
        if (!s || !y.s)
        {
            return *this = 0;
        }
        vector<int> c(a.size() + y.a.size());
        for (int i = 0; i < (int)a.size(); i++)
        {
            unsigned long long v = 0;
            for (int j = 0; j < (int)(y.a.size()) || v; j++)
            {
                unsigned long long w = c[i + j] + v;
                if (j < (int)(y.a.size()))
                {
                    w += (unsigned long long)a[i] * y.a[j];
                }
                c[i + j] = w % B;
                v = w / B;
            }
        }
        a = move(c);
        s *= y.s;
        norm();
        return *this;
    }

    static BigInt mulAbs(const BigInt &x, int y)
    {
        // x 是整数，y 满足 0<=y<B；返回 |x|*y。
        BigInt z;
        if (!y || !x.s)
        {
            return z;
        }
        z.s = 1;
        z.a.resize(x.a.size());
        unsigned long long c = 0;
        for (int i = 0; i < (int)x.a.size(); i++)
        {
            unsigned long long w = c + (unsigned long long)x.a[i] * y;
            z.a[i] = w % B;
            c = w / B;
        }
        if (c)
        {
            z.a.push_back(c);
        }
        return z;
    }

    // 商向 0 取整，余数与被除数同号；除数不能为 0。
    static pair<BigInt, BigInt> divmod(const BigInt &x, const BigInt &y)
    {
        // x 是被除数，y 是非零除数；返回向零取整的商和与 x 同号的余数。
        assert(y.s);
        BigInt q, r, d = y;
        d.s = 1;
        q.a.resize(x.a.size());
        q.s = x.s * y.s;
        for (int i = (int)(x.a.size()) - 1; i >= 0; i--)
        {
            r.a.insert(r.a.begin(), x.a[i]);
            r.s = 1;
            r.norm();
            int l = 0, h = B - 1;
            while (l < h)
            {
                int m = l + (h - l + 1) / 2;
                if (cmpAbs(mulAbs(d, m), r) <= 0)
                {
                    l = m;
                }
                else
                {
                    h = m - 1;
                }
            }
            q.a[i] = l;
            r = subAbs(r, mulAbs(d, l));
        }
        q.norm();
        if (r.a.size())
        {
            r.s = x.s;
        }
        return {q, r};
    }

    BigInt &operator/=(const BigInt &y)
    {
        // y 是非零除数；把当前整数改成向零取整的商并返回当前对象。
        return *this = divmod(*this, y).first;
    }
    BigInt &operator%=(const BigInt &y)
    {
        // y 是非零除数；把当前整数改成与原值同号的余数并返回当前对象。
        return *this = divmod(*this, y).second;
    }

    friend BigInt operator+(BigInt x, const BigInt &y)
    {
        // x、y 是加数；返回 x+y。
        return x += y;
    }
    friend BigInt operator-(BigInt x, const BigInt &y)
    {
        // x 是被减数，y 是减数；返回 x-y。
        return x -= y;
    }
    friend BigInt operator*(BigInt x, const BigInt &y)
    {
        // x、y 是乘数；返回 x*y。
        return x *= y;
    }
    friend BigInt operator/(BigInt x, const BigInt &y)
    {
        // x 是被除数，y 是非零除数；返回向零取整的商。
        return x /= y;
    }
    friend BigInt operator%(BigInt x, const BigInt &y)
    {
        // x 是被除数，y 是非零除数；返回与 x 同号的余数。
        return x %= y;
    }
};
