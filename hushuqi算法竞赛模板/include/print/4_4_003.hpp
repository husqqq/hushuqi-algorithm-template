#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <int A = 26, char S = 'a'> struct PamSeries
{
    struct Node
    {
        array<int, A> ch{};
        // diff=len-len[link]，series 指向上一条不同公差的系列。
        int len = 0, link = 0, diff = 0, series = 0;
    };

    vector<Node> t{{{}, -1, 0, 0, 0}, {{}, 0, 0, 0, 0}};
    vector<int> s{-1};
    // last 是当前最长回文后缀节点。
    int last = 1;

    int get(int p) const
    {
        // p 是一个回文节点；返回能被当前末字符继续扩展的后缀回文节点。
        int n = s.size() - 1;
        while (s[n - t[p].len - 1] != s[n])
        {
            p = t[p].link;
        }
        return p;
    }

    int add(char c)
    {
        // c 是追加到原串末尾的字符；返回新的最长回文后缀节点。
        int x = c - S;
        assert(0 <= x && x < A); // 调试检查，可删
        s.push_back(x);
        int p = get(last);
        if (!t[p].ch[x])
        {
            int q = t.size();
            t.push_back({{}, t[p].len + 2});
            t[q].link = t[q].len == 1 ? 1 : t[get(t[p].link)].ch[x];
            t[q].diff = t[q].len - t[t[q].link].len;
            t[q].series = t[q].diff == t[t[q].link].diff ? t[t[q].link].series : t[q].link;
            t[p].ch[x] = q;
        }
        return last = t[p].ch[x];
    }

    template <class F> void eachSeries(int p, F f) const
    {
        // p 是当前最长回文后缀，f 接收每条等差系列的首节点。
        for (; t[p].len > 0; p = t[p].series)
        {
            f(p);
        }
    }
};
