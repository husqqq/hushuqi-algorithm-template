// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 5 广义后缀自动机

#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变小则返回 true。
    if (b >= a)
    {
        return false;
    }
    a = b;
    return true;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变大则返回 true。
    if (a >= b)
    {
        return false;
    }
    a = b;
    return true;
}

template <int A = 26, char B = 'a'> struct ExSAM
{
    struct Node
    {
        array<int, A> ch{};
        // link 是后缀链接，len 是该状态的最长长度。
        int link = -1, len = 0;
        // cnt 是该状态所代表子串在全部输入串中的出现次数。
        long long cnt = 0;
    };

    struct TrieNode
    {
        array<int, A> ch{};
        // pass 是经过该 Trie 节点的输入串数，重复插入重复计算。
        long long pass = 0;
    };

    vector<Node> t{{}};
    vector<TrieNode> tr{{}};
    // done 记录是否已经 build，emptyCnt 是空串在全部输入串中的出现位置数。
    bool done = false;
    long long emptyCnt = 0;

    void insert(const string &s)
    {
        // s 是待加入集合的整条字符串；相邻字符串之间不产生跨边界子串。
        assert(!done); // 调试检查，可删
        emptyCnt += (int)s.size() + 1;
        int u = 0;
        for (char c : s)
        {
            int x = c - B;
            assert(0 <= x && x < A); // 调试检查，可删
            if (!tr[u].ch[x])
            {
                tr[u].ch[x] = tr.size();
                tr.push_back({});
            }
            u = tr[u].ch[x];
            tr[u].pass++;
        }
    }

    int extend(int p, int c)
    {
        // p 是当前 SAM 状态，c 是离散字符；返回扩展后的状态。
        int q = t[p].ch[c];
        if (q)
        {
            if (t[q].len == t[p].len + 1)
            {
                return q;
            }
            int nq = t.size();
            t.push_back(t[q]);
            t[nq].len = t[p].len + 1;
            t[nq].cnt = 0;
            while (p != -1 && t[p].ch[c] == q)
            {
                t[p].ch[c] = nq;
                p = t[p].link;
            }
            t[q].link = nq;
            return nq;
        }

        int u = t.size();
        t.push_back({});
        t[u].len = t[p].len + 1;
        while (p != -1 && !t[p].ch[c])
        {
            t[p].ch[c] = u;
            p = t[p].link;
        }
        if (p == -1)
        {
            t[u].link = 0;
            return u;
        }
        q = t[p].ch[c];
        if (t[q].len == t[p].len + 1)
        {
            t[u].link = q;
            return u;
        }
        int nq = t.size();
        t.push_back(t[q]);
        t[nq].len = t[p].len + 1;
        t[nq].cnt = 0;
        while (p != -1 && t[p].ch[c] == q)
        {
            t[p].ch[c] = nq;
            p = t[p].link;
        }
        t[q].link = t[u].link = nq;
        return u;
    }

    void build()
    {
        // 从公共 Trie 构造广义 SAM，并上传各状态的跨串出现次数。
        assert(!done); // 调试检查，可删
        done = true;
        vector<int> pos(tr.size());
        queue<int> q;
        q.push(0);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (int c = 0; c < A; c++)
            {
                int v = tr[u].ch[c];
                if (!v)
                {
                    continue;
                }
                pos[v] = extend(pos[u], c);
                t[pos[v]].cnt += tr[v].pass;
                q.push(v);
            }
        }
        int mx = 0;
        for (const Node &v : t)
        {
            mx = max(mx, v.len);
        }
        vector<int> c(mx + 1), p(t.size());
        for (const Node &v : t)
        {
            c[v.len]++;
        }
        partial_sum(c.begin(), c.end(), c.begin());
        for (int u = (int)t.size() - 1; u >= 0; u--)
        {
            p[--c[t[u].len]] = u;
        }
        for (int i = (int)p.size() - 1; i > 0; i--)
        {
            int u = p[i];
            if (t[u].link >= 0)
            {
                t[t[u].link].cnt += t[u].cnt;
            }
        }
    }

    int find(const string &s) const
    {
        // s 是查询串；返回对应状态，不存在或含域外字符时返回 -1。
        int u = 0;
        for (char c : s)
        {
            int x = c - B;
            if (x < 0 || x >= A)
            {
                return -1;
            }
            u = t[u].ch[x];
            if (!u)
            {
                return -1;
            }
        }
        return u;
    }

    long long count(const string &s) const
    {
        // s 是查询串；返回它在全部输入串中的出现次数，空串按各串分别计位置。
        assert(done); // 调试检查，可删
        if (s.empty())
        {
            return emptyCnt;
        }
        int u = find(s);
        return u < 0 ? 0 : t[u].cnt;
    }

    long long distinct() const
    {
        // 返回全部输入串并集中的本质不同非空子串数。
        assert(done); // 调试检查，可删
        long long ans = 0;
        for (int i = 1; i < t.size(); i++)
        {
            ans += t[i].len - t[t[i].link].len;
        }
        return ans;
    }
};
