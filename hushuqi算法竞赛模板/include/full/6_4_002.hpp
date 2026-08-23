#pragma once

#include "6_4_001.hpp"

struct DynCompOp
{
    // type 为 0 加边、1 删边、2 给点加值、3 查询连通块和；u、v 是端点或点，x 是加值。
    int type, u = 0, v = 0;
    long long x = 0;
};

vector<long long> offlineCSum(const vector<long long> &a, const vector<DynCompOp> &op)
{
    // a 是初始点权，op 是合法动态操作；返回全部 type=3 查询结果，顺序与操作出现顺序相同。
    int n = a.size(), q = op.size();
    struct Event
    {
        // type 为 0 合并边、1 连通块加值；u、v 是端点或点，x 是加值。
        int type, u, v;
        long long x;
    };
    struct Change
    {
        // type 为 0 空合并、1 合并回滚、2 加值回滚；其余字段保存旧父亲和旧连通块和。
        int type, u = 0, v = 0, pu = 0, pv = 0;
        long long su = 0, sv = 0;
    };
    vector<vector<Event>> seg(max(1LL, 4 * q));
    auto add = [&](auto &&self, int p, int l, int r, int ql, int qr, Event e) -> void
    {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr)
        {
            seg[p].push_back(e);
            return;
        }
        int m = (l + r) / 2;
        self(self, p * 2, l, m, ql, qr, e);
        self(self, p * 2 + 1, m, r, ql, qr, e);
    };
    map<pair<int, int>, int> born;
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
            add(add, 1, 0, q, i, q, {1, u, 0, x});
        }
    }
    for (auto [e, l] : born) add(add, 1, 0, q, l, q, {0, e.first, e.second, 0});
    vector<int> fa(n, -1);
    vector<long long> sum = a, ans;
    vector<Change> his;
    auto root = [&](int u)
    {
        while (fa[u] >= 0) u = fa[u];
        return u;
    };
    auto unite = [&](int u, int v)
    {
        u = root(u), v = root(v);
        if (u == v)
        {
            his.push_back({0});
            return;
        }
        if (fa[u] > fa[v]) swap(u, v);
        his.push_back({1, u, v, fa[u], fa[v], sum[u], sum[v]});
        fa[u] += fa[v];
        fa[v] = u;
        sum[u] += sum[v];
    };
    auto increase = [&](int u, long long x)
    {
        u = root(u);
        his.push_back({2, u, 0, 0, 0, sum[u], 0});
        sum[u] += x;
    };
    auto rollback = [&](int at)
    {
        while ((int)his.size() > at)
        {
            auto c = his.back();
            his.pop_back();
            if (c.type == 1)
            {
                fa[c.u] = c.pu;
                fa[c.v] = c.pv;
                sum[c.u] = c.su;
                sum[c.v] = c.sv;
            }
            else if (c.type == 2) sum[c.u] = c.su;
        }
    };
    auto solve = [&](auto &&self, int p, int l, int r) -> void
    {
        int at = his.size();
        for (auto e : seg[p])
        {
            if (e.type == 0) unite(e.u, e.v);
            else increase(e.u, e.x);
        }
        if (r - l == 1)
        {
            if (op[l].type == 3) ans.push_back(sum[root(op[l].u)]);
        }
        else
        {
            int m = (l + r) / 2;
            self(self, p * 2, l, m);
            self(self, p * 2 + 1, m, r);
        }
        rollback(at);
    };
    if (q) solve(solve, 1, 0, q);
    return ans;
}
