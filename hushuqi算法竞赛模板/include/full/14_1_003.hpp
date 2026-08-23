#pragma once

#include "14_1_001.hpp"

struct Expr
{
    // s 保存待求值表达式。
    string s;
    // p 保存下一个待读字符的位置。
    int p = 0;

    void skip()
    {
        while (p < (int)s.size() && isspace((unsigned char)s[p]))
        {
            p++;
        }
    }

    static bool take(__int128 v, long long &x)
    {
        // v 是中间结果，x 接收 64 位值；可表示时写入 x 并返回 true。
        if (v < LLONG_MIN || v > LLONG_MAX)
        {
            return false;
        }
        x = (long long)v;
        return true;
    }

    bool expr(long long &x)
    {
        // x 接收当前位置起的加减表达式值；语法或数值失败时返回 false。
        if (!term(x))
        {
            return false;
        }
        skip();
        while (p < (int)s.size() && (s[p] == '+' || s[p] == '-'))
        {
            char op = s[p++];
            long long y;
            if (!term(y))
            {
                return false;
            }
            __int128 v = op == '+' ? (__int128)x + y : (__int128)x - y;
            if (!take(v, x))
            {
                return false;
            }
            skip();
        }
        return true;
    }

    bool term(long long &x)
    {
        // x 接收当前位置起的乘除项值；语法或数值失败时返回 false。
        if (!atom(x))
        {
            return false;
        }
        skip();
        while (p < (int)s.size() && (s[p] == '*' || s[p] == '/'))
        {
            char op = s[p++];
            long long y;
            if (!atom(y) || (op == '/' && !y))
            {
                return false;
            }
            skip();
            __int128 v = op == '*' ? (__int128)x * y : (__int128)x / y;
            if (!take(v, x))
            {
                return false;
            }
        }
        return true;
    }

    bool atom(long long &x)
    {
        // x 接收当前位置起的一元表达式值；语法或数值失败时返回 false。
        skip();
        bool neg = false;
        while (p < (int)s.size() && (s[p] == '+' || s[p] == '-'))
        {
            neg ^= s[p] == '-';
            p++;
            skip();
        }
        if (p >= (int)s.size())
        {
            return false;
        }
        if (s[p] == '(')
        {
            p++;
            if (!expr(x) || p >= (int)s.size() || s[p] != ')')
            {
                return false;
            }
            p++;
            return !neg || take(-(__int128)x, x);
        }
        if (s[p] < '0' || s[p] > '9')
        {
            return false;
        }
        __int128 v = 0;
        while (p < (int)s.size() && s[p] >= '0' && s[p] <= '9')
        {
            v = v * 10 + s[p++] - '0';
            if (v > (__int128)LLONG_MAX + 1)
            {
                return false;
            }
        }
        if (neg)
        {
            v = -v;
        }
        return take(v, x);
    }

    optional<long long> eval(const string &t)
    {
        // t 是待求值表达式；成功时返回整数值，语法或数值失败时返回空。
        s = t;
        p = 0;
        long long x;
        if (!expr(x))
        {
            return nullopt;
        }
        skip();
        if (p != (int)s.size())
        {
            return nullopt;
        }
        return x;
    }
};
