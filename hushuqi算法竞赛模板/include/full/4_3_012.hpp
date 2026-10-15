#pragma once
#include "4_3_002.hpp"

struct FMIndex
{
    // sa 是带终止符文本的后缀数组；bwt[i] 是第 i 小后缀前面的那个符号。
    // 字节 x 编码为 x+1，唯一的终止符编码为 0。
    vector<int> sa, bwt;
    // c[x] 是 bwt 中小于 x 的符号个数；at[x] 是符号 x 在 bwt 中出现的行号，升序。
    array<int, 258> c{};
    array<vector<int>, 257> at;

    FMIndex(const string &text)
    {
        // text 是任意字节串；建立后缀数组、BWT 与按符号分组的出现位置。
        int n = text.size() + 1;
        vector<int> s(n);
        for (int i = 0; i + 1 < n; i++)
        {
            s[i] = (unsigned char)text[i] + 1;
        }
        sa = suffixArray(s);
        bwt.resize(n);
        for (int i = 0; i < n; i++)
        {
            bwt[i] = s[(sa[i] + n - 1) % n];
            at[bwt[i]].push_back(i);
            c[bwt[i] + 1]++;
        }
        for (int x = 1; x < 258; x++)
        {
            c[x] += c[x - 1];
        }
    }

    int rank(int x, int r) const
    {
        // x 是符号编码，r 是行号上界；返回 bwt[0..r) 中 x 的出现次数。
        return lower_bound(at[x].begin(), at[x].end(), r) - at[x].begin();
    }

    int lf(int i) const
    {
        // i 是行号；返回把该行循环右移一位后所在的行（LF 映射）。
        return c[bwt[i]] + rank(bwt[i], i);
    }

    pair<int, int> search(const string &p) const
    {
        // p 是模式串；返回后缀数组中以 p 为前缀的行区间 [l,r)，不出现时 l==r。
        int l = 0, r = sa.size();
        for (int i = (int)p.size() - 1; i >= 0 && l < r; i--)
        {
            int x = (unsigned char)p[i] + 1;
            l = c[x] + rank(x, l);
            r = c[x] + rank(x, r);
        }
        return {l, r};
    }

    vector<int> locate(const string &p) const
    {
        // p 是模式串；返回 p 在原文中全部出现的起点，升序；空模式返回 0..|text|。
        auto [l, r] = search(p);
        vector<int> ans(sa.begin() + l, sa.begin() + r);
        sort(ans.begin(), ans.end());
        return ans;
    }

    string restore() const
    {
        // 只用 bwt 沿 LF 映射倒推，返回原文。
        int n = bwt.size(), i = at[0][0];
        string s(n - 1, '\0');
        for (int k = n - 2; k >= 0; k--)
        {
            i = lf(i);
            s[k] = (char)(bwt[i] - 1);
        }
        return s;
    }
};
