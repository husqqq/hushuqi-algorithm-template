#pragma once

#include "10_1_004.hpp"
#include <bits/stdc++.h>
using namespace std;
#define int long long

// limb、进位和移位槽固定为 32 位，依赖截断与二进制位布局；64 位中间量承载双字运算。
struct HexBigInt
{
    using It = vector<uint32_t>::iterator;
    using CIt = vector<uint32_t>::const_iterator;

    bool neg = false;   // 是否为负数，零恒为非负。
    vector<uint32_t> a; // 绝对值的固定 32 位小端 limb，无高位零，零为空。

    HexBigInt()
    {
        // 无参数；构造整数零。
    }

    HexBigInt(long long x)
    {
        // x 是任意 64 位有符号整数（含 LLONG_MIN）；按符号与绝对值的 32 位小端 limb 构造。
        neg = x < 0;
        unsigned long long y = neg ? 0 - (unsigned long long)x : (unsigned long long)x;
        while (y)
        {
            a.push_back((uint32_t)y);
            y >>= 32;
        }
    }

    HexBigInt(const string &s)
    {
        // s 是带可选 +/- 号、至少一位数字的十六进制串（大小写均可）；解析为对应大整数。
        assign(s);
    }

    HexBigInt(bool sgn, vector<uint32_t> v) : neg(sgn), a(move(v))
    {
        // sgn 为真表示负数，v 是绝对值的 32 位小端 limb；构造后去高零，值为零时强制非负。
        trim(this->a);
        if (this->a.empty())
        {
            this->neg = false;
        }
    }

    HexBigInt &assign(const string &s)
    {
        // s 是带可选 +/- 号、至少一位数字的十六进制串（0-9A-Fa-f）；覆盖当前值并返回 *this，零非负。
        assert(!s.empty());
        int p = 0;
        neg = s[0] == '-';
        if (s[0] == '-' || s[0] == '+')
        {
            p++;
        }
        assert(p < (int)s.size());
        a.clear();
        for (int r = s.size(); r > p; r -= 8)
        {
            int l = max(p, r - 8);
            uint32_t x = 0;
            for (int i = l; i < r; i++)
            {
                x <<= 4;
                if ('0' <= s[i] && s[i] <= '9')
                {
                    x |= s[i] - '0';
                }
                else if ('A' <= s[i] && s[i] <= 'F')
                {
                    x |= s[i] - 'A' + 10;
                }
                else
                {
                    assert('a' <= s[i] && s[i] <= 'f');
                    x |= s[i] - 'a' + 10;
                }
            }
            a.push_back(x);
        }
        trim(a);
        if (a.empty())
        {
            neg = false;
        }
        return *this;
    }

    string toHex() const
    {
        // 无参数；返回大写十六进制串，负数带前导 -，无前导零，零返回 "0"。
        if (a.empty())
        {
            return "0";
        }
        const char *digit = "0123456789ABCDEF";
        string s = neg ? "-" : "";
        for (int i = a.size() - 1; i >= 0; i--)
        {
            string t(8, '0');
            uint32_t x = a[i];
            for (int j = 7; j >= 0; j--)
            {
                t[j] = digit[x & 15];
                x >>= 4;
            }
            if (i == (int)a.size() - 1)
            {
                t.erase(0, t.find_first_not_of('0'));
            }
            s += t;
        }
        return s;
    }

    bool isZero() const
    {
        // 无参数；返回当前值是否为零（limb 数组为空）。
        return a.empty();
    }

    HexBigInt operator-() const
    {
        // 无参数；返回相反数，零仍保持非负。
        HexBigInt x = *this;
        if (!x.a.empty())
        {
            x.neg = !x.neg;
        }
        return x;
    }

    friend bool operator==(const HexBigInt &x, const HexBigInt &y)
    {
        // x、y 是规范整数；返回 x 是否相等 y。
        return x.neg == y.neg && x.a == y.a;
    }

    friend bool operator<(const HexBigInt &x, const HexBigInt &y)
    {
        // x、y 是规范大整数；返回按有符号数值比较 x<y 是否成立。
        if (x.neg != y.neg)
        {
            return x.neg;
        }
        int c = cmp(x.a, y.a);
        return x.neg ? c > 0 : c < 0;
    }

    friend bool operator!=(const HexBigInt &x, const HexBigInt &y)
    {
        // x、y 是规范整数；返回 x 是否不等 y。
        return !(x == y);
    }

    friend bool operator>(const HexBigInt &x, const HexBigInt &y)
    {
        // x、y 是规范大整数；返回 x>y 是否成立（即 y<x）。
        return y < x;
    }

    friend bool operator<=(const HexBigInt &x, const HexBigInt &y)
    {
        // x、y 是规范整数；返回 x 是否小于等于 y。
        return !(y < x);
    }

    friend bool operator>=(const HexBigInt &x, const HexBigInt &y)
    {
        // x、y 是规范整数；返回 x 是否大于等于 y。
        return !(x < y);
    }

    friend HexBigInt operator+(const HexBigInt &x, const HexBigInt &y)
    {
        // x、y 是规范大整数；返回 x+y：同号相加绝对值，异号用较大绝对值减较小绝对值。
        if (x.neg == y.neg)
        {
            return {x.neg, addVec(x.a, y.a)};
        }
        int c = cmp(x.a, y.a);
        if (c <= 0)
        {
            return {c ? y.neg : false, subVec(y.a, x.a)};
        }
        return {x.neg, subVec(x.a, y.a)};
    }

    friend HexBigInt operator-(const HexBigInt &x, const HexBigInt &y)
    {
        // x、y 是规范大整数；返回 x-y，即 x+(-y)。
        return x + (-y);
    }

    friend HexBigInt operator*(const HexBigInt &x, const HexBigInt &y)
    {
        // x、y 是规范大整数；返回 x*y，积为零时非负。
        auto z = mulVec(x.a, y.a);
        return {z.empty() ? false : x.neg != y.neg, move(z)};
    }

    friend pair<HexBigInt, HexBigInt> divmod(const HexBigInt &x, const HexBigInt &y)
    {
        // x 是被除数，y 是非零除数；返回 {商,余数}，商向零截断，余数与 x 同号（零非负）。
        assert(!y.a.empty());
        auto [q, r] = divVec(x.a, y.a);
        return {{q.empty() ? false : x.neg != y.neg, move(q)}, {r.empty() ? false : x.neg, move(r)}};
    }

    friend HexBigInt operator/(const HexBigInt &x, const HexBigInt &y)
    {
        // x 是被除数，y 是非零除数；返回向零截断的商。
        return divmod(x, y).first;
    }

    friend HexBigInt operator%(const HexBigInt &x, const HexBigInt &y)
    {
        // x 是被除数，y 是非零除数；返回与 x 同号的余数 x-(x/y)*y。
        return divmod(x, y).second;
    }

    HexBigInt &operator+=(const HexBigInt &x)
    {
        // x 是右操作数；原地加上 x，返回 *this。
        return *this = *this + x;
    }

    HexBigInt &operator-=(const HexBigInt &x)
    {
        // x 是右操作数；原地减去 x，返回 *this。
        return *this = *this - x;
    }

    HexBigInt &operator*=(const HexBigInt &x)
    {
        // x 是右操作数；原地乘以 x，返回 *this。
        return *this = *this * x;
    }

    HexBigInt &operator/=(const HexBigInt &x)
    {
        // x 是右操作数；原地除以非零 x，向零截断，返回 *this。
        return *this = *this / x;
    }

    HexBigInt &operator%=(const HexBigInt &x)
    {
        // x 是右操作数；原地对非零 x 取余，余数与原值同号，返回 *this。
        return *this = *this % x;
    }

    friend ostream &operator<<(ostream &out, const HexBigInt &x)
    {
        // out 是输出流，x 是大整数；写入 x.toHex() 并返回 out。
        return out << x.toHex();
    }

    static HexBigInt fromDecimal(const string &s)
    {
        // s 是带可选正负号的非空十进制整数串；返回其精确值，以二进制 limb 保存。
        assert(!s.empty());
        int p = 0;
        bool sign = s[0] == '-';
        if (s[0] == '-' || s[0] == '+')
        {
            p++;
        }
        assert(p < (int)s.size());
        vector<int> group;
        for (int r = s.size(); r > p; r -= 9)
        {
            int l = max<int>(p, r - 9);
            int x = 0;
            for (int i = l; i < r; i++)
            {
                assert('0' <= s[i] && s[i] <= '9');
                x = x * 10 + s[i] - '0';
            }
            group.push_back(x);
        }
        int z = bit_ceil((unsigned long long)max<int>(1, group.size()));
        group.resize(z);
        vector<HexBigInt> power;
        power.emplace_back(1000000000LL);
        for (int n = 2; n < z; n <<= 1)
        {
            power.push_back(power.back() * power.back());
        }
        auto build = [&](auto &&self, int l, int len, int level) -> HexBigInt
        {
            // self 是递归自身；合并 group 的 [l,l+len)，len=2^level，返回对应非负整数。
            if (len == 1)
            {
                return HexBigInt((long long)group[l]);
            }
            int half = len / 2;
            HexBigInt low = self(self, l, half, level - 1);
            HexBigInt high = self(self, l + half, half, level - 1);
            return low + high * power[level - 1];
        };
        HexBigInt ans = build(build, 0, z, countr_zero((unsigned long long)z));
        if (!ans.a.empty())
        {
            ans.neg = sign;
        }
        return ans;
    }

    string toDecimal() const
    {
        // 无参数；按 10^9 幂递归分裂为十进制分组，返回规范字符串。
        if (a.empty())
        {
            return "0";
        }
        HexBigInt x = *this;
        x.neg = false;
        vector<HexBigInt> power{HexBigInt(1000000000LL)};
        int level = 0;
        while (!(x < power[level]))
        {
            power.push_back(power.back() * power.back());
            level++;
        }
        vector<int> group(1LL << level);
        auto split = [&](auto &&self, const HexBigInt &v, int l, int lv) -> void
        {
            // self 是递归自身；把非负 v 的 2^lv 个十进制块写入 group[l..l+2^lv)，无返回值。
            if (lv == 0)
            {
                group[l] = v.a.empty() ? 0 : v.a[0];
                return;
            }
            auto [high, low] = divmod(v, power[lv - 1]);
            self(self, low, l, lv - 1);
            self(self, high, l + (1LL << (lv - 1)), lv - 1);
        };
        split(split, x, 0, level);
        while (group.size() > 1 && group.back() == 0)
        {
            group.pop_back();
        }
        string s = neg ? "-" : "";
        s += to_string(group.back());
        for (int i = group.size() - 2; i >= 0; i--)
        {
            string t = to_string(group[i]);
            s += string(9 - t.size(), '0') + t;
        }
        return s;
    }

  private:
    static constexpr int naiveDiv = 64; // 改用竖式除法的 limb 数阈值。

    static void trim(vector<uint32_t> &x)
    {
        // x 是小端 limb 数组；原地弹掉最高位的零 limb，零变为空数组。
        while (!x.empty() && x.back() == 0)
        {
            x.pop_back();
        }
    }

    static bool less(CIt x, CIt xe, CIt y, CIt ye)
    {
        // [x,xe)、[y,ye) 是小端 limb 区间，长度可不同且允许高位零；返回无符号值 x<y 是否成立。
        int nx = xe - x, ny = ye - y;
        while (nx && x[nx - 1] == 0)
        {
            nx--;
        }
        while (ny && y[ny - 1] == 0)
        {
            ny--;
        }
        if (nx != ny)
        {
            return nx < ny;
        }
        for (int i = nx - 1; i >= 0; i--)
        {
            if (x[i] != y[i])
            {
                return x[i] < y[i];
            }
        }
        return false;
    }

    static int cmp(const vector<uint32_t> &x, const vector<uint32_t> &y)
    {
        // x、y 是已去高零的小端 limb 数组；返回无符号比较结果 -1（x<y）、0（相等）、1（x>y）。
        if (x == y)
        {
            return 0;
        }
        return less(x.begin(), x.end(), y.begin(), y.end()) ? -1 : 1;
    }

    static void add(CIt x, CIt xe, CIt y, CIt ye, It z, It ze)
    {
        // [x,xe)、[y,ye) 是小端 limb 区间；把无符号和 x+y 写入 z 的低 max(|x|,|y|) 个 limb，有进位时再写一位。
        // z 至少长 max(|x|,|y|)，产生最高进位时须再多一位；z 中更高的位不被改动。
        if (xe - x < ye - y)
        {
            swap(x, y);
            swap(xe, ye);
        }
        int nx = xe - x, ny = ye - y;
        assert(nx <= ze - z);
        uint32_t carry = 0;
        for (int i = 0; i < ny; i++)
        {
            unsigned long long v = (unsigned long long)x[i] + y[i] + carry;
            z[i] = v;
            carry = v >> 32;
        }
        for (int i = ny; i < nx; i++)
        {
            unsigned long long v = (unsigned long long)x[i] + carry;
            z[i] = v;
            carry = v >> 32;
        }
        if (carry)
        {
            assert(z + nx < ze);
            z[nx] = carry;
        }
    }

    static void addTo(It x, It xe, CIt y, CIt ye)
    {
        // [x,xe)、[y,ye) 是小端 limb 区间且 |x|≥|y|；原地令 x+=y，要求最终进位能在 x 的长度内吸收。
        int nx = xe - x, ny = ye - y;
        uint32_t carry = 0;
        for (int i = 0; i < ny; i++)
        {
            unsigned long long v = (unsigned long long)x[i] + y[i] + carry;
            x[i] = v;
            carry = v >> 32;
        }
        for (int i = ny; carry && i < nx; i++)
        {
            unsigned long long v = (unsigned long long)x[i] + carry;
            x[i] = v;
            carry = v >> 32;
        }
        assert(!carry);
    }

    static void subFrom(It x, It xe, CIt y, CIt ye)
    {
        // [x,xe)、[y,ye) 是小端 limb 区间且 |x|≥|y|；原地令 x-=y，要求 x 的值不小于 y（无最终借位）。
        int nx = xe - x, ny = ye - y;
        int borrow = 0;
        for (int i = 0; i < ny; i++)
        {
            long long v = (long long)x[i] - y[i] + borrow;
            x[i] = v;
            borrow = v >> 32;
        }
        for (int i = ny; borrow && i < nx; i++)
        {
            long long v = (long long)x[i] + borrow;
            x[i] = v;
            borrow = v >> 32;
        }
        assert(!borrow);
    }

    static vector<uint32_t> addVec(const vector<uint32_t> &x, const vector<uint32_t> &y)
    {
        // x、y 是小端 limb 数组；返回去高零后的无符号和 x+y。
        vector<uint32_t> z(max(x.size(), y.size()) + 1);
        add(x.begin(), x.end(), y.begin(), y.end(), z.begin(), z.end());
        trim(z);
        return z;
    }

    static vector<uint32_t> subVec(const vector<uint32_t> &x, const vector<uint32_t> &y)
    {
        // x、y 是去高零的小端 limb 数组且 x≥y；返回去高零后的无符号差 x-y。
        assert(cmp(x, y) >= 0);
        vector<uint32_t> z = x;
        subFrom(z.begin(), z.end(), y.begin(), y.end());
        trim(z);
        return z;
    }

    static void mulNaive(CIt x, CIt xe, CIt y, CIt ye, It z, It ze)
    {
        // [x,xe)、[y,ye) 是小端 limb 区间；竖式乘法把无符号积 x*y 写入 z。
        // z 长度恰为 |x|+|y|，且低 |y| 个 limb 须为零（通常整体清零）。
        int nx = xe - x, ny = ye - y;
        assert(ze - z == nx + ny);
        for (int i = 0; i < nx; i++)
        {
            uint32_t carry = 0;
            for (int j = 0; j < ny; j++)
            {
                unsigned long long v = (unsigned long long)x[i] * y[j] + carry + z[i + j];
                z[i + j] = v;
                carry = v >> 32;
            }
            z[i + ny] = carry;
        }
    }

    static void mul(CIt x, CIt xe, CIt y, CIt ye, It z, It ze)
    {
        // [x,xe)、[y,ye) 是小端 limb 区间，z 长度恰为 |x|+|y| 且须预先清零；把无符号积 x*y 写入 z。
        // 较短因子不超过 128 个 limb 时用竖式，否则用 Karatsuba（长短悬殊时先把长因子对半拆开）。
        if (xe - x < ye - y)
        {
            swap(x, y);
            swap(xe, ye);
        }
        int nx = xe - x, ny = ye - y;
        assert(ze - z == nx + ny);
        if (ny <= 128)
        {
            mulNaive(x, xe, y, ye, z, ze);
            return;
        }
        int n = (nx + 1) / 2;
        if (ny <= n)
        {
            mul(x, x + n, y, ye, z, z + n + ny);
            vector<uint32_t> carry(z + n, z + n + ny);
            fill(z + n, z + n + ny, 0);
            mul(x + n, xe, y, ye, z + n, ze);
            addTo(z + n, ze, carry.begin(), carry.end());
            return;
        }
        mul(x, x + n, y, y + n, z, z + 2 * n);
        mul(x + n, xe, y + n, ye, z + 2 * n, ze);
        vector<uint32_t> x1(n + 1), y1(n + 1), mid(2 * n + 2);
        add(x, x + n, x + n, xe, x1.begin(), x1.end());
        add(y, y + n, y + n, ye, y1.begin(), y1.end());
        mul(x1.begin(), x1.end(), y1.begin(), y1.end(), mid.begin(), mid.end());
        subFrom(mid.begin(), mid.end(), z, z + 2 * n);
        subFrom(mid.begin(), mid.end(), z + 2 * n, ze);
        trim(mid);
        addTo(z + n, ze, mid.begin(), mid.end());
    }

    static vector<uint32_t> mulVec(const vector<uint32_t> &x, const vector<uint32_t> &y)
    {
        // x、y 是去高零的小端 limb 数组；返回去高零后的无符号积，任一为空（零）时返回空。
        if (x.empty() || y.empty())
        {
            return {};
        }
        vector<uint32_t> z(x.size() + y.size());
        mul(x.begin(), x.end(), y.begin(), y.end(), z.begin(), z.end());
        trim(z);
        return z;
    }

    static void leftShift(It x, It xe, int s)
    {
        // [x,xe) 是小端 limb 区间，0<=s<32；原地整体左移 s 位，要求最高 limb 被移出的位全为零。
        assert(0 <= s && s < 32);
        if (!s)
        {
            return;
        }
        int back = 32 - s;
        uint32_t carry = 0;
        for (auto it = x; it != xe; ++it)
        {
            uint32_t v = (*it << s) | carry;
            carry = *it >> back;
            *it = v;
        }
        assert(!carry);
    }

    static void shiftR(It x, It xe, int s)
    {
        // [x,xe) 是小端 limb 区间，0<=s<32；原地整体右移 s 位，要求最低 limb 的低 s 位为零（值能被 2^s 整除）。
        assert(0 <= s && s < 32);
        if (!s)
        {
            return;
        }
        int back = 32 - s;
        uint32_t carry = 0;
        for (auto it = xe; it != x;)
        {
            --it;
            uint32_t v = (*it >> s) | carry;
            carry = *it << back;
            *it = v;
        }
        assert(!carry);
    }

    static void divNaive(CIt x, CIt xe, CIt y, CIt ye, It q, It qe, It r, It re)
    {
        // [x,xe) 是被除数、[y,ye) 是除数（最高 limb 非零）的小端 limb 区间；规格化长除法把商写入 q、余数写入 r。
        // 只写商与余数的有效低位，q 须能容纳商、r 至少 |y| 个 limb，二者须预先清零。
        int nx = xe - x, ny = ye - y;
        if (ny == 1)
        {
            unsigned long long carry = 0;
            for (int i = nx - 1; i >= 0; i--)
            {
                unsigned long long v = (carry << 32) | x[i];
                q[i] = v / y[0];
                carry = v - (unsigned long long)q[i] * y[0];
            }
            r[0] = carry;
            return;
        }
        if (max(nx, ny) <= 2)
        {
            unsigned long long xv = x[0] | (nx == 2 ? (unsigned long long)x[1] << 32 : 0);
            unsigned long long yv = y[0] | (ny == 2 ? (unsigned long long)y[1] << 32 : 0);
            unsigned long long qv = xv / yv, rv = xv % yv;
            if (q != qe)
            {
                q[0] = qv;
                if (q + 1 != qe)
                {
                    q[1] = qv >> 32;
                }
            }
            r[0] = rv;
            if (r + 1 != re)
            {
                r[1] = rv >> 32;
            }
            return;
        }
        if (less(x, xe, y, ye))
        {
            copy(x, min(xe, x + (ye - y)), r);
            return;
        }
        int shift = countl_zero(y[ny - 1]);
        vector<uint32_t> a(nx + (countl_zero(x[nx - 1]) < shift));
        vector<uint32_t> b(y, ye);
        copy(x, xe, a.begin());
        leftShift(a.begin(), a.end(), shift);
        leftShift(b.begin(), b.end(), shift);
        uint32_t top = b.back();
        vector<uint32_t> qv(a.size() - b.size() + 1);
        vector<uint32_t> rv(a.end() - b.size(), a.end());
        for (int i = qv.size() - 1; i >= 0; i--)
        {
            if (rv.size() == b.size())
            {
                if (!less(rv.begin(), rv.end(), b.begin(), b.end()))
                {
                    qv[i] = 1;
                    rv = subVec(rv, b);
                }
            }
            else if (rv.size() > b.size())
            {
                unsigned long long top2 = ((unsigned long long)rv.back() << 32) | rv[rv.size() - 2];
                unsigned long long z = min<unsigned long long>(UINT32_MAX, top2 / top);
                vector<uint32_t> bz = mulVec(b, vector<uint32_t>{(uint32_t)z});
                while (less(rv.begin(), rv.end(), bz.begin(), bz.end()))
                {
                    z--;
                    bz = subVec(bz, b);
                }
                rv = subVec(rv, bz);
                while (!less(rv.begin(), rv.end(), b.begin(), b.end()))
                {
                    z++;
                    rv = subVec(rv, b);
                }
                qv[i] = z;
            }
            if (i)
            {
                rv.insert(rv.begin(), a[i - 1]);
            }
            trim(rv);
        }
        shiftR(rv.begin(), rv.end(), shift);
        trim(qv);
        trim(rv);
        copy(qv.begin(), qv.end(), q);
        copy(rv.begin(), rv.end(), r);
    }

    static void div2n1n(CIt x, CIt xe, CIt y, CIt ye, It q, It qe, It r, It re)
    {
        // x 长 2n、y 长 n（divide 规格化后的除数），要求 x 的高 n 个 limb 小于 y；q 长 n、r 长 n+1，均须预先清零。
        // 把 x/y 的商写入 q、余数写入 r；n 为奇数或不超过 naiveDiv 时用长除法，否则拆成两次 3n/2n 除法。
        int n = ye - y;
        assert(xe - x == 2 * n && qe - q == n && re - r == n + 1);
        assert(less(x + n, xe, y, ye));
        if (n % 2 || n <= naiveDiv)
        {
            divNaive(x, xe, y, ye, q, qe, r, re);
            return;
        }
        int h = n / 2;
        vector<uint32_t> r1(n + h + 1);
        copy(x, x + h, r1.begin());
        div3n2n(x + h, xe, y, ye, q + h, qe, r1.begin() + h, r1.end());
        div3n2n(r1.begin(), r1.end() - 1, y, ye, q, q + h, r, re);
    }

    static void div3n2n(CIt x, CIt xe, CIt y, CIt ye, It q, It qe, It r, It re)
    {
        // x 长 3n、y 长 2n，要求 x 的高 2n 个 limb 小于 y；q 长 n、r 长 2n+1，均须预先清零。
        // 用高位 2n/n 除法（或全 1）估商，再反复 q--、r+=y 修正，把 x/y 的商写入 q、余数写入 r。
        int n = (ye - y) / 2;
        assert(xe - x == 3 * n && qe - q == n && re - r == 2 * n + 1);
        assert(less(x + n, xe, y, ye));
        vector<uint32_t> d(2 * n);
        if (less(x + 2 * n, xe, y + n, ye))
        {
            div2n1n(x + n, xe, y + n, ye, q, qe, r + n, re);
            mul(q, qe, y, y + n, d.begin(), d.end());
        }
        else
        {
            fill(q, qe, UINT32_MAX);
            add(x + n, x + 2 * n, y + n, ye, r + n, re);
            copy(y, y + n, d.begin() + n);
            subFrom(d.begin(), d.end(), y, y + n);
        }
        copy(x, x + n, r);
        while (less(r, re, d.begin(), d.end()))
        {
            addTo(r, re, y, ye);
            vector<uint32_t> one{1};
            subFrom(q, qe, one.begin(), one.end());
        }
        subFrom(r, re, d.begin(), d.end());
    }

    static void divide(CIt x, CIt xe, CIt y, CIt ye, It q, It qe, It r, It re)
    {
        // [x,xe)、[y,ye) 是小端 limb 区间（y 非空且最高 limb 非零），q 长 |x|-|y|+1、r 长 |y|，均须预先清零。
        // 把 x/y 的商写入 q、余数写入 r；较短时用长除法，否则补零规格化后逐块调用 2n/n 除法。
        int nx = xe - x, ny = ye - y;
        assert(ny > 0 && qe - q == nx - ny + 1 && re - r == ny);
        if (min(nx - ny, ny) <= naiveDiv)
        {
            divNaive(x, xe, y, ye, q, qe, r, re);
            return;
        }
        int blocks = (ny + naiveDiv - 1) / naiveDiv;
        if (blocks > 1)
        {
            blocks = bit_ceil((unsigned long long)blocks);
        }
        int n = (ny + blocks - 1) / blocks * blocks;
        int digit = n - ny;
        int shift = countl_zero(y[ny - 1]);
        vector<uint32_t> a(nx + digit + (countl_zero(x[nx - 1]) <= shift));
        vector<uint32_t> b(n), rem(n + 1), z(2 * n);
        copy(x, xe, a.begin() + digit);
        copy(y, ye, b.begin() + digit);
        leftShift(a.begin() + digit, a.end(), shift);
        leftShift(b.begin() + digit, b.end(), shift);
        int t = max<long long>(2, (a.size() + n - 1) / n);
        copy(a.begin() + (t - 2) * n, a.end(), z.begin());
        int upper = qe - (q + (t - 2) * n);
        if (upper < n)
        {
            vector<uint32_t> qq(n);
            div2n1n(z.begin(), z.end(), b.begin(), b.end(), qq.begin(), qq.end(), rem.begin(), rem.end());
            copy(qq.begin(), qq.begin() + upper, q + (t - 2) * n);
        }
        else
        {
            div2n1n(z.begin(), z.end(), b.begin(), b.end(), q + (t - 2) * n, q + (t - 1) * n, rem.begin(), rem.end());
        }
        for (int i = t - 3; i >= 0; i--)
        {
            copy(a.begin() + i * n, a.begin() + (i + 1) * n, z.begin());
            copy(rem.begin(), rem.begin() + n, z.begin() + n);
            fill(rem.begin(), rem.end(), 0);
            div2n1n(z.begin(), z.end(), b.begin(), b.end(), q + i * n, q + (i + 1) * n, rem.begin(), rem.end());
        }
        copy(rem.begin() + digit, rem.begin() + digit + (re - r), r);
        shiftR(r, re, shift);
    }

    static pair<vector<uint32_t>, vector<uint32_t>> divVec(const vector<uint32_t> &x, const vector<uint32_t> &y)
    {
        // x、y 是去高零的小端 limb 数组且 y 非空；返回去高零后的无符号 {商,余数}，x<y 时为 {空,x}。
        assert(!y.empty());
        if (cmp(x, y) < 0)
        {
            return {{}, x};
        }
        vector<uint32_t> q(x.size() - y.size() + 1), r(y.size());
        divide(x.begin(), x.end(), y.begin(), y.end(), q.begin(), q.end(), r.begin(), r.end());
        trim(q);
        trim(r);
        return {q, r};
    }
};

// 十进制百万位整数：以 10^9 为基数，长乘法复用公共三模 NTT，除法使用 Newton 逆。
struct FastBigInt
{
    static constexpr int B = 1000000000; // 十进制分组基数。
    bool neg = false;                         // 是否为负数，零恒为非负。
    vector<int> a;                       // 绝对值的 B 进制小端块，无高位零；性能版用紧凑存储。

    FastBigInt()
    {
        // 无参数；构造整数零。
    }
    FastBigInt(long long x)
    {
        // x 是任意有符号 64 位整数；构造其精确值，包含 LLONG_MIN。
        neg = x < 0;
        // 无符号取绝对值，避免对 LLONG_MIN 做有符号取反。
        unsigned long long y = neg ? 0 - (unsigned long long)x : (unsigned long long)x;
        while (y)
        {
            a.push_back(y % B);
            y /= B;
        }
    }
    FastBigInt(const string &s)
    {
        // s 是带可选正负号的非空十进制串；构造其精确值。
        assign(s);
    }
    FastBigInt(bool s, vector<int> v) : neg(s), a(move(v))
    {
        // s 指定负号，v 是小端 B 进制块且各块小于 B；去高零并消除负零。
        trim(a);
        if (a.empty())
        {
            neg = false;
        }
    }

    FastBigInt &assign(const string &s)
    {
        // s 是带可选正负号的非空十进制串；覆盖当前值并返回 *this。
        assert(!s.empty());
        int p = (s[0] == '-' || s[0] == '+');
        neg = s[0] == '-';
        assert(p < (int)s.size());
        a.clear();
        for (int r = s.size(); r > p; r -= 9)
        {
            int l = max(p, r - 9);
            int v = 0;
            for (int i = l; i < r; i++)
            {
                assert('0' <= s[i] && s[i] <= '9');
                v = v * 10 + s[i] - '0';
            }
            a.push_back(v);
        }
        trim(a);
        if (a.empty())
        {
            neg = false;
        }
        return *this;
    }

    string toDecimal() const
    {
        // 无参数；返回无前导零的十进制串，负数带负号，零返回 "0"。
        if (a.empty())
        {
            return "0";
        }
        string s = neg ? "-" : "";
        s += to_string(a.back());
        for (int i = (int)a.size() - 2; i >= 0; i--)
        {
            string t = to_string(a[i]);
            s += string(9 - t.size(), '0') + t;
        }
        return s;
    }

    friend bool operator==(const FastBigInt &x, const FastBigInt &y)
    {
        // x、y 是规范整数；返回 x 是否相等 y。
        return x.neg == y.neg && x.a == y.a;
    }
    friend bool operator<(const FastBigInt &x, const FastBigInt &y)
    {
        // x、y 是规范整数；返回 x 是否小于 y。
        if (x.neg != y.neg)
        {
            return x.neg;
        }
        int c = cmp(x.a, y.a);
        return x.neg ? c > 0 : c < 0;
    }
    friend bool operator!=(const FastBigInt &x, const FastBigInt &y)
    {
        // x、y 是规范整数；返回 x 是否不等 y。
        return !(x == y);
    }
    friend bool operator>(const FastBigInt &x, const FastBigInt &y)
    {
        // x、y 是规范整数；返回 x 是否大于 y。
        return y < x;
    }
    friend bool operator<=(const FastBigInt &x, const FastBigInt &y)
    {
        // x、y 是规范整数；返回 x 是否小于等于 y。
        return !(y < x);
    }
    friend bool operator>=(const FastBigInt &x, const FastBigInt &y)
    {
        // x、y 是规范整数；返回 x 是否大于等于 y。
        return !(x < y);
    }

    friend FastBigInt operator-(const FastBigInt &x)
    {
        // x 是规范整数；返回相反数，零仍为非负。
        FastBigInt y = x;
        if (!y.a.empty())
        {
            y.neg = !y.neg;
        }
        return y;
    }
    friend FastBigInt operator+(const FastBigInt &x, const FastBigInt &y)
    {
        // x、y 是规范整数；返回和。
        if (x.neg == y.neg)
        {
            return {x.neg, addVec(x.a, y.a)};
        }
        int c = cmp(x.a, y.a);
        if (c <= 0)
        {
            return {c ? y.neg : false, subVec(y.a, x.a)};
        }
        return {x.neg, subVec(x.a, y.a)};
    }
    friend FastBigInt operator-(const FastBigInt &x, const FastBigInt &y)
    {
        // x、y 是规范整数；返回差。
        return x + (-y);
    }
    friend FastBigInt operator*(const FastBigInt &x, const FastBigInt &y)
    {
        // x、y 是规范整数；返回积。
        auto z = mulVec(x.a, y.a);
        return {z.empty() ? false : x.neg != y.neg, move(z)};
    }
    friend pair<FastBigInt, FastBigInt> divmod(const FastBigInt &x, const FastBigInt &y)
    {
        // x 是被除数，y 是非零除数；返回 {向零截断的商,与 x 同号的余数}，零非负。
        assert(!y.a.empty());
        auto z = divVec(x.a, y.a);
        return {{z.first.empty() ? false : x.neg != y.neg, move(z.first)},
                {z.second.empty() ? false : x.neg, move(z.second)}};
    }
    friend FastBigInt operator/(const FastBigInt &x, const FastBigInt &y)
    {
        // x、y 是规范整数且 y 非零；返回向零截断的商。
        return divmod(x, y).first;
    }
    friend FastBigInt operator%(const FastBigInt &x, const FastBigInt &y)
    {
        // x、y 是规范整数且 y 非零；返回与 x 同号的余数。
        return divmod(x, y).second;
    }

  private:
    static void trim(vector<int> &x)
    {
        // x 是小端 B 进制块；原地删除高位零，零变为空，无返回值。
        while (!x.empty() && x.back() == 0)
        {
            x.pop_back();
        }
    }
    static int cmp(const vector<int> &x, const vector<int> &y)
    {
        // x、y 是无高零的小端块；按绝对值比较，返回 -1、0 或 1。
        if (x.size() != y.size())
        {
            return x.size() < y.size() ? -1 : 1;
        }
        for (int i = (int)x.size() - 1; i >= 0; i--)
        {
            if (x[i] != y[i])
            {
                return x[i] < y[i] ? -1 : 1;
            }
        }
        return 0;
    }
    static vector<int> addVec(const vector<int> &x, const vector<int> &y)
    {
        // x、y 是规范非负小端块；返回去高零的和。
        vector<int> z(max(x.size(), y.size()) + 1);
        int carry = 0;
        for (int i = 0; i < (int)z.size() - 1; i++)
        {
            int v = carry + (i < (int)x.size() ? x[i] : 0) + (i < (int)y.size() ? y[i] : 0);
            z[i] = v % B;
            carry = v / B;
        }
        z.back() = carry;
        trim(z);
        return z;
    }
    static vector<int> subVec(const vector<int> &x, const vector<int> &y)
    {
        // x、y 是规范非负小端块且 x>=y；返回去高零的差。
        assert(cmp(x, y) >= 0);
        vector<int> z = x;
        int borrow = 0;
        for (int i = 0; i < (int)z.size(); i++)
        {
            int v = (int)z[i] - (i < (int)y.size() ? y[i] : 0) - borrow;
            if (v < 0)
            {
                v += B, borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            z[i] = v;
        }
        trim(z);
        return z;
    }
    static vector<int> mulSmall(const vector<int> &x, int m)
    {
        // x 是规范非负小端块，0<=m<B；返回去高零的 x*m。
        vector<int> z(x.size() + 1);
        int carry = 0;
        for (int i = 0; i < (int)x.size(); i++)
        {
            unsigned long long v = (unsigned long long)x[i] * m + carry;
            z[i] = v % B;
            carry = v / B;
        }
        z.back() = carry;
        trim(z);
        return z;
    }
    static pair<vector<int>, int> divSmall(const vector<int> &x, int d)
    {
        // x 是规范非负小端块，0<d<B；返回 {去高零的商,整型余数}。
        vector<int> q(x.size());
        int rem = 0;
        for (int i = (int)x.size() - 1; i >= 0; i--)
        {
            unsigned long long v = (unsigned long long)rem * B + x[i];
            q[i] = v / d;
            rem = v % d;
        }
        trim(q);
        return {q, (int)rem};
    }
    static vector<int> mulVec(const vector<int> &x, const vector<int> &y)
    {
        // x、y 是规范非负小端块；返回精确积，长乘法的卷积长度不得超过 2^24。
        if (x.empty() || y.empty())
        {
            return {};
        }
        if (min(x.size(), y.size()) <= 64)
        {
            // 竖式系数累计与 CRT 重建需要超过 64 位的非负中间量。
            vector<__uint128_t> c(x.size() + y.size());
            for (int i = 0; i < (int)x.size(); i++)
            {
                for (int j = 0; j < (int)y.size(); j++)
                {
                    c[i + j] += (__uint128_t)x[i] * y[j];
                }
            }
            vector<int> z(c.size());
            __uint128_t carry = 0; // 保存跨十进制块进位，不能缩成 64 位。
            for (int i = 0; i < (int)c.size(); i++)
            {
                __uint128_t v = c[i] + carry;
                z[i] = v % B;
                carry = v / B;
            }
            trim(z);
            return z;
        }
        // 三素数共同支持长度 2^24；该范围内整数卷积系数严格小于三模乘积。
        assert(x.size() + y.size() - 1 <= (1ULL << 24));
        constexpr int p0 = 167772161, p1 = 469762049, p2 = 754974721;
        constexpr int r0 = 3, r1 = 3, r2 = 11;
        vector<unsigned long long> a(x.begin(), x.end()), b(y.begin(), y.end());
        auto c0 = arbModConv::StaticNTT<p0, r0>::conv(a, b);
        auto c1 = arbModConv::StaticNTT<p1, r1>::conv(a, b);
        auto c2 = arbModConv::StaticNTT<p2, r2>::conv(a, b);
        int i01 = arbModConv::inv(p0 % p1, p1);
        int i012 = arbModConv::inv((unsigned long long)p0 * p1 % p2, p2);
        vector<int> z(c0.size());
        __uint128_t carry = 0; // 保存跨十进制块进位，不能缩成 64 位。
        for (int i = 0; i < (int)c0.size(); i++)
        {
            int u = c0[i];
            int v = ((c1[i] + p1 - u % p1) % p1) * (unsigned long long)i01 % p1;
            int seen = (u + (unsigned long long)p0 * v) % p2;
            int w = ((c2[i] + p2 - seen) % p2) * (unsigned long long)i012 % p2;
            __uint128_t cur = carry + u + (__uint128_t)p0 * v + (__uint128_t)p0 * p1 * w;
            z[i] = cur % B;
            carry = cur / B;
        }
        while (carry)
        {
            z.push_back(carry % B);
            carry /= B;
        }
        trim(z);
        return z;
    }
    static pair<vector<int>, vector<int>> divNaive(const vector<int> &x, const vector<int> &y)
    {
        // x、y 是规范非负小端块且 y 非零；竖式除法返回 {商,余数}。
        if (x.empty() || cmp(x, y) < 0)
        {
            return {{}, x};
        }
        if (y.size() == 1)
        {
            auto [q, r] = divSmall(x, y[0]);
            return {q, r ? vector<int>{r} : vector<int>{}};
        }
        int norm = B / (y.back() + 1);
        vector<int> a = mulSmall(x, norm), b = mulSmall(y, norm);
        vector<int> q(a.size() - b.size() + 1), r(a.end() - b.size(), a.end());
        for (int i = (int)q.size() - 1; i >= 0; i--)
        {
            unsigned long long top = r.size() > b.size() ? (unsigned long long)r.back() * B + r[r.size() - 2] : 0;
            int d = top ? min<unsigned long long>(B - 1, top / b.back()) : 1;
            vector<int> z = mulSmall(b, d);
            while (cmp(r, z) < 0)
            {
                d--, z = subVec(z, b);
            }
            r = subVec(r, z);
            while (cmp(r, b) >= 0)
            {
                d++, r = subVec(r, b);
            }
            q[i] = d;
            if (i)
            {
                r.insert(r.begin(), a[i - 1]);
            }
            trim(r);
        }
        auto rr = divSmall(r, norm);
        assert(rr.second == 0);
        trim(q);
        return {q, rr.first};
    }
    static vector<int> calcInv(const vector<int> &a, int deg)
    {
        // a 是最高块至少 B/2 的规范正整数，deg>0；返回 B^(|a|+deg)/a 的整数近似逆。
        int k = deg, c = a.size();
        while (k > 64)
        {
            k = (k + 1) / 2;
        }
        vector<int> z(c + k + 1);
        z.back() = 1;
        z = divNaive(z, a).first;
        while (k < deg)
        {
            vector<int> s = mulVec(z, z);
            s.insert(s.begin(), 0);
            vector<int> t(2 * k + 1);
            int take = min<int>(a.size(), t.size());
            copy(a.end() - take, a.end(), t.end() - take);
            vector<int> u = mulVec(s, t);
            if ((int)u.size() > 2 * k + 1)
            {
                u.erase(u.begin(), u.begin() + 2 * k + 1);
            }
            else
            {
                u.clear();
            }
            vector<int> w(k + 1), w2 = addVec(z, z);
            copy(w2.begin(), w2.end(), back_inserter(w));
            z = subVec(w, u);
            if (!z.empty())
            {
                z.erase(z.begin());
            }
            k *= 2;
        }
        if (k > deg)
        {
            z.erase(z.begin(), z.begin() + k - deg);
        }
        return z;
    }
    static pair<vector<int>, vector<int>> divVec(const vector<int> &x, const vector<int> &y)
    {
        // x、y 是规范非负小端块且 y 非零；返回精确 {商,余数}，长除用 Newton 逆估商后修正。
        assert(!y.empty());
        if (cmp(x, y) < 0)
        {
            return {{}, x};
        }
        if (y.size() <= 64 || x.size() - y.size() <= 64)
        {
            return divNaive(x, y);
        }
        int norm = B / (y.back() + 1);
        vector<int> a = mulSmall(x, norm), b = mulSmall(y, norm);
        int deg = a.size() - b.size() + 2;
        vector<int> inv = calcInv(b, deg);
        vector<int> q = mulVec(a, inv);
        if ((int)q.size() > (int)b.size() + deg)
        {
            q.erase(q.begin(), q.begin() + b.size() + deg);
        }
        else
        {
            q.clear();
        }
        vector<int> prod = mulVec(b, q);
        while (cmp(a, prod) < 0)
        {
            q = subVec(q, {1}), prod = subVec(prod, b);
        }
        vector<int> r = subVec(a, prod);
        while (cmp(r, b) >= 0)
        {
            q = addVec(q, {1}), r = subVec(r, b);
        }
        auto rr = divSmall(r, norm);
        assert(rr.second == 0);
        trim(q);
        return {q, rr.first};
    }
};
