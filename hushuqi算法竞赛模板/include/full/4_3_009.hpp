#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct SuffixBST
{
    struct Node
    {
        int l = 0, r = 0, sz = 1, key = 0;
        unsigned long long pri = 0;
    };

    static constexpr unsigned long long base = 11995408973635179863ULL;
    string r;
    vector<unsigned long long> h{0}, pw{1};
    vector<Node> t{{}};
    vector<long long> ways{0};
    int root = 0;

    static unsigned long long priority(unsigned long long x)
    {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    int size(int u) const
    {
        return u ? t[u].sz : 0;
    }

    void pull(int u)
    {
        t[u].sz = size(t[u].l) + size(t[u].r) + 1;
    }

    unsigned long long hash(int l, int rr) const
    {
        return h[rr] - h[l] * pw[rr - l];
    }

    int lcpKey(int a, int b) const
    {
        int l = 0, rr = min(a, b) + 1;
        while (l + 1 < rr)
        {
            int m = (l + rr) / 2;
            if (hash(a - m, a) == hash(b - m, b))
            {
                l = m;
            }
            else
            {
                rr = m;
            }
        }
        return l;
    }

    bool lessKey(int a, int b) const
    {
        if (a == b)
        {
            return false;
        }
        int p = lcpKey(a, b);
        if (p == min(a, b))
        {
            return a < b;
        }
        return (unsigned char)r[a - p - 1] < (unsigned char)r[b - p - 1];
    }

    void split(int u, int key, int &x, int &y)
    {
        if (!u)
        {
            x = y = 0;
        }
        else if (lessKey(t[u].key, key))
        {
            x = u;
            split(t[u].r, key, t[u].r, y);
            pull(u);
        }
        else
        {
            y = u;
            split(t[u].l, key, x, t[u].l);
            pull(u);
        }
    }

    int merge(int x, int y)
    {
        if (!x || !y)
        {
            return x | y;
        }
        if (t[x].pri > t[y].pri)
        {
            t[x].r = merge(t[x].r, y);
            pull(x);
            return x;
        }
        t[y].l = merge(x, t[y].l);
        pull(y);
        return y;
    }

    void insert(int u)
    {
        int x, y;
        split(root, t[u].key, x, y);
        root = merge(merge(x, u), y);
    }

    int erase(int u, int key)
    {
        if (t[u].key == key)
        {
            return merge(t[u].l, t[u].r);
        }
        if (lessKey(key, t[u].key))
        {
            t[u].l = erase(t[u].l, key);
        }
        else
        {
            t[u].r = erase(t[u].r, key);
        }
        pull(u);
        return u;
    }

    pair<int, int> neighbors(int key) const
    {
        int u = root, pre = 0, suc = 0;
        while (u)
        {
            if (lessKey(t[u].key, key))
            {
                pre = t[u].key;
                u = t[u].r;
            }
            else
            {
                suc = t[u].key;
                u = t[u].l;
            }
        }
        return {pre, suc};
    }

    void pushFront(char c)
    {
        r.push_back(c);
        h.push_back(h.back() * base + (unsigned char)c + 1);
        pw.push_back(pw.back() * base);
        int key = r.size();
        auto [pre, suc] = neighbors(key);
        int same = 0;
        if (pre)
        {
            same = max(same, lcpKey(key, pre));
        }
        if (suc)
        {
            same = max(same, lcpKey(key, suc));
        }
        ways.push_back(ways.back() + key - same);
        t.push_back({0, 0, 1, key, priority(key)});
        insert(key);
    }

    void popFront()
    {
        assert(!r.empty());
        int key = r.size();
        root = erase(root, key);
        r.pop_back();
        h.pop_back();
        pw.pop_back();
        ways.pop_back();
        t.pop_back();
    }

    int rank(int i) const
    {
        int key = (int)r.size() - i;
        assert(1 <= key && key <= (int)r.size());
        int u = root, ans = 0;
        while (t[u].key != key)
        {
            if (lessKey(key, t[u].key))
            {
                u = t[u].l;
            }
            else
            {
                ans += size(t[u].l) + 1;
                u = t[u].r;
            }
        }
        return ans + size(t[u].l);
    }

    int kth(int k) const
    {
        assert(0 <= k && k < size(root));
        int u = root;
        while (true)
        {
            int z = size(t[u].l);
            if (k < z)
            {
                u = t[u].l;
            }
            else if (k == z)
            {
                return (int)r.size() - t[u].key;
            }
            else
            {
                k -= z + 1;
                u = t[u].r;
            }
        }
    }

    long long distinct() const
    {
        return ways.back();
    }
};
