#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

int multiSubstr(const vector<string> &a)
{
    // a 是全部输入串；返回它们的最长公共子串长度。
    if (a.empty())
    {
        return 0;
    }
    struct N
    {
        // len 是该状态的最长长度，link 是后缀链接。
        int len = 0, link = -1;
        // to 按字节保存转移目标状态。
        unordered_map<unsigned char, int> to;
    };
    vector<N> t(1);
    int last = 0;
    for (unsigned char c : a[0])
    {
        int u = t.size();
        t.push_back({t[last].len + 1});
        int p = last;
        while (p != -1 && !t[p].to.contains(c))
        {
            t[p].to[c] = u;
            p = t[p].link;
        }
        if (p == -1)
        {
            t[u].link = 0;
        }
        else
        {
            int q = t[p].to[c];
            if (t[p].len + 1 == t[q].len)
            {
                t[u].link = q;
            }
            else
            {
                int v = t.size();
                t.push_back(t[q]);
                t[v].len = t[p].len + 1;
                while (p != -1)
                {
                    auto it = t[p].to.find(c);
                    if (it == t[p].to.end() || it->second != q)
                    {
                        break;
                    }
                    it->second = v;
                    p = t[p].link;
                }
                t[q].link = t[u].link = v;
            }
        }
        last = u;
    }
    vector<int> ord(t.size()), cnt(a[0].size() + 1), mn(t.size());
    for (const N &v : t)
    {
        cnt[v.len]++;
    }
    partial_sum(cnt.begin(), cnt.end(), cnt.begin());
    for (int u = (int)t.size() - 1; u >= 0; u--)
    {
        ord[--cnt[t[u].len]] = u;
    }
    reverse(ord.begin(), ord.end());
    for (int i = 0; i < (int)t.size(); i++)
    {
        mn[i] = t[i].len;
    }
    for (int k = 1; k < (int)a.size(); k++)
    {
        vector<int> best(t.size());
        int v = 0, l = 0;
        for (unsigned char c : a[k])
        {
            while (v && !t[v].to.contains(c))
            {
                v = t[v].link;
                l = min(l, t[v].len);
            }
            if (t[v].to.contains(c))
            {
                v = t[v].to[c];
                l++;
            }
            else
            {
                l = 0;
            }
            best[v] = max(best[v], l);
        }
        for (int u : ord)
        {
            if (t[u].link >= 0)
            {
                best[t[u].link] = max(best[t[u].link], min(best[u], t[t[u].link].len));
            }
        }
        for (int i = 0; i < (int)t.size(); i++)
        {
            mn[i] = min(mn[i], best[i]);
        }
    }
    return *max_element(mn.begin(), mn.end());
}

optional<int> multiSubseq(const vector<string> &a)
{
    // a 是全部输入串；返回多串 LCS 长度，状态数无法用 size_t 表示时返回空。
    if (a.empty())
    {
        return 0;
    }
    int k = a.size();
    size_t states = 1;
    vector<size_t> st(k), base(k);
    for (int i = 0; i < k; i++)
    {
        size_t len = a[i].size() + 1;
        if (len > numeric_limits<size_t>::max() / states)
        {
            return nullopt;
        }
        base[i] = states;
        states *= len;
    }
    if (states > vector<int>().max_size())
    {
        return nullopt;
    }
    vector<int> f(states);
    for (size_t s = 1; s < states; s++)
    {
        size_t q = s;
        bool zero = false, same = true;
        int c = -1;
        size_t back = 0;
        for (int i = k - 1; i >= 0; i--)
        {
            st[i] = q / base[i];
            q %= base[i];
            if (!st[i])
            {
                zero = true;
            }
            else
            {
                int cur = (unsigned char)a[i][st[i] - 1];
                if (c == -1)
                {
                    c = cur;
                }
                else if (c != cur)
                {
                    same = false;
                }
            }
            back += base[i];
        }
        if (!zero && same)
        {
            f[s] = f[s - back] + 1;
        }
        else
        {
            for (int i = 0; i < k; i++)
            {
                if (st[i])
                {
                    f[s] = max(f[s], f[s - base[i]]);
                }
            }
        }
    }
    return f.back();
}
