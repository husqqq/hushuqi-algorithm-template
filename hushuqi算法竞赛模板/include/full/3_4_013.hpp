#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct FischerHeun
{
    // a 是原序列；first[i] 是 i 在欧拉序中第一次出现的位置。
    vector<int> a, first;
    // euler、dep 是笛卡尔树欧拉序上的结点编号与深度，相邻深度恰差 1。
    vector<int> euler, dep;
    // b 是块长；pat[k] 是第 k 块深度差模式对应的块内表编号。
    int b = 1;
    vector<int> pat;
    // tab[id][l*b+r] 是该模式块内 [l,r] 的最小深度位置；st 是块最小值的稀疏表。
    vector<vector<int>> tab, st;

    FischerHeun(const vector<int> &v) : a(v), first(v.size(), -1)
    {
        // v 是静态序列；建立 O(n) 预处理、O(1) 查询的区间最小值结构。
        int n = a.size();
        if (!n)
        {
            return;
        }
        // 单调栈建笛卡尔树：lc、rc 是左右儿子，栈底是根。
        // 改区间最大值：只把下面的 a[i] < a[stk.back()] 改成 a[i] > a[stk.back()]，其余深度比较不变。
        vector<int> lc(n, -1), rc(n, -1), stk;
        for (int i = 0; i < n; i++)
        {
            int last = -1;
            while (!stk.empty() && a[i] < a[stk.back()])
            {
                last = stk.back();
                stk.pop_back();
            }
            if (!stk.empty())
            {
                rc[stk.back()] = i;
            }
            lc[i] = last;
            stk.push_back(i);
        }
        auto emit = [&](int v, int d)
        {
            // v 是结点，d 是深度；把 v 追加进欧拉序。
            if (first[v] < 0)
            {
                first[v] = euler.size();
            }
            euler.push_back(v);
            dep.push_back(d);
        };
        auto dfs = [&](auto &&self, int v, int d) -> void
        {
            // v 是当前结点，d 是其深度；进入以及每个儿子返回后各记录一次 v。
            emit(v, d);
            if (lc[v] >= 0)
            {
                self(self, lc[v], d + 1);
                emit(v, d);
            }
            if (rc[v] >= 0)
            {
                self(self, rc[v], d + 1);
                emit(v, d);
            }
        };
        dfs(dfs, stk[0], 0);
        int m = dep.size();
        b = max(1LL, (int)bit_width((unsigned long long)m) / 2);
        int cnt = (m + b - 1) / b;
        pat.resize(cnt);
        vector<int> mins(cnt);
        map<int, int> id; // id[code] 是该深度差模式在 tab 中的编号。
        for (int k = 0; k < cnt; k++)
        {
            int l = k * b, r = min(m, l + b), code = 0;
            mins[k] = l;
            for (int i = l + 1; i < r; i++)
            {
                if (dep[i] > dep[i - 1])
                {
                    code |= 1LL << (i - l - 1);
                }
                if (dep[i] < dep[mins[k]])
                {
                    mins[k] = i;
                }
            }
            if (!id.count(code))
            {
                // 按模式还原相对深度 d，再暴力填满块内所有 [i,j] 的最小位置。
                vector<int> d(b), t(b * b);
                for (int i = 1; i < b; i++)
                {
                    d[i] = d[i - 1] + ((code >> (i - 1) & 1) ? 1 : -1);
                }
                for (int i = 0; i < b; i++)
                {
                    int best = i;
                    for (int j = i; j < b; j++)
                    {
                        if (d[j] < d[best])
                        {
                            best = j;
                        }
                        t[i * b + j] = best;
                    }
                }
                id[code] = tab.size();
                tab.push_back(move(t));
            }
            pat[k] = id[code];
        }
        st.push_back(move(mins));
        for (int k = 1; (1LL << k) <= cnt; k++)
        {
            st.emplace_back(cnt - (1LL << k) + 1);
            for (int i = 0; i < (int)st[k].size(); i++)
            {
                st[k][i] = pick(st[k - 1][i], st[k - 1][i + (1LL << (k - 1))]);
            }
        }
    }

    int pick(int x, int y) const
    {
        // x、y 是欧拉序位置；返回深度较小者，相等时取 x。
        return dep[x] <= dep[y] ? x : y;
    }

    int inside(int k, int l, int r) const
    {
        // k 是块号，l、r 是块内闭区间；返回其中深度最小的欧拉序位置。
        return k * b + tab[pat[k]][l * b + r];
    }

    int argmin(int l, int r) const
    {
        // [l,r) 是非空半开区间；返回其中一个最小值的下标。
        assert(0 <= l && l < r && r <= (int)a.size()); // 调试检查，可删。
        int x = first[l], y = first[r - 1];
        if (x > y)
        {
            swap(x, y);
        }
        int u = x / b, v = y / b;
        if (u == v)
        {
            return euler[inside(u, x % b, y % b)];
        }
        int ans = pick(inside(u, x % b, b - 1), inside(v, 0, y % b));
        if (v > u + 1)
        {
            int k = bit_width((unsigned long long)(v - u - 1)) - 1;
            ans = pick(ans, pick(st[k][u + 1], st[k][v - (1LL << k)]));
        }
        return euler[ans];
    }

    int query(int l, int r) const
    {
        // [l,r) 是非空半开区间；返回区间最小值。
        return a[argmin(l, r)];
    }
};
