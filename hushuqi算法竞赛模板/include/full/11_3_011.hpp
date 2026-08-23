#pragma once

#include "11_1_009.hpp"

using IP = Point<int>;
using IW = __int128_t;

struct DelaunayG
{
    struct Edge
    {
        int to = 0;          // to 保存有向边终点。
        int ccw = 0, cw = 0; // ccw、cw 是同起点边的逆时针、顺时针相邻边。
        int rev = 0;         // rev 保存反向边编号。
        bool on = false;     // on 表示边当前是否存在。
    };

    vector<IP> p;      // p 是按 x、y 排序且去重后的点。
    vector<Edge> e;    // e 保存可回收的旋转边结构。
    vector<int> free;  // free 保存已删除边的空闲编号。

    DelaunayG(vector<IP> p) : p(move(p))
    {
        // p 必须按字典序严格递增；构造 Delaunay 平面图。
        if (this->p.size() >= 2)
        {
            build(0, this->p.size());
        }
    }

    static bool inCircle(IP a, IP b, IP c, IP d)
    {
        // a、b、c、d 是整数点；返回 d 是否严格位于有向圆 abc 内。
        a -= d, b -= d, c -= d;
        auto cr = [](IP x, IP y)
        {
            return (IW)x.x * y.y - (IW)x.y * y.x;
        };
        auto sq = [](IP x)
        {
            return (IW)x.x * x.x + (IW)x.y * x.y;
        };
        IW z = cr(b, c) * sq(a) + cr(c, a) * sq(b) + cr(a, b) * sq(c);
        return z > 0;
    }

    int turn(int a, int b, int c) const
    {
        // a、b、c 是点下标；返回三点的精确转向。
        auto z = cross(p[b] - p[a], p[c] - p[a]);
        return (z > 0) - (z < 0);
    }

    int slot()
    {
        // 无参数；返回一个可写边编号。
        if (free.empty())
        {
            e.push_back({});
            return (int)e.size() - 1;
        }
        int x = free.back();
        free.pop_back();
        return x;
    }

    pair<int, int> makeEdge(int u, int v)
    {
        // u、v 是端点；建立一对反向边并返回编号。
        int a = slot(), b = slot();
        e[a] = {v, a, a, b, true};
        e[b] = {u, b, b, a, true};
        return {a, b};
    }

    void eraseOne(int x)
    {
        // x 是有向边；从其起点的旋转链中删除它。
        e[e[x].ccw].cw = e[x].cw;
        e[e[x].cw].ccw = e[x].ccw;
        e[x].on = false;
    }

    void eraseEdge(int x)
    {
        // x 是有向边；删除它及其反向边并回收编号。
        int y = e[x].rev;
        eraseOne(x);
        eraseOne(y);
        free.push_back(x);
        free.push_back(y);
    }

    void insertCcw(int x, int at)
    {
        // x 是新边，at 是同起点边；把 x 插到 at 的逆时针一侧。
        int y = e[at].ccw;
        e[x].ccw = y, e[y].cw = x;
        e[x].cw = at, e[at].ccw = x;
    }

    void insertCw(int x, int at)
    {
        // x 是新边，at 是同起点边；把 x 插到 at 的顺时针一侧。
        int y = e[at].cw;
        e[x].cw = y, e[y].ccw = x;
        e[x].ccw = at, e[at].cw = x;
    }

    pair<int, int> next(int x) const
    {
        // x 是当前出边；返回逆时针相邻点及对应出边。
        int v = e[x].to;
        return {v, e[e[x].rev].ccw};
    }

    pair<int, int> prev(int x) const
    {
        // x 是当前出边；返回顺时针相邻点及对应出边。
        int y = e[x].cw;
        return {e[y].to, e[y].rev};
    }

    tuple<int, int, int, int> bottom(int a, int ea, int b, int eb) const
    {
        // a、b 在两个凸壳上，ea、eb 是出边；返回共同下切线状态。
        while (true)
        {
            auto [x, ex] = prev(ea);
            if (turn(b, a, x) > 0)
            {
                a = x, ea = ex;
                continue;
            }
            auto [y, ey] = next(eb);
            if (turn(a, b, y) < 0)
            {
                b = y, eb = ey;
                continue;
            }
            return {a, ea, b, eb};
        }
    }

    pair<int, int> extreme(int a, int ea, bool low) const
    {
        // a、ea 是凸壳入口，low 控制取最小或最大编号；返回极值点状态。
        pair<int, int> ans{a, ea};
        int u = a, x = ea;
        do
        {
            tie(u, x) = next(x);
            ans = low ? min(ans, pair{u, x}) : max(ans, pair{u, x});
        } while (x != ea);
        return ans;
    }

    pair<int, int> join(int a, int ea, int b, int eb)
    {
        // a、b 是左右 Delaunay 图入口；合并两侧并返回新外壳入口。
        tie(a, ea) = extreme(a, ea, false);
        tie(b, eb) = extreme(b, eb, true);
        auto [al, eal, bl, ebl] = bottom(a, ea, b, eb);
        auto [bu, ebu, au, eau] = bottom(b, eb, a, ea);
        ebl = e[ebl].cw;
        ebu = e[ebu].cw;
        auto [ab, ba] = makeEdge(al, bl);
        insertCw(ab, eal);
        insertCcw(ba, ebl);
        if (al == au)
        {
            eau = ab;
        }
        if (bl == bu)
        {
            ebu = ba;
        }
        int ap = al, eap = eal, bp = bl, ebp = ebl;
        while (ap != au || bp != bu)
        {
            int a2 = e[eap].to, b2 = e[ebp].to;
            int na = e[eap].ccw, nb = e[ebp].cw;
            if (eap != eau && na != ab)
            {
                int a1 = e[na].to;
                if (inCircle(p[ap], p[bp], p[a2], p[a1]))
                {
                    eraseEdge(eap);
                    eap = na;
                    continue;
                }
            }
            if (ebp != ebu && nb != ba)
            {
                int b1 = e[nb].to;
                if (inCircle(p[b2], p[ap], p[bp], p[b1]))
                {
                    eraseEdge(ebp);
                    ebp = nb;
                    continue;
                }
            }
            bool takeA = ebp == ebu;
            if (eap != eau && ebp != ebu)
            {
                if (turn(ap, bp, b2) < 0)
                {
                    takeA = true;
                }
                else if (turn(a2, ap, bp) < 0)
                {
                    takeA = false;
                }
                else
                {
                    takeA = inCircle(p[ap], p[bp], p[b2], p[a2]);
                }
            }
            if (takeA)
            {
                na = e[e[eap].rev].ccw;
                auto [x, y] = makeEdge(a2, bp);
                insertCw(x, na);
                insertCcw(y, ebp);
                eap = na, ap = a2;
            }
            else
            {
                nb = e[e[ebp].rev].cw;
                auto [x, y] = makeEdge(b2, ap);
                insertCcw(x, nb);
                insertCw(y, eap);
                ebp = nb, bp = b2;
            }
        }
        return {al, ab};
    }

    pair<int, int> build(int l, int r)
    {
        // l、r 是有序点的半开区间；递归构造并返回外壳入口。
        if (r - l == 2)
        {
            auto [x, y] = makeEdge(l, l + 1);
            return {l, x};
        }
        if (r - l == 3)
        {
            int u = l, v = l + 1, w = l + 2;
            auto [uv, vu] = makeEdge(u, v);
            auto [vw, wv] = makeEdge(v, w);
            int z = turn(u, v, w);
            if (!z)
            {
                insertCcw(vu, vw);
                return {u, uv};
            }
            auto [uw, wu] = makeEdge(u, w);
            if (z > 0)
            {
                insertCw(uv, uw), insertCw(vw, vu), insertCw(wu, wv);
                return {u, uv};
            }
            insertCcw(uv, uw), insertCcw(vw, vu), insertCcw(wu, wv);
            return {v, vu};
        }
        int m = (l + r) / 2;
        auto [a, ea] = build(l, m);
        auto [b, eb] = build(m, r);
        return join(a, ea, b, eb);
    }

    vector<pair<int, int>> edges() const
    {
        // 无参数；返回每条当前无向边一次。
        vector<pair<int, int>> ans;
        for (int i = 0; i < (int)e.size(); i++)
        {
            if (e[i].on && i > e[i].rev)
            {
                ans.push_back(minmax(e[i].to, e[e[i].rev].to));
            }
        }
        sort(ans.begin(), ans.end());
        return ans;
    }
};

struct DelaunayRes
{
    struct VorEdge
    {
        P a;     // a 是线段端点或射线、直线上的一点。
        P b;     // type=0 时 b 是另一端点，否则 b 是方向向量。
        int type; // type 为 0、1、2 时分别表示线段、射线、直线。
    };

    vector<pair<int, int>> edge; // edge 保存 Delaunay 无向边原下标。
    vector<array<int, 3>> tri;   // tri 保存逆时针三角面原下标。
    vector<P> center;            // center 保存与 tri 同序的外心。
    vector<VorEdge> vor;         // vor 保存完整 Voronoi 边。
};

DelaunayRes delaunay(const vector<IP> &input)
{
    // input 是整数点集；返回 Delaunay 三角剖分及其 Voronoi 对偶。
    vector<int> ord(input.size());
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int x, int y)
    {
        return input[x] != input[y] ? input[x] < input[y] : x < y;
    });
    vector<IP> p;
    vector<int> id;
    for (int x : ord)
    {
        if (p.empty() || p.back() != input[x])
        {
            p.push_back(input[x]);
            id.push_back(x);
        }
    }
    DelaunayRes ans;
    if (p.size() < 2)
    {
        return ans;
    }
    DelaunayG g(p);
    auto es = g.edges();
    int n = p.size();
    vector<vector<int>> adj(n);
    for (auto [u, v] : es)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    auto halfInt = [](IP x)
    {
        return x.y < 0 || (x.y == 0 && x.x < 0);
    };
    map<pair<int, int>, int> pos;
    for (int u = 0; u < n; u++)
    {
        sort(adj[u].begin(), adj[u].end(), [&](int x, int y)
        {
            IP a = p[x] - p[u], b = p[y] - p[u];
            int ha = halfInt(a), hb = halfInt(b);
            auto z = cross(a, b);
            return ha != hb ? ha < hb : (z ? z > 0 : norm(a) < norm(b));
        });
        for (int i = 0; i < (int)adj[u].size(); i++)
        {
            pos[{u, adj[u][i]}] = i;
        }
    }
    set<pair<int, int>> used;
    vector<array<int, 3>> tri;
    for (auto [s, t] : es)
    {
        for (auto [u0, v0] : {pair{s, t}, pair{t, s}})
        {
            if (used.contains({u0, v0}))
            {
                continue;
            }
            vector<int> face;
            int u = u0, v = v0;
            while (!used.contains({u, v}))
            {
                used.insert({u, v});
                face.push_back(u);
                int k = pos[{v, u}];
                int w = adj[v][(k - 1 + adj[v].size()) % adj[v].size()];
                u = v, v = w;
            }
            IW area = 0;
            for (int i = 0; i < (int)face.size(); i++)
            {
                area += cross(p[face[i]], p[face[(i + 1) % face.size()]]);
            }
            if (area > 0)
            {
                assert(face.size() == 3);
                tri.push_back({face[0], face[1], face[2]});
            }
        }
    }
    map<pair<int, int>, vector<int>> side;
    for (auto e : es)
    {
        side[e];
        ans.edge.push_back(minmax(id[e.first], id[e.second]));
    }
    for (int i = 0; i < (int)tri.size(); i++)
    {
        auto x = tri[i];
        for (int k = 0; k < 3; k++)
        {
            side[minmax(x[k], x[(k + 1) % 3])].push_back(i);
        }
        P a = (P)p[x[0]], u = (P)(p[x[1]] - p[x[0]]), v = (P)(p[x[2]] - p[x[0]]);
        P c = a + (rot(u) * norm(v) - rot(v) * norm(u)) / (2 * cross(u, v));
        ans.center.push_back(c);
        ans.tri.push_back({id[x[0]], id[x[1]], id[x[2]]});
    }
    for (auto [e, f] : side)
    {
        if (f.size() == 2)
        {
            ans.vor.push_back({ans.center[f[0]], ans.center[f[1]], 0});
        }
        else if (f.size() == 1)
        {
            int u = e.first, v = e.second;
            auto x = tri[f[0]];
            bool forward = false;
            for (int k = 0; k < 3; k++)
            {
                forward |= x[k] == u && x[(k + 1) % 3] == v;
            }
            if (!forward)
            {
                swap(u, v);
            }
            P d{(Real)(p[v].y - p[u].y), (Real)(p[u].x - p[v].x)};
            ans.vor.push_back({ans.center[f[0]], d, 1});
        }
        else
        {
            P a = (P)p[e.first], b = (P)p[e.second];
            ans.vor.push_back({(a + b) / 2, rot(b - a), 2});
        }
    }
    sort(ans.edge.begin(), ans.edge.end());
    return ans;
}
