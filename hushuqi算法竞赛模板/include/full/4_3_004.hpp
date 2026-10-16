#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

template <int A = 26, char S = 'a'> struct SAM
{
    static_assert(A > 0 && (unsigned char)S + A <= 256);
    struct Node
    {
        array<int32_t, A> next{}; // next[c] 是字符 c 的转移，0 表示不存在（根没有入边）。
        // link 是后缀链接，len 是最长长度，cnt 是当前出现次数，first 是一个最长串的末位置。
        int32_t link = -1, len = 0, cnt = 0, first = -1;
    };

    vector<Node> t{{}}; // t[0] 是根；追加前状态数加二须小于 2^31。
    // last 是当前整串对应的末状态。
    int last = 0;
    // counted 表示出现次数是否已经沿后缀链接上传。
    bool counted = false;
    long long subs = 0; // 本质不同非空子串数，可达平方级，关闭整数宏仍用 64 位。

    SAM() = default; // 构造只含根的空串自动机。
    SAM(const string &s)
    {
        // s 是原字符串；预留状态上界，按顺序插入，避免克隆引起整池搬迁。
        t.reserve(2 * s.size() + 1);
        for (char c : s)
        {
            add(c);
        }
    }

    void add(char c)
    {
        // c 是要追加到原串末尾的字符；无返回值，上传次数后不能追加。
        assert(!counted && t.size() + 2 < (1LL << 31));
        int x = (unsigned char)c - (unsigned char)S;
        assert(0 <= x && x < A); // 调试检查，可删
        int p = last, q = t.size();
        t.push_back({});
        t[q].len = t[p].len + 1;
        t[q].cnt = 1;
        t[q].first = t[q].len - 1;
        while (p != -1 && !t[p].next[x])
        {
            t[p].next[x] = q;
            p = t[p].link;
        }
        if (p == -1)
        {
            t[q].link = 0;
        }
        else
        {
            int v = t[p].next[x];
            if (t[v].len == t[p].len + 1)
            {
                t[q].link = v;
            }
            else
            {
                int z = t.size();
                t.push_back(t[v]);
                t[z].len = t[p].len + 1;
                t[z].cnt = 0;
                while (p != -1 && t[p].next[x] == v)
                {
                    t[p].next[x] = z;
                    p = t[p].link;
                }
                t[v].link = t[q].link = z;
            }
        }
        last = q;
        subs += t[q].len - t[t[q].link].len;
    }

    int find(const string &s) const
    {
        // s 是查询串；返回对应状态，不存在或含域外字符时返回 -1。
        int p = 0;
        for (char c : s)
        {
            int x = (unsigned char)c - (unsigned char)S;
            if (x < 0 || x >= A || !t[p].next[x])
            {
                return -1;
            }
            p = t[p].next[x];
        }
        return p;
    }

    long long distinct() const
    {
        // 返回当前自动机表示的本质不同非空子串数。
        return subs;
    }

    void count()
    {
        // 把各末状态的计数沿后缀链接上传；重复调用不再二次累计。
        if (counted)
        {
            return;
        }
        int n = 0;
        for (auto &v : t)
        {
            n = max<int>(n, v.len);
        }
        vector<int> c(n + 1), p(t.size());
        for (auto &v : t)
        {
            c[v.len]++;
        }
        partial_sum(c.begin(), c.end(), c.begin());
        for (int i = (int)t.size() - 1; i >= 0; i--)
        {
            p[--c[t[i].len]] = i;
        }
        for (int i = (int)p.size() - 1; i; i--)
        {
            int x = p[i];
            t[t[x].link].cnt += t[x].cnt;
        }
        counted = true;
    }

    int occ(const string &s)
    {
        // s 是查询串；返回它在原串中的出现次数，空串返回原串长度加一。
        if (s.empty())
        {
            return t[last].len + 1;
        }
        int p = find(s);
        if (p < 0)
        {
            return 0;
        }
        count();
        return t[p].cnt;
    }
};

template <int A = 26, char S = 'a'>
array<int, 4> commonSubstr(const string &a, const string &b)
{
    // a、b 的无符号字节属于 [S,S+A)；返回 {a_l,a_r,b_l,b_r} 半开区间，无解全零。
    if (a.empty() || b.empty()) return {};
    bool rev = a.size() > b.size(); // 对短串建表，返回时恢复原参数的坐标顺序。
    const string &s = rev ? b : a, &t = rev ? a : b;
    SAM<A, S> sam(s);
    int p = 0, len = 0, best = 0;
    array<int, 4> ans{};
    for (int i = 0; i < (int)t.size(); i++)
    {
        int x = (unsigned char)t[i] - (unsigned char)S;
        assert(0 <= x && x < A);
        while (p && !sam.t[p].next[x])
        {
            p = sam.t[p].link;
            len = min<int>(len, sam.t[p].len);
        }
        if (sam.t[p].next[x])
        {
            p = sam.t[p].next[x];
            len++;
        }
        else
        {
            len = 0;
        }
        if (len > best)
        {
            best = len;
            int end = sam.t[p].first;
            ans = rev ? array<int, 4>{i - len + 1, i + 1, end - len + 1, end + 1}
                      : array<int, 4>{end - len + 1, end + 1, i - len + 1, i + 1};
        }
    }
    return ans;
}
