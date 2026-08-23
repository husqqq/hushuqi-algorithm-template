#pragma once

#include "14_1_016.hpp"

struct Decimal
{
    // man 是带符号十进制尾数，scale 是小数点后的十进制位数。
    BigInt man = 0;
    int scale = 0;
    // prec 是乘除、幂和平方根保留的小数位数。
    inline static int prec = 50;

    Decimal() = default;

    Decimal(long long x) : man(x)
    {
        // x 是 64 位整数；构造同值十进制数。
    }

    explicit Decimal(const BigInt &x) : man(x)
    {
        // x 是任意精度整数；构造同值十进制数。
    }

    explicit Decimal(const string &s)
    {
        // s 是带可选正负号和小数点的十进制串；构造按当前精度舍入后的十进制数。
        assign(s);
    }

    static void setPrecision(int digits)
    {
        // digits 是后续运算保留的小数位数；修改全局动态精度，无返回值。
        assert(digits >= 0);
        prec = digits;
    }

    static int getPrecision()
    {
        // 无参数；返回当前动态精度。
        return prec;
    }

    Decimal &assign(const string &s)
    {
        // s 是带可选正负号和至多一个小数点的十进制串；解析、舍入并返回当前对象。
        assert(!s.empty());
        int p = 0, sign = 1;
        if (s[p] == '+' || s[p] == '-')
        {
            sign = s[p] == '-' ? -1 : 1;
            p++;
        }
        assert(p < (int)s.size());
        string digit;
        bool dot = false;
        scale = 0;
        for (; p < (int)s.size(); p++)
        {
            if (s[p] == '.')
            {
                assert(!dot);
                dot = true;
                continue;
            }
            assert('0' <= s[p] && s[p] <= '9');
            digit.push_back(s[p]);
            if (dot)
            {
                scale++;
            }
        }
        assert(!digit.empty());
        man = BigInt((sign < 0 ? "-" : "") + digit);
        round(prec);
        normalize();
        return *this;
    }

    string str(int fixed = -1) const
    {
        // fixed 小于 0 时删除末尾零，否则固定输出 fixed 位小数；返回十进制字符串。
        assert(fixed >= -1);
        Decimal x = *this;
        if (fixed >= 0)
        {
            x.round(fixed);
        }
        if (!x.man.s)
        {
            return fixed > 0 ? "0." + string(fixed, '0') : "0";
        }
        bool neg = x.man.s < 0;
        string s = x.man.str();
        if (neg)
        {
            s.erase(s.begin());
        }
        if ((int)s.size() <= x.scale)
        {
            s = string(x.scale + 1 - s.size(), '0') + s;
        }
        string whole = s.substr(0, s.size() - x.scale);
        string fraction = x.scale ? s.substr(s.size() - x.scale) : "";
        if (fixed >= 0)
        {
            fraction += string(fixed - fraction.size(), '0');
        }
        string ans = (neg ? "-" : "") + whole;
        if (!fraction.empty())
        {
            ans += "." + fraction;
        }
        return ans;
    }

    void round(int digits)
    {
        // digits 是要保留的小数位数；按绝对值五入舍去其余位，无返回值。
        assert(digits >= 0);
        if (scale > digits)
        {
            man = drop10(man, scale - digits, true);
            scale = digits;
        }
        normalize();
    }

    Decimal trunc() const
    {
        // 无参数；返回向零取整后的整数值。
        Decimal ans;
        ans.man = drop10(man, scale, false);
        ans.normalize();
        return ans;
    }

    Decimal abs() const
    {
        // 无参数；返回当前数的绝对值。
        Decimal ans = *this;
        if (ans.man.s < 0)
        {
            ans.man.s = 1;
        }
        return ans;
    }

    bool isZero() const
    {
        // 无参数；返回当前数是否为零。
        return man.s == 0;
    }

    static Decimal pow(Decimal base, int exponent)
    {
        // base 是底数，exponent 是非负整数指数；返回按当前精度计算的幂。
        assert(exponent >= 0);
        Decimal ans(1);
        while (exponent)
        {
            if (exponent & 1)
            {
                ans *= base;
            }
            exponent >>= 1;
            if (exponent)
            {
                base *= base;
            }
        }
        return ans;
    }

    Decimal sqrt() const
    {
        // 无参数；当前数须非负，返回按当前精度四舍五入的平方根。
        assert(man.s >= 0);
        if (!man.s)
        {
            return Decimal(0);
        }
        int work = max(prec + 4, (scale + 1) / 2 + 2);
        int exponent = 2 * work - scale;
        assert(exponent >= 0);
        Decimal ans;
        ans.man = BigInt(bigSqrt(shift10(man, exponent).str()));
        ans.scale = work;
        ans.round(prec);
        return ans;
    }

    Decimal operator-() const
    {
        // 无参数；返回当前数的相反数。
        Decimal ans = *this;
        ans.man = -ans.man;
        return ans;
    }

    Decimal &operator+=(const Decimal &other)
    {
        // other 是加数；把它加到当前数并返回当前对象。
        return *this = *this + other;
    }

    Decimal &operator-=(const Decimal &other)
    {
        // other 是减数；从当前数减去它并返回当前对象。
        return *this = *this - other;
    }

    Decimal &operator*=(const Decimal &other)
    {
        // other 是乘数；把当前数乘以它并返回当前对象。
        return *this = *this * other;
    }

    Decimal &operator/=(const Decimal &other)
    {
        // other 是非零除数；把当前数除以它并返回当前对象。
        return *this = *this / other;
    }

    Decimal &operator%=(const Decimal &other)
    {
        // other 是非零除数；把当前数改为向零取商对应的余数并返回当前对象。
        return *this = *this % other;
    }

    friend Decimal operator+(const Decimal &a, const Decimal &b)
    {
        // a、b 是加数；返回按当前精度舍入的和。
        int scale = max(a.scale, b.scale);
        Decimal ans;
        ans.man = shift10(a.man, scale - a.scale) + shift10(b.man, scale - b.scale);
        ans.scale = scale;
        ans.round(prec);
        return ans;
    }

    friend Decimal operator-(const Decimal &a, const Decimal &b)
    {
        // a 是被减数，b 是减数；返回按当前精度舍入的差。
        return a + (-b);
    }

    friend Decimal operator*(const Decimal &a, const Decimal &b)
    {
        // a、b 是乘数；返回按当前精度舍入的积。
        Decimal ans;
        ans.man = a.man * b.man;
        ans.scale = a.scale + b.scale;
        ans.round(prec);
        return ans;
    }

    friend Decimal operator/(const Decimal &a, const Decimal &b)
    {
        // a 是被除数，b 是非零除数；返回按当前精度舍入的商。
        assert(b.man.s);
        if (!a.man.s)
        {
            return Decimal(0);
        }
        int sign = a.man.s * b.man.s;
        BigInt num = absInt(a.man), den = absInt(b.man);
        int work = prec + 4;
        int exponent = work + b.scale - a.scale;
        if (exponent >= 0)
        {
            num = shift10(num, exponent);
        }
        else
        {
            den = shift10(den, -exponent);
        }
        Decimal ans;
        ans.man = num / den;
        ans.man.s *= sign;
        ans.scale = work;
        ans.round(prec);
        return ans;
    }

    friend Decimal operator%(const Decimal &a, const Decimal &b)
    {
        // a 是被除数，b 是非零除数；返回 a-trunc(a/b)*b。
        assert(b.man.s);
        BigInt num = a.man, den = b.man;
        if (b.scale >= a.scale)
        {
            num = shift10(num, b.scale - a.scale);
        }
        else
        {
            den = shift10(den, a.scale - b.scale);
        }
        Decimal quotient(num / den);
        return a - quotient * b;
    }

    friend bool operator==(const Decimal &a, const Decimal &b)
    {
        // a、b 是待比较十进制数；返回二者是否相等。
        return a.scale == b.scale && a.man == b.man;
    }

    friend bool operator!=(const Decimal &a, const Decimal &b)
    {
        // a、b 是待比较十进制数；返回二者是否不等。
        return !(a == b);
    }

    friend bool operator<(const Decimal &a, const Decimal &b)
    {
        // a、b 是待比较十进制数；返回 a 是否小于 b。
        int scale = max(a.scale, b.scale);
        return shift10(a.man, scale - a.scale) < shift10(b.man, scale - b.scale);
    }

    friend bool operator>(const Decimal &a, const Decimal &b)
    {
        // a、b 是待比较十进制数；返回 a 是否大于 b。
        return b < a;
    }

    friend bool operator<=(const Decimal &a, const Decimal &b)
    {
        // a、b 是待比较十进制数；返回 a 是否不大于 b。
        return !(b < a);
    }

    friend bool operator>=(const Decimal &a, const Decimal &b)
    {
        // a、b 是待比较十进制数；返回 a 是否不小于 b。
        return !(a < b);
    }

    friend ostream &operator<<(ostream &out, const Decimal &x)
    {
        // out 是输出流，x 是待输出十进制数；写入去掉末尾零的表示并返回输出流。
        return out << x.str();
    }

    friend istream &operator>>(istream &in, Decimal &x)
    {
        // in 是输入流，x 接收十进制数；读取一个字符串并返回输入流。
        string s;
        in >> s;
        if (in)
        {
            x.assign(s);
        }
        return in;
    }

  private:
    static BigInt absInt(BigInt x)
    {
        // x 是任意精度整数；返回其绝对值。
        if (x.s < 0)
        {
            x.s = 1;
        }
        return x;
    }

    static BigInt shift10(BigInt x, int digits)
    {
        // x 是任意精度整数，digits 是非负十进制位数；返回 x*10^digits。
        assert(digits >= 0);
        if (!x.s || !digits)
        {
            return x;
        }
        int sign = x.s;
        x.s = 1;
        int blocks = digits / 9, rest = digits % 9;
        if (rest)
        {
            int factor = 1;
            for (int i = 0; i < rest; i++)
            {
                factor *= 10;
            }
            x = BigInt::mulAbs(x, factor);
        }
        x.a.insert(x.a.begin(), blocks, 0);
        x.s = sign;
        return x;
    }

    static BigInt drop10(BigInt x, int digits, bool nearest)
    {
        // x 是任意精度整数，digits 是舍去位数；nearest 表示是否绝对值五入，返回向零截断或舍入后的整数。
        assert(digits >= 0);
        if (!x.s || !digits)
        {
            return x;
        }
        int sign = x.s;
        x.s = 1;
        int blocks = digits / 9, rest = digits % 9;
        int roundDig = 0;
        if (rest && blocks < (int)x.a.size())
        {
            int factor = 1;
            for (int i = 1; i < rest; i++)
            {
                factor *= 10;
            }
            roundDig = x.a[blocks] / factor % 10;
        }
        else if (!rest && blocks && blocks - 1 < (int)x.a.size())
        {
            roundDig = x.a[blocks - 1] / 100000000;
        }
        if (blocks >= (int)x.a.size())
        {
            x = 0;
        }
        else
        {
            x.a.erase(x.a.begin(), x.a.begin() + blocks);
            if (rest)
            {
                int divisor = 1;
                for (int i = 0; i < rest; i++)
                {
                    divisor *= 10;
                }
                unsigned long long carry = 0;
                for (int i = (int)x.a.size() - 1; i >= 0; i--)
                {
                    unsigned long long cur = carry * BigInt::B + x.a[i];
                    x.a[i] = cur / divisor;
                    carry = cur % divisor;
                }
            }
            x.norm();
            if (x.s)
            {
                x.s = sign;
            }
        }
        if (nearest && roundDig >= 5)
        {
            x += BigInt(sign);
        }
        return x;
    }

    void normalize()
    {
        // 无参数；删除尾数与小数位中的十进制末尾零，无返回值。
        man.norm();
        if (!man.s)
        {
            scale = 0;
            return;
        }
        while (scale >= 9 && !man.a.empty() && man.a[0] == 0)
        {
            man.a.erase(man.a.begin());
            scale -= 9;
        }
        man.norm();
        while (scale && man.a[0] % 10 == 0)
        {
            man = drop10(man, 1, false);
            scale--;
        }
    }
};
