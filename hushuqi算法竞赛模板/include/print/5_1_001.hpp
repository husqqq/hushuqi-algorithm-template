#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

using AdjEdge = array<int, 2>;
using AdjList = vector<vector<AdjEdge>>;
using AdjMatrix = vector<vector<int>>;

class ForwardStar
{
    // head[u] 是点 u 的第一条出边编号，-1 表示没有出边。
    vector<int> head;
    // e[i][0..3] 是终点、边权、下一条同起点边和起点。
    vector<array<int, 4>> e;

  public:
    ForwardStar(int n) : head(n, -1)
    {
        // n 是点数；构造一张没有边的图。
    }

    int add(int u, int v, int w = 1)
    {
        // u、v 是端点，w 是边权；加入有向边并返回边编号。
        e.push_back({v, w, head[u], u});
        head[u] = e.size() - 1;
        return head[u];
    }

    int addResEdge(int u, int v, int w, int rw = 0)
    {
        // u、v 是正向边端点，w、rw 是正反边初值；返回正向边编号。
        int id = e.size();
        add(u, v, w);
        add(v, u, rw);
        return id;
    }

    const vector<int> &heads() const
    {
        // 无参数；返回各点第一条出边编号。
        return head;
    }

    const vector<array<int, 4>> &edges() const
    {
        // 无参数；返回边表。
        return e;
    }
};
