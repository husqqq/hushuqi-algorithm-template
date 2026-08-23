#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class Val = int> struct SiblingTrie
{
    struct Node
    {
        char c = 0;
        int child = 0;
        int next = 0;
        int pass = 0;
        int end = 0;
        Val val{};
    };

    // 根为 0；非根节点同时充当字符边，child/next 构成儿子兄弟链。
    vector<Node> tr{Node{}};

    int newNode(char c)
    {
        tr.push_back(Node{});
        tr.back().c = c;
        return (int)tr.size() - 1;
    }

    int childOf(int p, char c) const
    {
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
