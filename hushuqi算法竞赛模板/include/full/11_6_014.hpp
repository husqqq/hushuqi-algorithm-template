#pragma once

#include "11_3_001.hpp"
#include "11_6_012.hpp"

struct PlaneBasis3
{
    P3 o; // o 是局部二维坐标原点。
    P3 u; // u 是局部 x 轴单位向量。
    P3 v; // v 是局部 y 轴单位向量。
    P3 n; // n 是满足 cross(u,v)=n 的单位法向量。
};

struct MeshLoop3
{
    vector<P3> points; // points 是不重复首点的有向截面环。
    Real area;   // area 是沿截平面法向观察的有向面积。
    Real perimeter;    // perimeter 是截面环周长。
};

PlaneBasis3 planeBasis3(Plane s)
{
    // s 是非退化平面；返回以 s.p 为原点的右手正交基。
    assert(sgn(abs(s.n)) != 0); // 调试检查，可删。
    P3 n = unit(s.n);
    P3 axis;
    if (fabsl(n.x) <= fabsl(n.y) && fabsl(n.x) <= fabsl(n.z))
    {
        axis = {1, 0, 0};
    }
    else if (fabsl(n.y) <= fabsl(n.z))
    {
        axis = {0, 1, 0};
    }
    else
    {
        axis = {0, 0, 1};
    }
    P3 u = unit(cross(axis, n));
    P3 v = cross(n, u);
    return {s.p, u, v, n};
}

P planeCoord3(PlaneBasis3 b, P3 p)
{
    // b 是右手正交平面基，p 是空间点；返回 p 的局部二维投影坐标。
    P3 q = p - b.o;
    return {dot(q, b.u), dot(q, b.v)};
}

P3 planePoint3(PlaneBasis3 b, P p)
{
    // b 是右手正交平面基，p 是局部二维坐标；返回对应的空间平面点。
    return b.o + b.u * p.x + b.v * p.y;
}

vector<P3> coplanarHull(const vector<P3> &points, Plane s)
{
    // points 是 s 上的点集，s 是非退化平面；返回沿 s.n 观察逆时针的凸包顶点。
    PlaneBasis3 basis = planeBasis3(s);
    vector<P> projected;
    map<P, int> original;
    for (int i = 0; i < (int)points.size(); i++)
    {
        assert(onPlane3(points[i], s)); // 调试检查，可删。
        P q = planeCoord3(basis, points[i]);
        projected.push_back(q);
        if (!original.count(q))
        {
            original[q] = i;
        }
    }
    vector<P3> out;
    for (P q : hull(projected))
    {
        out.push_back(points[original[q]]);
    }
    return out;
}

Real meshVolume3(const vector<P3> &points, const vector<array<int, 3>> &faces)
{
    // points 是网格顶点，faces 每项三维依次是按一致外向顺序的三个顶点下标；返回封闭网格体积。
    if (faces.empty())
    {
        return 0;
    }
    assert(!points.empty() && 0 <= faces[0][0] && faces[0][0] < (int)points.size()); // 调试检查，可删。
    P3 o = points[faces[0][0]];
    Real sum = 0;
    for (auto f : faces)
    {
        for (int x : f)
        {
            assert(0 <= x && x < (int)points.size()); // 调试检查，可删。
        }
        P3 a = points[f[0]] - o;
        P3 b = points[f[1]] - o;
        P3 c = points[f[2]] - o;
        assert(sgn(abs(cross(b - a, c - a))) != 0); // 调试检查，可删。
        sum += dot(a, cross(b, c));
    }
    return fabsl(sum) / 6;
}

optional<vector<MeshLoop3>> meshSection3(const vector<P3> &points, const vector<array<int, 3>> &faces, Plane s)
{
    // points 是网格顶点，faces 每项三维依次是按一致外向顺序的三个顶点下标，s 是截平面；返回截面环，退化时返回空。
    assert(sgn(abs(s.n)) != 0); // 调试检查，可删。
    vector<Real> distance(points.size());
    vector<int> side(points.size());
    for (int i = 0; i < (int)points.size(); i++)
    {
        distance[i] = dot(points[i] - s.p, s.n) / abs(s.n);
        side[i] = sgn(distance[i]);
        if (side[i] == 0)
        {
            return nullopt;
        }
    }
    map<pair<int, int>, int> id; // id 把无向网格边映射到唯一截面点编号。
    vector<P3> nodePoint;        // nodePoint 保存各截面点的空间坐标。
    vector<int> next;            // next 保存有向截面环中的后继点编号。
    vector<int> indegree;        // indegree 保存各截面点在有向截面图中的入度。
    auto getNode = [&](int a, int b)
    {
        pair<int, int> edge = minmax(a, b);
        auto it = id.find(edge);
        if (it != id.end())
        {
            return it->second;
        }
        Real t = distance[a] / (distance[a] - distance[b]);
        int z = (int)nodePoint.size();
        id[edge] = z;
        nodePoint.push_back(points[a] + (points[b] - points[a]) * t);
        next.push_back(-1);
        indegree.push_back(0);
        return z;
    };
    for (auto f : faces)
    {
        for (int x : f)
        {
            assert(0 <= x && x < (int)points.size()); // 调试检查，可删。
        }
        P3 fn = cross(points[f[1]] - points[f[0]], points[f[2]] - points[f[0]]);
        if (sgn(abs(fn)) == 0)
        {
            return nullopt;
        }
        vector<pair<int, int>> cut;
        for (int i = 0; i < 3; i++)
        {
            int a = f[i];
            int b = f[(i + 1) % 3];
            if (side[a] * side[b] < 0)
            {
                cut.push_back({a, b});
            }
        }
        if (cut.empty())
        {
            continue;
        }
        if (cut.size() != 2)
        {
            return nullopt;
        }
        int a = getNode(cut[0].first, cut[0].second);
        int b = getNode(cut[1].first, cut[1].second);
        P3 dir = cross(s.n, fn);
        Real turn = dot(nodePoint[b] - nodePoint[a], dir);
        if (sgn(turn / abs(nodePoint[b] - nodePoint[a]) / abs(dir)) == 0)
        {
            return nullopt;
        }
        if (turn < 0)
        {
            swap(a, b);
        }
        if (next[a] != -1 || indegree[b] != 0)
        {
            return nullopt;
        }
        next[a] = b;
        indegree[b]++;
    }
    for (int i = 0; i < (int)nodePoint.size(); i++)
    {
        if (next[i] == -1 || indegree[i] != 1)
        {
            return nullopt;
        }
    }
    PlaneBasis3 basis = planeBasis3(s);
    vector<char> used(nodePoint.size());
    vector<MeshLoop3> out;
    for (int start = 0; start < (int)nodePoint.size(); start++)
    {
        if (used[start])
        {
            continue;
        }
        vector<P3> loop;
        int cur = start;
        while (!used[cur])
        {
            used[cur] = true;
            loop.push_back(nodePoint[cur]);
            cur = next[cur];
        }
        if (cur != start || loop.size() < 3)
        {
            return nullopt;
        }
        Real area2 = 0;
        Real perimeter = 0;
        for (int i = 1; i + 1 < (int)loop.size(); i++)
        {
            area2 += dot(cross(loop[i] - loop[0], loop[i + 1] - loop[0]), basis.n);
        }
        for (int i = 0; i < (int)loop.size(); i++)
        {
            int j = (i + 1) % loop.size();
            perimeter += abs(loop[j] - loop[i]);
        }
        out.push_back({loop, area2 / 2, perimeter});
    }
    return out;
}
