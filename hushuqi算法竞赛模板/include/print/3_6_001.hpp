#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <int A = 26, char S = 'a'> struct Trie
{
    // ch[u][c] 是节点 u 沿字符 S+c 的儿子。
    vector<array<int, A>> ch;
    // cnt[u] 是经过 u 的插入次数，end[u] 是恰在 u 结束的字符串数。
    vector<int> cnt, end;

    Trie()
    {
        // 建立表示空前缀的根节点 0。
        newNode();
    }

    int newNode()
    {
        // 新建全零节点并返回编号。
        ch.push_back({});
        cnt.push_back(0);
        end.push_back(0);
        return (int)ch.size() - 1;
    }

    int insert(const string &s)
    {
        // s 是字符均位于 [S,S+A) 的字符串；插入一份并返回终点节点。
        int p = 0;
        cnt[p]++;
        for (char c : s)
        {
            int x = c - S;
            assert(0 <= x && x < A); // 调试检查，可删
            if (!ch[p][x])
            {
                ch[p][x] = newNode();
            }
            p = ch[p][x];
            cnt[p]++;
        }
        end[p]++;
        return p;
    }

    int find(const string &s) const
    {
        // s 是查询串；返回其前缀节点，不存在或字符越界时返回 -1。
        int p = 0;
        for (char c : s)
        {
            int x = c - S;
            if (x < 0 || x >= A || !ch[p][x])
            {
                return -1;
            }
            p = ch[p][x];
        }
        return p;
    }

    int lcp(const string &s) const
    {
        // s 是查询串；返回它与任意已插入串的最长公共前缀长度。
        int p = 0, n = 0;
        for (char c : s)
        {
            int x = c - S;
            if (x < 0 || x >= A || !ch[p][x])
            {
                break;
            }
            p = ch[p][x];
            n++;
        }
        return n;
    }

    int prefixCnt(const string &s) const
    {
        // s 是查询前缀；返回以它开头的已插入字符串数。
        int p = find(s);
        return p < 0 ? 0 : cnt[p];
    }

    int stringCnt(const string &s) const
    {
        // s 是完整查询串；返回它被插入的次数。
        int p = find(s);
        return p < 0 ? 0 : end[p];
    }
};
