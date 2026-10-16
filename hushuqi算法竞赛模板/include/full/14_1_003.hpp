#pragma once

#include "14_1_001.hpp"

struct Expr
{
    struct Node
    {
        string tok;               // 数字串、二元运算符，或折叠连续符号后的一元 u+、u-。
        array<int, 2> ch{-1, -1}; // [0] 左或唯一孩子，[1] 右孩子；-1 表示没有。
    };
    vector<Node> a; // 后序排列的语法树节点；孩子编号小于父节点编号，仅读取。
    int rt = -1;    // 根编号；未成功解析时为 -1。

private:
    string s;  // 当前中缀表达式。
    int p = 0; // 下一个待读字符的位置。

    void skip()
    {
        // 无参数；把 p 移过连续空白，无返回值。
        while (p < (int)s.size() && isspace((unsigned char)s[p]))
        {
            p++;
        }
    }

    int expr()
    {
        // 无参数；解析当前位置的加减表达式，返回根编号；语法错误返回 -1。
        int x = term();
        skip();
        while (x >= 0 && p < (int)s.size() && (s[p] == '+' || s[p] == '-'))
        {
            string op(1, s[p++]);
            int y = term();
            if (y < 0) return -1;
            a.push_back({op, {x, y}});
            x = (int)a.size() - 1;
            skip();
        }
        return x;
    }

    int term()
    {
        // 无参数；解析当前位置的乘除项，返回根编号；语法错误返回 -1。
        int x = atom();
        skip();
        while (x >= 0 && p < (int)s.size() && (s[p] == '*' || s[p] == '/'))
        {
            string op(1, s[p++]);
            int y = atom();
            if (y < 0) return -1;
            a.push_back({op, {x, y}});
            x = (int)a.size() - 1;
            skip();
        }
        return x;
    }

    int atom()
    {
        // 无参数；解析一元符号及括号或数字，返回根编号；语法错误返回 -1。
        skip();
        bool sign = false, neg = false;
        while (p < (int)s.size() && (s[p] == '+' || s[p] == '-'))
        {
            sign = true;
            neg ^= s[p++] == '-';
            skip();
        }
        if (p == (int)s.size()) return -1;
        int x;
        if (s[p] == '(')
        {
            p++;
            x = expr();
            if (x < 0 || p == (int)s.size() || s[p] != ')') return -1;
            p++;
        }
        else
        {
            int l = p;
            while (p < (int)s.size() && s[p] >= '0' && s[p] <= '9')
            {
                p++;
            }
            if (l == p) return -1;
            a.push_back({s.substr(l, p - l), {-1, -1}});
            x = (int)a.size() - 1;
        }
        if (sign)
        {
            a.push_back({neg ? "u-" : "u+", {x, -1}});
            x = (int)a.size() - 1;
        }
        return x;
    }

public:
    bool parse(const string &t)
    {
        // t 是中缀表达式；成功时保存 a、rt 并返回 true，语法错误时清空树并返回 false。
        s = t;
        p = 0;
        a.clear();
        rt = expr();
        skip();
        if (rt < 0 || p != (int)s.size())
        {
            a.clear();
            rt = -1;
            return false;
        }
        return true;
    }

    vector<string> postfix() const
    {
        // 无参数；返回当前树的后缀记号序列，一元运算记作 u+、u-；无树时返回空序列。
        vector<string> res;
        for (const auto &v : a)
        {
            res.push_back(v.tok);
        }
        return res;
    }

    optional<int> value() const
    {
        // 无参数；返回当前树在 int 范围内的整数值；无树、除零或运算结果越界时返回空。
        // __int128 承载字面量幅值和乘法中间量，关闭整数宏后仍显式加宽。
        if (rt < 0) return nullopt;
        vector<__int128> val(a.size());
        for (int i = 0; i < (int)a.size(); i++)
        {
            const auto &[tok, ch] = a[i];
            auto &v = val[i];
            if (ch[0] < 0)
            {
                for (char c : tok)
                {
                    v = v * 10 + c - '0';
                    if (v > (__int128)numeric_limits<int>::max() + 1) return nullopt;
                }
                continue;
            }
            __int128 x = val[ch[0]];
            if (ch[1] < 0)
            {
                v = tok == "u-" ? -x : x;
            }
            else
            {
                __int128 y = val[ch[1]];
                if (x > numeric_limits<int>::max() || y > numeric_limits<int>::max())
                    return nullopt;
                if (tok == "+") v = x + y;
                if (tok == "-") v = x - y;
                if (tok == "*") v = x * y;
                if (tok == "/")
                {
                    if (!y) return nullopt;
                    v = x / y;
                }
            }
            if (v < numeric_limits<int>::min() || v > numeric_limits<int>::max()) return nullopt;
        }
        if (val[rt] > numeric_limits<int>::max()) return nullopt;
        return (int)val[rt];
    }

    optional<int> eval(const string &t)
    {
        // t 是中缀表达式；解析后返回当前树的值，语法、除零或数值错误时返回空。
        if (!parse(t)) return nullopt;
        return value();
    }
};
