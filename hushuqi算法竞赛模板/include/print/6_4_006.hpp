#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<int> offlineCSum(const vector<int> &a, const vector<array<int, 4>> &op)
{
    // a 是初始点权；op[i] 的四维依次是类型、u、v、x：类型 0 加边 u-v，1 删边 u-v，
    // 2 给点 u 加 x，3 查询 u 所在连通块的点权和。加边时该边不存在，删边时该边存在。
    // 返回全部类型 3 查询的结果，顺序与操作出现顺序相同。
    int n = a.size(), q = op.size();
    // seg[p] 是时间线段树结点 p 上的事件，每个事件的四维依次是类型（0 合并、1 加值）、u、v、x。
    vector<vector<array<int, 4>>> seg(max(1LL, 4 * q));
    auto add = [&](auto &&self, int p, int l, int r, int ql, int qr, array<int, 4> e) -> void
    {
        // 把事件 e 挂到覆盖时间区间 [ql,qr) 的结点上；p 管辖 [l,r)。
        if (qr <= l || r <= ql)
        {
            return;
        }
        if (ql <= l && r <= qr)
        {
            seg[p].push_back(e);
            return;
        }
        int m = (l + r) / 2;
        self(self, p * 2, l, m, ql, qr, e);
        self(self, p * 2 + 1, m, r, ql, qr, e);
    };
    map<pair<int, int>, int> born; // born[{u,v}] 是当前存在的边的加入时刻。
    for (int i = 0; i < q; i++)
    {
        auto [type, u, v, x] = op[i];
        if (type == 0)
        {
            born[minmax(u, v)] = i;
        }
        else if (type == 1)
        {
            auto e = minmax(u, v);
            add(add, 1, 0, q, born[e], i, {0, e.first, e.second, 0});
            born.erase(e);
        }
        else if (type == 2)
        {
            // 点加值从此刻起一直有效，相当于在 [i,q) 内给所在连通块加 x。
            add(add, 1, 0, q, i, q, {1, u, 0, x});
        }
    }
    for (auto [e, l] : born)
    {
        add(add, 1, 0, q, l, q, {0, e.first, e.second, 0});
    }
    // fa 是不压缩路径的按大小合并并查集，根的 fa 为 -大小；sum[r] 是以 r 为根的连通块点权和。
    vector<int> fa(n, -1), sum = a, ans;
    // his 是修改记录，三维依次是被改的点、旧 fa、旧 sum。
    vector<array<int, 3>> his;
    auto root = [&](int u)
    {
        // 返回 u 所在连通块的根。
        while (fa[u] >= 0)
        {
            u = fa[u];
        }
        return u;
    };
    auto save = [&](int u)
    {
        // 记录点 u 修改前的状态。
        his.push_back({u, fa[u], sum[u]});
    };
    auto rollback = [&](int at)
    {
        // 撤销到 his 只有 at 条记录时的状态。
        while ((int)his.size() > at)
        {
            auto [u, f, s] = his.back();
            his.pop_back();
            fa[u] = f;
            sum[u] = s;
        }
    };
    auto solve = [&](auto &&self, int p, int l, int r) -> void
    {
        // 进入结点 p（管辖时刻 [l,r)）时应用其事件，到叶子回答查询，退出时回滚。
        int at = his.size();
        for (auto [type, u, v, x] : seg[p])
        {
            u = root(u);
            if (type == 1)
            {
                save(u);
                sum[u] += x;
                continue;
            }
            v = root(v);
            if (u == v)
            {
                continue;
            }
            if (fa[u] > fa[v])
            {
                swap(u, v);
            }
            save(u);
            save(v);
            fa[u] += fa[v];
            fa[v] = u;
            sum[u] += sum[v];
        }
        if (r - l == 1)
        {
            if (op[l][0] == 3)
            {
                ans.push_back(sum[root(op[l][1])]);
            }
        }
        else
        {
            int m = (l + r) / 2;
            self(self, p * 2, l, m);
            self(self, p * 2 + 1, m, r);
        }
        rollback(at);
    };
    if (q)
    {
        solve(solve, 1, 0, q);
    }
    return ans;
}
