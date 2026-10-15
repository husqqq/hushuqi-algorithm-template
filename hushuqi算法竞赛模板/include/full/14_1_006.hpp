#pragma once

#include "14_1_001.hpp"

string convertBase(string s, int from, int to)
{
    // s 是字符序 0-9A-Za-z 下带可选正负号的 from 进制整数；返回同字符序、无前导零的 to 进制字符串。
    // 调试检查，可删。
    assert(!s.empty());
    // 调试检查，可删。
    assert(2 <= from && from <= 62 && 2 <= to && to <= 62);
    const string digit = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    bool neg = s[0] == '-';
    int p = neg || s[0] == '+';
    // 调试检查，可删。
    assert(p < (int)s.size());
    vector<int> a;
    for (; p < (int)s.size(); p++)
    {
        size_t pos = digit.find(s[p]);
        // 调试检查，可删。
        assert(pos != string::npos && pos < (size_t)from);
        int x = pos;
        a.push_back(x);
    }
    while (a.size() > 1 && a[0] == 0)
    {
        a.erase(a.begin());
    }
    string ans;
    while (!(a.size() == 1 && a[0] == 0))
    {
        vector<int> b;
        int rem = 0;
        for (int x : a)
        {
            int cur = rem * from + x;
            if (!b.empty() || cur / to)
            {
                b.push_back(cur / to);
            }
            rem = cur % to;
        }
        ans.push_back(digit[rem]);
        a = b.empty() ? vector<int>{0} : move(b);
    }
    if (ans.empty())
    {
        return "0";
    }
    reverse(ans.begin(), ans.end());
    return neg ? "-" + ans : ans;
}

BigInt bigPow(BigInt a, int e)
{
    // a 是任意精度整数，e 是非负整数；返回 a^e，依赖 14.1.001 的 BigInt 乘法。
    // 调试检查，可删。
    assert(e >= 0);
    BigInt ans = 1;
    while (e)
    {
        if (e & 1)
        {
            ans *= a;
        }
        e >>= 1;
        if (e)
        {
            a *= a;
        }
    }
    return ans;
}
