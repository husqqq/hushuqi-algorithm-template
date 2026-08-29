#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <int A = 26, char S = 'a'> struct SAM
{
    struct Node
    {
        array<int, A> next{};
        // link 是后缀链接，len 是最长长度，cnt 是当前出现次数，first 是一个最长串的末位置。
        int link = -1, len = 0, cnt = 0, first = -1;
    };

    vector<Node> t{{}};
    // last 是当前整串对应的末状态。
    int last = 0;
    // counted 表示出现次数是否已经沿后缀链接上传。
    bool counted = false;

    SAM() = default;
    SAM(const string &s)
    {
        // s 是原字符串；按顺序插入其全部字符。
        for (char c : s)
        {
            add(c);
        }
    }

    void add(char c)
    {
        // c 是要追加到原串末尾的字符。
        assert(!counted); // 调试检查，可删
        int x = c - S;
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
    }

    int find(const string &s) const
    {
        // s 是查询串；返回对应状态，不存在或含域外字符时返回 -1。
        int p = 0;
        for (char c : s)
        {
            int x = c - S;
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
        // 无参数；返回当前自动机表示的不同子串数量。
        long long ans = 0;
        for (int i = 1; i < (int)t.size(); i++)
        {
            ans += t[i].len - t[t[i].link].len;
        }
        return ans;
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
            chmax(n, v.len);
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

array<int, 4> commonSubstr(const string &a, const string &b)
{
    // a、b 是小写字符串；返回 {a_l,a_r,b_l,b_r} 的一组最长公共子串半开区间。
    SAM<26, 'a'> sam(a);
    int p = 0, len = 0, best = 0;
    array<int, 4> ans{};
    for (int i = 0; i < (int)b.size(); i++)
    {
        int x = b[i] - 'a';
        while (p && !sam.t[p].next[x])
        {
            p = sam.t[p].link;
            len = min(len, sam.t[p].len);
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
            ans = {end - len + 1, end + 1, i - len + 1, i + 1};
        }
    }
    return ans;
}
