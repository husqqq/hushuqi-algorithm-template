#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class Cap = long long, class Cost = long long>
struct FastCostFlow
{
    struct Raw
    {
        // from、to 是端点；性能版固定为 32 位，打印版使用普通 int。
        int32_t from, to;
        Cap cap;   // 原边容量。
        Cost cost; // 原边单位费用。
    };
    struct E
    {
        // to 是终点，rev 是反向边全局下标；性能版固定为 32 位，打印版使用普通 int。
        int32_t to, rev;
        Cap cap;   // 当前残量容量。
        Cost cost; // 单位费用。
    };
    struct Q
    {
        Cost d;    // 当前约化距离。
        int32_t u; // u 是终点；性能版固定为 32 位以缩小堆元素。
        bool operator<(const Q &o) const { return d > o.d; }
    };

    // n 与端点在性能版固定为 32 位；容量、费用及答案仍由模板参数控制。
    int32_t n, source = -1, sink = -1;
    vector<Raw> raw;       // 首次求流前暂存的原边。
    vector<int32_t> start; // CSR 中点 u 的边区间为 [start[u],start[u+1])。
    vector<E> e;           // 按起点连续排列的残量边。
    vector<Cost> h, d;     // h 是势能，d 是本轮约化距离。
    vector<int32_t> pre;   // pre[v] 是最短路进入 v 的残量边下标。
    vector<int32_t> zero;  // 尚未确定的零约化距离点。
    vector<Q> heap;        // 正约化距离候选的最小堆。
    vector<char> vis, in;  // vis 是已确定/已到达标记，in 是 SPFA 入队标记。
    bool built = false;    // 是否已经构造 CSR 残量图。
    bool neg = false;      // 是否存在正容量负费用原边。

    FastCostFlow(int32_t n = 0) : n(n)
    {
        // n 是点数；构造一张空网络。
        assert(n >= 0); // 调试检查，可删。
        static_assert(numeric_limits<Cap>::is_signed);
        static_assert(numeric_limits<Cost>::is_signed);
    }

    void add(int32_t u, int32_t v, Cap cap, Cost cost)
    {
        // u、v 是端点，cap 是非负容量，cost 是单位费用；加入一条原边。
        assert(!built); // 调试检查，可删：首次 flow 后不能继续加边。
        assert(0 <= u && u < n && 0 <= v && v < n && cap >= 0); // 调试检查，可删。
        assert(cost != numeric_limits<Cost>::min()); // 调试检查，可删。
        raw.push_back({u, v, cap, cost});
        neg |= cap > 0 && cost < 0;
    }

    void build()
    {
        // 无参数；把原边一次性展开成按起点连续存储的 CSR 残量图。
        assert(raw.size() <= (size_t)numeric_limits<int32_t>::max() / 2); // 调试检查，可删。
        start.assign(n + 1, 0);
        for (const auto &a : raw)
        {
            start[a.from + 1]++;
            start[a.to + 1]++;
        }
        for (int32_t u = 0; u < n; u++)
        {
            start[u + 1] += start[u];
        }
        vector<int32_t> ptr = start;
        e.resize(raw.size() * 2);
        for (const auto &a : raw)
        {
            int32_t x = ptr[a.from]++;
            int32_t y = ptr[a.to]++;
            e[x] = {a.to, y, a.cap, a.cost};
            e[y] = {a.from, x, 0, -a.cost};
        }
        vector<Raw>().swap(raw);
        h.assign(n, 0);
        d.resize(n);
        pre.resize(n);
        vis.resize(n);
        in.resize(n);
        zero.reserve(n);
        heap.reserve(e.size());
        built = true;
    }

    void init(int32_t s)
    {
        // s 是源点；有负费用边时用一次 SPFA 建立可行势能，否则保留零势能。
        fill(h.begin(), h.end(), 0);
        if (!neg)
        {
            return;
        }
        fill(vis.begin(), vis.end(), 0);
        fill(in.begin(), in.end(), 0);
        queue<int32_t> q;
        d[s] = 0;
        vis[s] = in[s] = 1;
        q.push(s);
        while (!q.empty())
        {
            int32_t u = q.front();
            q.pop();
            in[u] = 0;
            for (int32_t i = start[u]; i < start[u + 1]; i++)
            {
                const E &a = e[i];
                if (a.cap == 0)
                {
                    continue;
                }
                Cost nd = d[u] + a.cost;
                if (!vis[a.to] || nd < d[a.to])
                {
                    d[a.to] = nd;
                    vis[a.to] = 1;
                    if (!in[a.to])
                    {
                        in[a.to] = 1;
                        q.push(a.to);
                    }
                }
            }
        }
        for (int32_t u = 0; u < n; u++)
        {
            if (vis[u])
            {
                h[u] = d[u];
            }
        }
    }

    bool dualRef(int32_t s, int32_t t)
    {
        // s、t 是固定源汇；求一条最短增广路并更新势能，返回 t 是否可达。
        const Cost inf = numeric_limits<Cost>::max();
        fill(d.begin(), d.end(), inf);
        fill(vis.begin(), vis.end(), 0);
        zero.clear();
        heap.clear();
        d[s] = 0;
        zero.push_back(s);
        size_t heapSize = 0;
        while (!zero.empty() || !heap.empty())
        {
            int32_t u;
            if (!zero.empty())
            {
                u = zero.back();
                zero.pop_back();
            }
            else
            {
                while (heapSize < heap.size())
                {
                    heapSize++;
                    push_heap(heap.begin(), heap.begin() + heapSize);
                }
                u = heap.front().u;
                pop_heap(heap.begin(), heap.end());
                heap.pop_back();
                heapSize--;
            }
            if (vis[u])
            {
                continue;
            }
            vis[u] = 1;
            if (u == t)
            {
                break;
            }
            Cost du = d[u];
            for (int32_t i = start[u]; i < start[u + 1]; i++)
            {
                const E &a = e[i];
                if (a.cap == 0)
                {
                    continue;
                }
                Cost w = a.cost + h[u] - h[a.to];
                assert(w >= 0); // 调试检查，可删：势能必须使可达残量边约化费用非负。
                Cost nd = du + w;
                if (nd < d[a.to])
                {
                    d[a.to] = nd;
                    pre[a.to] = i;
                    if (nd == du)
                    {
                        zero.push_back(a.to);
                    }
                    else
                    {
                        heap.push_back({nd, a.to});
                    }
                }
            }
        }
        if (!vis[t])
        {
            return false;
        }
        for (int32_t u = 0; u < n; u++)
        {
            if (vis[u])
            {
                h[u] -= d[t] - d[u];
            }
        }
        return true;
    }

    pair<Cap, Cost> flow(int32_t s, int32_t t,
                         Cap lim = numeric_limits<Cap>::max())
    {
        // s、t 是固定源汇，lim 是本次最多发送的流量；返回本次新增流量与最小费用。
        assert(0 <= s && s < n && 0 <= t && t < n && lim >= 0); // 调试检查，可删。
        if (s == t)
        {
            return {0, 0};
        }
        if (source == -1)
        {
            source = s;
            sink = t;
            build();
            init(s);
        }
        assert(source == s && sink == t); // 调试检查，可删：分段调用不能更换源汇。
        Cap f = 0;
        Cost cost = 0;
        while (f < lim && dualRef(s, t))
        {
            Cap x = lim - f;
            for (int32_t v = t; v != s; v = e[e[pre[v]].rev].to)
            {
                x = min(x, e[pre[v]].cap);
            }
            for (int32_t v = t; v != s; v = e[e[pre[v]].rev].to)
            {
                E &a = e[pre[v]];
                a.cap -= x;
                e[a.rev].cap += x;
            }
            f += x;
            cost += (Cost)x * (h[t] - h[s]);
        }
        return {f, cost};
    }
};
