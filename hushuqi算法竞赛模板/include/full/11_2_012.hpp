#pragma once

#include "11_1_002.hpp"

template <class T, class W = long long> struct PlanarEmbed
{
    struct Edge
    {
        int u;    // u 是无向边的一个端点。
        int v;    // v 是无向边的另一个端点。
        W weight; // weight 是穿过该边时对应的对偶边权。
    };

    vector<Point<T>> point;            // point 保存各顶点的互异整数坐标。
    vector<Edge> edge;                 // edge 按加入顺序保存无自环、无重复的无向边。
    vector<array<int, 2>> halfEdge;    // halfEdge[h][0]、[1] 分别是半边 h 的起点、终点。
    vector<int> nextHalfEdge;          // nextHalfEdge[h] 是沿 h 左侧面边界继续走的下一条半边。
    vector<int> faceOfHalf;        // faceOfHalf[h] 是半边 h 左侧面的编号。
    vector<__int128> faceArea2;        // faceArea2[f] 是面 f 边界游走的有符号双倍面积。
    int outer = -1;                // outer 是外部面的编号。
    vector<vector<pair<int, W>>> dual; // dual[f] 的元素是相邻面编号和分隔边权，允许平行边。

    explicit PlanarEmbed(vector<Point<T>> point = {}) : point(move(point))
    {
        // point 是平面嵌入各顶点的互异整数坐标；建立尚未加入边的嵌入。
        static_assert(is_integral_v<T>);
    }

    void addEdge(int u, int v, const W &weight = W(1))
    {
        // u、v 是不同端点编号，weight 是穿过该边的代价；加入一条无向边，无返回值。
        assert(0 <= u && u < (int)point.size());   // 调试检查，可删。
        assert(0 <= v && v < (int)point.size());   // 调试检查，可删。
        assert(u != v && !(point[u] == point[v])); // 调试检查，可删。
        edge.push_back({u, v, weight});
    }

    void build()
    {
        // 无参数；枚举各半边左侧面并填写 nextHalfEdge、faceOfHalf、faceArea2、outer 与 dual，无返回值。
        int n = point.size();
        int m = edge.size();
        halfEdge.assign(2 * m, {});
        nextHalfEdge.assign(2 * m, -1);
        faceOfHalf.assign(2 * m, -1);
        faceArea2.clear();
        outer = -1;
        dual.clear();

        set<Point<T>> uniquePoint(point.begin(), point.end());
        assert((int)uniquePoint.size() == n); // 调试检查，可删。
        vector<vector<int>> around(n);
        set<pair<int, int>> uniqEdge;
        for (int i = 0; i < m; i++)
        {
            auto [u, v, weight] = edge[i];
            assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
            assert(u != v && !(point[u] == point[v]));  // 调试检查，可删。
            pair<int, int> key{min(u, v), max(u, v)};
            assert(uniqEdge.insert(key).second); // 调试检查，可删。
            halfEdge[2 * i] = {u, v};
            halfEdge[2 * i + 1] = {v, u};
            around[u].push_back(2 * i);
            around[v].push_back(2 * i + 1);
        }

        if (m == 0)
        {
            assert(n <= 1); // 多点输入仍要求连通。
            faceArea2.push_back(0);
            outer = 0;
            dual.resize(1);
            return;
        }

        vector<char> seen(n);
        queue<int> q;
        seen[0] = true;
        q.push(0);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (int h : around[u])
            {
                int v = halfEdge[h][1];
                if (!seen[v])
                {
                    seen[v] = true;
                    q.push(v);
                }
            }
        }
        assert(count(seen.begin(), seen.end(), true) == n); // 调试检查，可删。

        auto dir = [&](int h)
        {
            // h 是半边编号；返回先提升到 __int128 再相减得到的方向向量。
            int u = halfEdge[h][0];
            int v = halfEdge[h][1];
            __int128 x = (__int128)point[v].x - (__int128)point[u].x;
            __int128 y = (__int128)point[v].y - (__int128)point[u].y;
            return pair<__int128, __int128>{x, y};
        };
        auto halfPlane = [](__int128 x, __int128 y)
        {
            // x、y 是非零方向向量坐标；返回极角排序使用的半平面编号。
            return y < 0 || (y == 0 && x < 0);
        };

        vector<int> position(2 * m);
        for (int u = 0; u < n; u++)
        {
            sort(around[u].begin(),
                 around[u].end(),
                 [&](int a, int b)
                 {
                     // a、b 是从 u 出发的半边编号；返回 a 是否在逆时针极角序中先于 b。
                     if (a == b)
                     {
                         return false;
                     }
                     auto [ax, ay] = dir(a);
                     auto [bx, by] = dir(b);
                     int ha = halfPlane(ax, ay);
                     int hb = halfPlane(bx, by);
                     if (ha != hb)
                     {
                         return ha < hb;
                     }
                     __int128 z = ax * by - ay * bx;
                     if (z != 0)
                     {
                         return z > 0;
                     }
                     return a < b;
                 });
            for (int i = 0; i < (int)around[u].size(); i++)
            {
                position[around[u][i]] = i;
                if (i == 0)
                {
                    continue;
                }
                auto [ax, ay] = dir(around[u][i - 1]);
                auto [bx, by] = dir(around[u][i]);
                if (halfPlane(ax, ay) == halfPlane(bx, by))
                {
                    assert(ax * by - ay * bx != 0); // 不允许两条边占用同一射线。
                }
            }
        }

        for (int h = 0; h < 2 * m; h++)
        {
            int v = halfEdge[h][1];
            int reverse = h ^ 1;
            int d = around[v].size();
            int pos = position[reverse];
            nextHalfEdge[h] = around[v][(pos + d - 1) % d];
        }

        for (int start = 0; start < 2 * m; start++)
        {
            if (faceOfHalf[start] != -1)
            {
                continue;
            }
            int face = faceArea2.size();
            int h = start;
            __int128 area = 0;
            do
            {
                assert(faceOfHalf[h] == -1); // 调试检查，可删。
                faceOfHalf[h] = face;
                auto [u, v] = halfEdge[h];
                area += (__int128)point[u].x * (__int128)point[v].y - (__int128)point[u].y * (__int128)point[v].x;
                h = nextHalfEdge[h];
            } while (h != start);
            faceArea2.push_back(area);
        }

        outer = min_element(faceArea2.begin(), faceArea2.end()) - faceArea2.begin();
        dual.assign(faceArea2.size(), {});
        for (int i = 0; i < m; i++)
        {
            int a = faceOfHalf[2 * i];
            int b = faceOfHalf[2 * i + 1];
            if (a == b)
            {
                continue;
            }
            dual[a].push_back({b, edge[i].weight});
            dual[b].push_back({a, edge[i].weight});
        }
    }
};
