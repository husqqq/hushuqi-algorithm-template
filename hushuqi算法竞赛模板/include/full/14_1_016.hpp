#pragma once

#include "14_1_001.hpp"

string trimNum(string s)
{
    // s 是非空十进制数字串；返回删除多余前导零后的规范表示。
    size_t p = s.find_first_not_of('0');
    return p == string::npos ? "0" : s.substr(p);
}

int cmpNum(const string &a, const string &b)
{
    // a、b 是规范非负十进制数；返回 -1、0、1 表示大小关系。
    if (a.size() != b.size())
    {
        return a.size() < b.size() ? -1 : 1;
    }
    if (a == b)
    {
        return 0;
    }
    return a < b ? -1 : 1;
}

string mulSmall(const string &a, int b)
{
    // a 是规范非负十进制数，b 在 [0,100]；返回精确乘积。
    if (b == 0 || a == "0")
    {
        return "0";
    }
    string c;
    int carry = 0;
    for (int i = (int)a.size() - 1; i >= 0; i--)
    {
        int v = (a[i] - '0') * b + carry;
        c.push_back(char('0' + v % 10));
        carry = v / 10;
    }
    while (carry)
    {
        c.push_back(char('0' + carry % 10));
        carry /= 10;
    }
    reverse(c.begin(), c.end());
    return c;
}

string addSmall(string a, int b)
{
    // a 是规范非负十进制数，b 在 [0,99]；返回精确和。
    for (int i = (int)a.size() - 1; i >= 0 && b; i--)
    {
        int v = a[i] - '0' + b;
        a[i] = char('0' + v % 10);
        b = v / 10;
    }
    while (b)
    {
        a.insert(a.begin(), char('0' + b % 10));
        b /= 10;
    }
    return trimNum(a);
}

string subNum(string a, const string &b)
{
    // a、b 是规范非负十进制数且 a>=b；返回精确差。
    // 调试检查，可删。
    assert(cmpNum(a, b) >= 0);
    int borrow = 0;
    int j = (int)b.size() - 1;
    for (int i = (int)a.size() - 1; i >= 0; i--)
    {
        int v = a[i] - '0' - borrow - (j >= 0 ? b[j] - '0' : 0);
        borrow = v < 0;
        if (borrow)
        {
            v += 10;
        }
        a[i] = char('0' + v);
        j--;
    }
    return trimNum(a);
}

string bigSqrt(string s)
{
    // s 是无符号非负十进制整数；返回 floor(sqrt(s)) 的规范十进制表示。
    // 调试检查，可删。
    assert(!s.empty());
    for (char c : s)
    {
        // 调试检查，可删。
        assert('0' <= c && c <= '9');
    }
    s = trimNum(s);
    if (s == "0")
    {
        return "0";
    }
    if (s.size() & 1)
    {
        s = "0" + s;
    }
    string root = "0";
    string rem = "0";
    for (int i = 0; i < (int)s.size(); i += 2)
    {
        rem = trimNum(rem + s.substr(i, 2));
        string base = mulSmall(root, 20);
        int d = 9;
        while (cmpNum(mulSmall(addSmall(base, d), d), rem) > 0)
        {
            d--;
        }
        rem = subNum(rem, mulSmall(addSmall(base, d), d));
        root = addSmall(mulSmall(root, 10), d);
    }
    return root;
}
