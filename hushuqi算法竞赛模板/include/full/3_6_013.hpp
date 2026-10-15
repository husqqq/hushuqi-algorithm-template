#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class Val = int> struct SiblingTrie
{
    struct Node
    {
        char c = 0; // c 是从父亲走到本节点的字符。
        int child = 0; // child 是第一个儿子，0 表示没有。
        int next = 0; // next 是下一个兄弟，0 表示没有。
        int pass = 0; // pass 是经过本节点（以其为前缀）的串数。
        int end = 0; // end 是恰好在本节点结束的串数。
        Val val{}; // val 是最后一次插入在此结束的串时附带的值。
    };

    // 根为 0；非根节点同时充当字符边，child/next 构成儿子兄弟链。
    vector<Node> tr{Node{}};

    int newNode(char c)
    {
        // c 是边上的字符；新建节点并返回编号。
        tr.push_back(Node{});
        tr.back().c = c;
        return (int)tr.size() - 1;
    }

    int childOf(int p, char c) const
    {
        // p 是节点，c 是字符；顺兄弟链查找字符为 c 的儿子，不存在返回 0。
        for (int q = tr[p].child; q; q = tr[q].next)
        {
            if (tr[q].c == c)
            {
                return q;
            }
        }
        return 0;
    }

    int insert(string_view s, Val v = Val(1))
    {
        // s 是待插入串，v 是附带值；沿途 pass 加一、终点 end 加一并记录 v，返回终点编号。
        int p = 0;
        tr[p].pass++;
        for (char c : s)
        {
            int q = childOf(p, c);
            if (!q)
            {
                q = newNode(c);
                tr[q].next = tr[p].child;
                tr[p].child = q;
            }
            p = q;
            tr[p].pass++;
        }
        tr[p].end++;
        tr[p].val = v;
        return p;
    }

    int find(string_view s) const
    {
        // s 是查询串；返回 s 对应的节点编号，s 不是任何插入串的前缀时返回 -1。
        int p = 0;
        for (char c : s)
        {
            p = childOf(p, c);
            if (!p)
            {
                return -1;
            }
        }
        return p;
    }

    int lcp(string_view s) const
    {
        // s 是查询串；返回 s 与全部插入串的最长公共前缀长度。
        int p = 0, len = 0;
        for (char c : s)
        {
            p = childOf(p, c);
            if (!p)
            {
                break;
            }
            len++;
        }
        return len;
    }
};
