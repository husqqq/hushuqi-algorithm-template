#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

int editDist(const string &a, const string &b)
{
    // a、b 是字节串；返回把 a 改成 b 的最少插入、删除、替换次数。
    // row[j] 滚动保存 a 前 i 个字节与 b 前 j 个字节的编辑距离。
    vector<int> row(b.size() + 1);
    iota(row.begin(), row.end(), 0);
    for (int i = 0; i < (int)a.size(); i++)
    {
        int pre = row[0]; // pre 是上一行的 row[j]，即左上角的值。
        row[0] = i + 1;
        for (int j = 0; j < (int)b.size(); j++)
        {
            int old = row[j + 1];
            row[j + 1] = min({row[j + 1] + 1, row[j] + 1, pre + (a[i] != b[j])});
            pre = old;
        }
    }
    return row.back();
}

int myersDist(const string &p, const string &t)
{
    // p 是至多 64 字节的串，t 是任意字节串；返回二者的编辑距离，与 editDist(p,t) 相同。
    int n = p.size();
    if (!n)
    {
        return t.size();
    }
    assert(n <= 64); // 调试检查，可删。
    // eq[c] 的第 i 位表示 p[i]==c；vp、vn 是 DP 当前列竖向差为 +1、-1 的位集。
    array<unsigned long long, 256> eq{};
    for (int i = 0; i < n; i++)
    {
        eq[(unsigned char)p[i]] |= 1ULL << i;
    }
    unsigned long long vp = ~0ULL, vn = 0, top = 1ULL << (n - 1);
    int ans = n; // ans 是当前列最后一行的值，即 p 与 t 已读前缀的编辑距离。
    for (unsigned char c : t)
    {
        unsigned long long x = eq[c] | vn;
        unsigned long long d = (((eq[c] & vp) + vp) ^ vp) | eq[c] | vn;
        unsigned long long hp = vn | ~(d | vp), hn = vp & d;
        if (hp & top)
        {
            ans++;
        }
        if (hn & top)
        {
            ans--;
        }
        hp = (hp << 1) | 1;
        hn <<= 1;
        vp = hn | ~(x | hp);
        vn = d & hp;
    }
    return ans;
}
