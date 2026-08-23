#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T = long long>
struct HLPP
{
    struct E
    {
        // to 是终点，rev 是反向边在 e[to] 中的下标。
        int to, rev;
        // cap 是当前残量容量。
        T cap;
    };

    // n 是点数，src、sink 是最近一次求流使用的源汇点。
    int n, src = -1, sink = -1;
    // e[u] 是点 u 发出的全部残量边。
    vector<vector<E>> e;
    // ex 是超额流，h 是标号，cur 是当前弧。
    vector<T> ex;
    vector<int> h, cur;
    // actHead/actNext 按标号维护活跃点链表，in 标记是否已经入链。
    vector<int> actHead, actNext;
    vector<char> in;
    // gapHead/gapPrev/gapNext 按标号维护全部顶点链表。
    vector<int> gapHead, gapPrev, gapNext;
    int maxH = -1, work = 0;
    bool used = false; // used 标记是否已经调用过 flow。

    HLPP(int n = 0)
        : n(n), e(n), ex(n), h(n), cur(n), actHead(n, -1), actNext(n, -1), in(n),
          gapHead(n, -1), gapPrev(n, -1), gapNext(n, -1)
    {
        // n 是残量网络点数；构造空网络。
    }

    void add(int u, int v, T c)
    {
        // u、v 是端点，c 是非负容量；加入一对残量边。
        assert(0 <= u && u < n && 0 <= v && v < n && c >= 0); // 调试检查，可删。
        int id = e[u].size();
        int rev = e[v].size() + (u == v);
        e[u].push_back({v, rev, c});
        e[v].push_back({u, id, 0});
    }

    void insertGap(int u)
    {
        // 把标号小于 n 的点 u 插入对应 GAP 链表。
        int d = h[u];
        gapPrev[u] = -1;
        gapNext[u] = gapHead[d];
        if (gapHead[d] != -1)
        {
            gapPrev[gapHead[d]] = u;
        }
        gapHead[d] = u;
    }

    void removeGap(int u)
    {
        // 从当前标号链表移除点 u。
        int d = h[u];
        if (gapPrev[u] == -1)
        {
            gapHead[d] = gapNext[u];
        }
        else
        {
            gapNext[gapPrev[u]] = gapNext[u];
        }
        if (gapNext[u] != -1)
        {
            gapPrev[gapNext[u]] = gapPrev[u];
        }
        gapPrev[u] = gapNext[u] = -1;
    }

    void enqueue(int u)
    {
        // 把有超额且可达汇点的非源汇点加入最高标号活跃链表。
        if (u == src || u == sink || ex[u] <= 0 || h[u] >= n || in[u])
        {
            return;
        }
        in[u] = true;
        actNext[u] = actHead[h[u]];
        actHead[h[u]] = u;
        maxH = max(maxH, h[u]);
    }

    void relabelAll()
    {
        // 从汇点按反向残量边 BFS，重建标号、GAP 链表与活跃点链表。
        fill(h.begin(), h.end(), n);
        fill(cur.begin(), cur.end(), 0);
        fill(actHead.begin(), actHead.end(), -1);
        fill(actNext.begin(), actNext.end(), -1);
        fill(in.begin(), in.end(), false);
        fill(gapHead.begin(), gapHead.end(), -1);
        fill(gapPrev.begin(), gapPrev.end(), -1);
        fill(gapNext.begin(), gapNext.end(), -1);
        maxH = -1;
        queue<int> q;
        h[sink] = 0;
        q.push(sink);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (const auto &a : e[u])
            {
                int v = a.to;
                if (v != src && h[v] == n && e[v][a.rev].cap > 0)
                {
                    h[v] = h[u] + 1;
                    q.push(v);
                }
            }
        }
        for (int u = 0; u < n; u++)
        {
            if (h[u] < n)
            {
                insertGap(u);
            }
        }
        for (int u = 0; u < n; u++)
        {
            enqueue(u);
        }
        work = 0;
    }

    void gapRelabel(int old)
    {
        // old 层为空时，所有更高标号点都不可达汇点，统一标为 n。
        for (int d = old + 1; d < n; d++)
        {
            int u = gapHead[d];
            gapHead[d] = -1;
            while (u != -1)
            {
                int v = gapNext[u];
                h[u] = n;
                gapPrev[u] = gapNext[u] = -1;
                if (in[u])
                {
                    in[u] = false;
                    actNext[u] = -1;
                }
                u = v;
            }
            actHead[d] = -1;
        }
        maxH = min(maxH, old - 1);
    }

    void relabel(int u)
    {
        // 当前弧耗尽时把 u 改到残量邻点的最低标号加一。
        int old = h[u], nh = n;
        for (const auto &a : e[u])
        {
            if (a.cap > 0)
            {
                nh = min(nh, h[a.to] + 1);
            }
        }
        removeGap(u);
        bool gap = gapHead[old] == -1;
        if (gap)
        {
            gapRelabel(old);
        }
        h[u] = gap ? n : nh;
        cur[u] = 0;
        if (h[u] < n)
        {
            insertGap(u);
        }
        work++;
    }

    void push(int u, E &a)
    {
        // 沿允许边从 u 推出尽量多的超额流。
        T f = min(ex[u], a.cap);
        a.cap -= f;
        e[a.to][a.rev].cap += f;
        ex[u] -= f;
        ex[a.to] += f;
        enqueue(a.to);
    }

    void discharge(int u)
    {
        // 反复推流或重标号，直到 u 没有超额或已不可达汇点。
        while (ex[u] > 0 && h[u] < n)
        {
            if (cur[u] == (int)e[u].size())
            {
                relabel(u);
                continue;
            }
            E &a = e[u][cur[u]];
            if (a.cap == 0 || h[u] != h[a.to] + 1)
            {
                cur[u]++;
                continue;
            }
            push(u, a);
        }
    }

    T flow(int s, int t)
    {
        // s、t 是源汇点；首次调用时返回最大流。
        assert(0 <= s && s < n && 0 <= t && t < n); // 调试检查，可删。
        assert(!used); // 调试检查，可删。
        used = true;
        if (s == t)
        {
            return 0;
        }
        src = s;
        sink = t;
        ex.assign(n, 0);
        relabelAll();
        for (auto &a : e[s])
        {
            if (a.to == s || a.cap == 0)
            {
                continue;
            }
            T f = a.cap;
            a.cap = 0;
            e[a.to][a.rev].cap += f;
            ex[s] -= f;
            ex[a.to] += f;
            enqueue(a.to);
        }
        while (maxH >= 0)
        {
            if (actHead[maxH] == -1)
            {
                maxH--;
                continue;
            }
            int u = actHead[maxH];
            actHead[maxH] = actNext[u];
            actNext[u] = -1;
            in[u] = false;
            discharge(u);
            if (ex[u] > 0 && h[u] < n)
            {
                enqueue(u);
            }
            if (work >= 4 * max(1LL, n))
            {
                relabelAll();
            }
        }
        return ex[t];
    }
};
