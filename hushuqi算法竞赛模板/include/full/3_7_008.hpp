#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct PermTree
{
    struct Node
    {
        int l = 0;        // l 保存强区间左端点。
        int r = 0;        // r 保存强区间右端点，区间为 [l,r)。
        int type = 0;     // type 为 0、1、2 时分别表示叶、析点、合点。
        int par = -1;     // par 保存父结点，根的父亲为 -1。
        int mid = -1;     // 合点中 mid 保存最右孩子的左端点。
        vector<int> ch;   // ch 按原排列顺序保存直接孩子。
    };

    struct RMQ
    {
        vector<int> lg;         // lg[len] 保存 floor(log2(len))。
        vector<vector<int>> mn; // mn[k][i] 保存长 2^k 区间最小值。
        vector<vector<int>> mx; // mx[k][i] 保存长 2^k 区间最大值。

        RMQ(const vector<int> &a)
        {
            // a 是排列；建立区间最大值和最小值稀疏表。
            int n = a.size();
            lg.assign(n + 1, 0);
            for (int i = 2; i <= n; i++)
            {
                lg[i] = lg[i / 2] + 1;
            }
            int z = n ? lg[n] + 1 : 0;
            mn.assign(z, vector<int>(n));
            mx.assign(z, vector<int>(n));
            if (n)
            {
                mn[0] = mx[0] = a;
            }
            for (int k = 1; k < z; k++)
            {
                for (int i = 0; i + (1LL << k) <= n; i++)
                {
                    mn[k][i] = min(mn[k - 1][i], mn[k - 1][i + (1LL << (k - 1))]);
                    mx[k][i] = max(mx[k - 1][i], mx[k - 1][i + (1LL << (k - 1))]);
                }
            }
        }

        pair<int, int> query(int l, int r) const
        {
            // l、r 是非空半开区间端点；返回该区间的 {最小值,最大值}。
            int k = lg[r - l];
            int d = r - (1LL << k);
            return {min(mn[k][l], mn[k][d]), max(mx[k][l], mx[k][d])};
        }
    };

    struct Seg
    {
        int n;               // n 保存叶子个数。
        vector<__int128> mn, tag; // mn、tag 保存区间最小值与区间加懒标记。

        Seg(int n) : n(n), mn(4 * n + 4), tag(4 * n + 4)
        {
            // n 是排列长度；初始 q 全为 0。
        }

        void apply(int u, __int128 v)
        {
            // u 是线段树结点，v 是增量；给整段加 v。
            mn[u] += v;
            tag[u] += v;
        }

        void push(int u)
        {
            // u 是线段树结点；把区间加标记下传给两个儿子。
            if (tag[u])
            {
                apply(u * 2, tag[u]);
                apply(u * 2 + 1, tag[u]);
                tag[u] = 0;
            }
        }

        void add(int u, int l, int r, int ql, int qr, __int128 v)
        {
            // u 管理 [l,r)，给 [ql,qr) 加 v。
            if (ql <= l && r <= qr)
            {
                apply(u, v);
                return;
            }
            push(u);
            int m = (l + r) / 2;
            if (ql < m)
            {
                add(u * 2, l, m, ql, qr, v);
            }
            if (m < qr)
            {
                add(u * 2 + 1, m, r, ql, qr, v);
            }
            mn[u] = min(mn[u * 2], mn[u * 2 + 1]);
        }

        void add(int l, int r, __int128 v)
        {
            // l、r 是半开区间端点；给 q[l,r) 加 v。
            if (l < r)
            {
                add(1, 0, n, l, r, v);
            }
        }

        int firstZero(int u, int l, int r)
        {
            // u 管理 [l,r)，且段内最小值为 0；返回最左的零位置。
            if (r - l == 1)
            {
                return l;
            }
            push(u);
            int m = (l + r) / 2;
            if (mn[u * 2] == 0)
            {
                return firstZero(u * 2, l, m);
            }
            return firstZero(u * 2 + 1, m, r);
        }

        int firstZero()
        {
            // 无参数；返回 q 中最左的零位置。
            assert(mn[1] == 0);
            return firstZero(1, 0, n);
        }
    };

    vector<Node> t; // t 保存全部析合树结点。
    int root = -1;  // root 保存整段排列对应的根结点。

    PermTree(const vector<int> &a)
    {
        // a 是一段连续整数的排列；用增量析合森林构造析合树。
        int n = a.size();
        if (!n)
        {
            return;
        }
        vector<int> chk = a;
        sort(chk.begin(), chk.end());
        assert(unique(chk.begin(), chk.end()) == chk.end());
        assert((__int128)chk.back() - (__int128)chk[0] == n - 1);
        RMQ rmq(a);
        Seg seg(n);
        vector<int> low, high, forest;
        auto common = [&](int l, int r)
        {
            auto [mn, mx] = rmq.query(l, r);
            return (__int128)mx - (__int128)mn == r - l - 1;
        };
        auto node = [&](int l, int r, int type)
        {
            t.push_back({l, r, type, -1, -1, {}});
            return (int)t.size() - 1;
        };
        auto link = [&](int u, int v)
        {
            t[u].ch.push_back(v);
            t[v].par = u;
        };
        for (int i = 0; i < n; i++)
        {
            int lp = low.empty() ? 0 : low.back() + 1;
            while (!low.empty() && a[i] <= a[low.back()])
            {
                int p = low.back();
                low.pop_back();
                int l = low.empty() ? 0 : low.back() + 1;
                seg.add(l, p + 1, (__int128)a[p]);
                lp = l;
            }
            seg.add(lp, i + 1, -(__int128)a[i]);
            low.push_back(i);
            lp = high.empty() ? 0 : high.back() + 1;
            while (!high.empty() && a[i] >= a[high.back()])
            {
                int p = high.back();
                high.pop_back();
                int l = high.empty() ? 0 : high.back() + 1;
                seg.add(l, p + 1, -(__int128)a[p]);
                lp = l;
            }
            seg.add(lp, i + 1, (__int128)a[i]);
            high.push_back(i);
            int now = node(i, i + 1, 0);
            int le = seg.firstZero();
            while (!forest.empty() && t[forest.back()].l >= le)
            {
                int x = forest.back();
                if (t[x].type == 2 && common(t[x].mid, i + 1))
                {
                    forest.pop_back();
                    t[x].r = i + 1;
                    t[x].mid = t[now].l;
                    link(x, now);
                    now = x;
                }
                else if (common(t[x].l, i + 1))
                {
                    forest.pop_back();
                    int u = node(t[x].l, i + 1, 2);
                    t[u].mid = t[now].l;
                    link(u, x);
                    link(u, now);
                    now = u;
                }
                else
                {
                    int u = node(0, i + 1, 1);
                    link(u, now);
                    while (true)
                    {
                        x = forest.back();
                        forest.pop_back();
                        link(u, x);
                        if (common(t[x].l, i + 1))
                        {
                            break;
                        }
                    }
                    t[u].l = t[x].l;
                    now = u;
                }
            }
            forest.push_back(now);
            seg.add(0, i + 1, -1);
        }
        assert(forest.size() == 1);
        root = forest[0];
        for (auto &x : t)
        {
            sort(x.ch.begin(), x.ch.end(), [&](int u, int v)
            {
                return t[u].l < t[v].l;
            });
        }
    }
};
