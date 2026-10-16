#pragma once

#include "14_1_001.hpp"

string convertBase(const string &s, int from, int to)
{
    // s 是字符序 0-9A-Za-z 下带可选正负号的 from 进制整数；返回同字符序、无前导零的 to 进制字符串。
    assert(!s.empty()); // 调试检查，可删。
    assert(2 <= from && from <= 62 && 2 <= to && to <= 62); // 调试检查，可删。
    const string digit = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    bool neg = s[0] == '-';
    int p = neg || s[0] == '+';
    assert(p < (int)s.size()); // 调试检查，可删。
    vector<int> a;
    for (; p < (int)s.size(); p++)
    {
        size_t pos = digit.find(s[p]);
        assert(pos != string::npos && pos < (size_t)from); // 调试检查，可删。
        if (pos || !a.empty()) a.push_back((int)pos);
    }
    if (a.empty()) return "0";
    string ans;
    if (from == to)
    {
        for (int x : a)
        {
            ans.push_back(digit[x]);
        }
        return neg ? "-" + ans : ans;
    }
    if (has_single_bit((unsigned)from) && has_single_bit((unsigned)to))
    {
        int fb = countr_zero((unsigned)from), tb = countr_zero((unsigned)to);
        int buf = 0, bits = 0; // buf 保存尚未输出的低位，bits 是其有效位数。
        for (int i = (int)a.size() - 1; i >= 0; i--)
        {
            buf |= a[i] << bits;
            bits += fb;
            while (bits >= tb)
            {
                ans.push_back(digit[buf & (to - 1)]);
                buf >>= tb;
                bits -= tb;
            }
        }
        if (bits) ans.push_back(digit[buf]);
        while (ans.size() > 1 && ans.back() == '0')
        {
            ans.pop_back();
        }
    }
    else
    {
        while (!a.empty())
        {
            int rem = 0, k = 0; // k 是本轮商的有效位数，商原地覆盖已读数码。
            for (int x : a)
            {
                int cur = rem * from + x;
                if (cur / to || k) a[k++] = cur / to;
                rem = cur % to;
            }
            ans.push_back(digit[rem]);
            a.resize(k);
        }
    }
    reverse(ans.begin(), ans.end());
    return neg ? "-" + ans : ans;
}

BigInt bigPow(BigInt a, int e)
{
    // a 是任意精度整数，e 是非负整数；返回 a^e，依赖 14.1.001 的 BigInt 乘法。
    assert(e >= 0); // 调试检查，可删。
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
