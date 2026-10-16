#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <int A = 26, char S = 'a'> struct DequePAM
{
    static_assert(1 <= A && (unsigned char)S + A <= 256);

  private:
    struct Node
    {
        // ch[c] 是两侧各加字符 c 的转移，0 表示不存在。
        // 性能版以 32 位压缩节点；操作期间串长小于 2^31-2，保证编号、长度、计数可存。
        array<int, A> ch{};
        int par = 0;   // 去掉两端字符后的转移父节点。
        int link = 0;  // 最长真回文后缀，与最长真回文前缀相同。
        int quick = 0; // 跳到前导字符与 link 不同的最长真回文后缀。
        int len = 0;   // 回文长度；奇根为 -1，偶根为 0。
        int cnt = 0;   // 以本节点为最长回文后缀的前缀个数，也等于以它为最长回文前缀的后缀个数。
        int sons = 0;  // 仍存在的后缀链接孩子数。
    };
    vector<Node> t;        // 节点池；0 为奇根，1 为偶根。
    vector<int> spare; // 已回收的节点编号，复用时清空。
    // 每项依次为 {字符编号, 从此处开始的表面回文节点, 到此处结束的表面回文节点}；1 表示无表面。
    deque<array<int, 3>> d;
    int kinds = 0; // 当前本质不同非空回文数。

    int newNode()
    {
        // 无参数；返回清空的可用节点编号，优先复用已回收节点。
        if (spare.empty())
        {
            t.push_back({});
            return (int)t.size() - 1;
        }
        int p = spare.back();
        spare.pop_back();
        t[p] = Node{};
        return p;
    }

    int extend(int c, int p, bool front) const
    {
        // c 是待加字符编号，p 是待扩展回文，front 选择左端；返回最长可扩展的前/后缀节点。
        int n = d.size();
        while (true)
        {
            if (t[p].len == -1 || (t[p].len < n && d[front ? t[p].len : n - t[p].len - 1][0] == c))
            {
                return p;
            }
            int q = t[p].link;
            if (t[q].len == -1 || (t[q].len < n && d[front ? t[q].len : n - t[q].len - 1][0] == c))
            {
                return q;
            }
            p = t[p].quick;
        }
    }

    void push(char c, bool front)
    {
        // c 是新字符，front 选择左端；更新转移、表面与间接出现计数，无返回值。
        int x = (unsigned char)c - (unsigned char)S;
        assert(0 <= x && x < A);
        assert(d.size() < (1ULL << 31) - 2);
        int par = d.empty() ? 0 : extend(x, front ? d.front()[1] : d.back()[2], front);
        int v = t[par].ch[x];
        bool fresh = !v;
        if (fresh)
        {
            v = newNode();
            t[v].par = par;
            t[v].len = t[par].len + 2;
            t[v].link = par == 0 ? 1 : t[extend(x, t[par].link, front)].ch[x];
            assert(t[v].link);
            t[t[v].link].sons++;
            t[par].ch[x] = v;
            kinds++;
        }
        if (front)
        {
            d.push_front({x, 1, 1});
        }
        else
        {
            d.push_back({x, 1, 1});
        }
        int n = d.size(), w = t[v].link;
        if (fresh)
        {
            int q = t[w].link;
            int i = front ? t[w].len : n - t[w].len - 1;
            int j = front ? t[q].len : n - t[q].len - 1;
            t[v].quick = q != 0 && d[i][0] == d[j][0] ? t[w].quick : q;
        }
        int l = front ? 0 : n - t[v].len, r = l + t[v].len - 1;
        d[l][1] = v;
        d[r][2] = v;
        // 新表面覆盖其最长真回文前/后缀；另一端记录已被上面的赋值覆盖。
        int k = front ? t[v].len - t[w].len : n - t[v].len + t[w].len - 1;
        if (t[w].len >= 1 && d[k][front ? 1 : 2] == w)
        {
            d[k][front ? 1 : 2] = 1;
        }
        t[v].cnt++;
    }

    void pop(bool front)
    {
        // front 选择删除左端，否则删除右端；要求非空，恢复新暴露的表面并回收消失节点。
        assert(!d.empty());
        int n = d.size(), v = front ? d.front()[1] : d.back()[2];
        int x = front ? d.front()[0] : d.back()[0], w = t[v].link;
        int l = front ? 0 : n - t[v].len, r = l + t[v].len - 1;
        int k = front ? t[v].len - t[w].len : n - t[v].len + t[w].len - 1;
        if (t[v].len >= 2 && t[d[k][front ? 1 : 2]].len < t[w].len)
        {
            d[k][front ? 1 : 2] = w;
            d[front ? r : l][front ? 2 : 1] = w;
        }
        else
        {
            d[front ? r : l][front ? 2 : 1] = 1;
        }
        t[v].cnt--;
        if (!t[v].sons && !t[v].cnt)
        {
            t[t[v].par].ch[x] = 0;
            t[w].sons--;
            spare.push_back(v);
            kinds--;
        }
        if (front)
        {
            d.pop_front();
        }
        else
        {
            d.pop_back();
        }
    }

  public:
    DequePAM() : t(2)
    {
        // 无参数；建立空串及长度 -1、0 的两个根。
        t[0].len = -1;
    }
    void pushFront(char c)
    {
        // c 属于指定连续字节域；从左端加入，无返回值。
        push(c, true);
    }
    void pushBack(char c)
    {
        // c 属于指定连续字节域；从右端加入，无返回值。
        push(c, false);
    }
    void popFront()
    {
        // 无参数；删除左端字符，要求非空，无返回值。
        pop(true);
    }
    void popBack()
    {
        // 无参数；删除右端字符，要求非空，无返回值。
        pop(false);
    }
    int size() const
    {
        // 无参数；返回当前字符串长度。
        return d.size();
    }
    int distinct() const
    {
        // 无参数；返回本质不同非空回文子串数。
        return kinds;
    }
    int longPrefix() const
    {
        // 无参数；返回最长回文前缀长度，空串返回 0。
        return d.empty() ? 0 : t[d.front()[1]].len;
    }
    int longSuffix() const
    {
        // 无参数；返回最长回文后缀长度，空串返回 0。
        return d.empty() ? 0 : t[d.back()[2]].len;
    }
};
