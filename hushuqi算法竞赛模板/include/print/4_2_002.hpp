#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <int A = 26> class AC
{
  public:
    struct Node
    {
        // ch 是 Trie 实边，nxt 是补全失配后的自动机转移。
        array<int, A> ch, nxt;
        // fail 指向当前状态的最长真后缀状态。
        int fail = 0;
        Node()
        {
            ch.fill(-1);
            nxt.fill(0);
        }
    };

  private:
    // t 是状态表，pos 按插入顺序保存各模式终态。
    vector<Node> t{1};
    vector<int> pos;

  public:
    int add(const string &s, char fst = 'a')
    {
        // s 是非空模式串，fst 是字符集起始字节；返回模式终态。
        assert(!s.empty()); // 调试检查，可删
        int u = 0;
        for (char c : s)
        {
            int x = (unsigned char)c - (unsigned char)fst;
            assert(0 <= x && x < A); // 调试检查，可删
            if (t[u].ch[x] == -1)
            {
                t[u].ch[x] = t.size();
                t.push_back({});
            }
            u = t[u].ch[x];
        }
        pos.push_back(u);
        return u;
    }

    void build()
    {
        // 补全全部失配链接和自动机转移。
        queue<int> q;
        for (int c = 0; c < A; c++)
        {
            int v = t[0].ch[c];
            if (v == -1)
            {
                continue;
            }
            t[0].nxt[c] = v;
            q.push(v);
        }
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            t[u].nxt = t[t[u].fail].nxt;
            for (int c = 0; c < A; c++)
            {
                int v = t[u].ch[c];
                if (v == -1)
                {
                    continue;
                }
                t[v].fail = t[u].nxt[c];
                t[u].nxt[c] = v;
                q.push(v);
            }
        }
    }

    int step(int u, int c) const
    {
        // u 是当前状态，c 是 [0,A) 内字符编号；返回下一状态。
        assert(0 <= u && u < (int)t.size()); // 调试检查，可删
        assert(0 <= c && c < A); // 调试检查，可删
        return t[u].nxt[c];
    }
    const vector<Node> &nodes() const
    {
        // 返回按编号排列的状态表。
        return t;
    }
    const vector<int> &ends() const
    {
        // 返回各模式按插入顺序对应的终态。
        return pos;
    }
};
