#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "5_5_001.hpp"
#include "5_6_003.hpp"

struct EdgeColor
{
    // 颜色数。
    int k;
    // c[i] 是第 i 条原边的颜色。
    vector<int> c;
};

struct BipEdgeColor
{
    // 左、右部点数。
    int n, m;
    // 每条原边依次保存左端点、右端点、插入编号。
    vector<array<int, 3>> e;

    BipEdgeColor(int n, int m) : n(n), m(m)
    {
        // n、m 是左右部点数；构造空二分图。
        assert(n >= 0 && m >= 0); // 调试检查，可删。
    }

    void add(int u, int v)
    {
        // u、v 是左右端点；按加入顺序给边编号。
        assert(0 <= u && u < n && 0 <= v && v < m); // 调试检查，可删。
        e.push_back({u, v, (int)e.size()});
    }

    EdgeColor work() const
    {
        // 返回最少颜色数及每条原边的颜色；先合并低度点，避免孤立点使补图膨胀。
        vector<int> dl(n), dr(m);
        int k = 0;
        for (auto x : e)
        {
            k = max(k, ++dl[x[0]]);
            k = max(k, ++dr[x[1]]);
        }
        if (!k) return {0, {}};
        auto compress = [&](const vector<int> &deg) -> pair<vector<int>, int>
        {
            // deg 是一侧原点度数；返回原点到合并点的映射及合并后点数。
            int sz = (int)deg.size();
            DSU ds(sz);
            // 堆项依次为集合总度数、并查集根。
            priority_queue<array<int, 2>, vector<array<int, 2>>, greater<>> q;
            for (int i = 0; i < sz; i++)
            {
                q.push({deg[i], i});
            }
            while (q.size() > 1)
            {
                auto [x, u] = q.top(); q.pop();
                auto [y, v] = q.top(); q.pop();
                if (x > k - y)
                {
                    q.push({x, u});
                    q.push({y, v});
                    break;
                }
                ds.merge(u, v);
                q.push({x + y, ds.find(u)});
            }
            vector<int> id(sz), pos(sz, -1);
            int cnt = 0;
            for (int i = 0; i < sz; i++)
            {
                int u = ds.find(i);
                if (pos[u] == -1) pos[u] = cnt++;
                id[i] = pos[u];
            }
            return {id, cnt};
        };
        auto [pl, nl] = compress(dl);
        auto [pr, nr] = compress(dr);
        int z = max(nl, nr);
        // 补图边依次保存左端点、右端点、原边编号；虚拟边编号为 -1。
        vector<array<int, 3>> es;
        es.reserve(2 * e.size() + 2 * z);
        vector<int> cl(z), cr(z);
        for (auto x : e)
        {
            x[0] = pl[x[0]];
            x[1] = pr[x[1]];
            es.push_back(x);
            cl[x[0]]++;
            cr[x[1]]++;
        }
        int u = 0, v = 0;
        while (u < z && v < z)
        {
            while (u < z && cl[u] == k)
            {
                u++;
            }
            while (v < z && cr[v] == k)
            {
                v++;
            }
            if (u == z || v == z) break;
            int cnt = min(k - cl[u], k - cr[v]);
            for (int i = 0; i < cnt; i++)
            {
                es.push_back({u, v, -1});
            }
            cl[u] += cnt;
            cr[v] += cnt;
        }
        vector<int> ans(e.size());
        auto perfect = [&](const vector<int> &ids) -> vector<int>
        {
            // ids 是正则二分图边编号；返回一组覆盖全部合并点的完美匹配边。
            HopcroftKarp hk(z, z);
            for (int id : ids)
            {
                hk.addEdge(es[id][0], es[id][1]);
            }
            int got = hk.matching();
            assert(got == z); // 调试检查，可删：正则二分图总有完美匹配。
            auto ml = hk.leftMatch();
            vector<int> take(z, -1);
            for (int id : ids)
            {
                int u = es[id][0], v = es[id][1];
                if (ml[u] == v && take[u] == -1) take[u] = id;
            }
            return take;
        };
        // seen[id] 是该补图边最近一次被标记的轮次；共享数组避免每个递归节点全量清零。
        vector<int> seen(es.size());
        int turn = 0;
        auto color = [&](auto &&self, vector<int> ids, int d, int first) -> void
        {
            // ids 是 d 正则图边集，d 是度数，first 是该集合第一种可用颜色。
            if (!d) return;
            if (d == 1)
            {
                for (int id : ids)
                {
                    if (es[id][2] != -1) ans[es[id][2]] = first;
                }
                return;
            }
            if (d & 1)
            {
                auto take = perfect(ids);
                turn++;
                for (int id : take)
                {
                    seen[id] = turn;
                    if (es[id][2] != -1) ans[es[id][2]] = first;
                }
                vector<int> rest;
                rest.reserve(ids.size() - take.size());
                for (int id : ids)
                {
                    if (seen[id] != turn) rest.push_back(id);
                }
                self(self, move(rest), d - 1, first + 1);
                return;
            }
            // g[x] 的元素依次为相邻点、补图边编号。
            vector<vector<array<int, 2>>> g(2 * z);
            for (int id : ids)
            {
                int a = es[id][0], b = z + es[id][1];
                g[a].push_back({b, id});
                g[b].push_back({a, id});
            }
            turn++;
            vector<int> ptr(2 * z), a, b;
            for (int s = 0; s < 2 * z; s++)
            {
                while (ptr[s] < (int)g[s].size() && seen[g[s][ptr[s]][1]] == turn)
                {
                    ptr[s]++;
                }
                if (ptr[s] == (int)g[s].size()) continue;
                // 偶度图中极大未重复边迹必回到起点；二分图中其长度为偶数。
                int x = s, parity = 0;
                while (true)
                {
                    while (ptr[x] < (int)g[x].size() && seen[g[x][ptr[x]][1]] == turn)
                    {
                        ptr[x]++;
                    }
                    if (ptr[x] == (int)g[x].size()) break;
                    auto [y, id] = g[x][ptr[x]++];
                    seen[id] = turn;
                    (parity ? b : a).push_back(id);
                    parity ^= 1;
                    x = y;
                }
                assert(x == s && parity == 0); // 调试检查，可删。
            }
            self(self, move(a), d / 2, first);
            self(self, move(b), d / 2, first + d / 2);
        };
        vector<int> ids(es.size());
        iota(ids.begin(), ids.end(), 0);
        color(color, move(ids), k, 0);
        return {k, move(ans)};
    }
};
