#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

// AdjEdge[0..1] 依次是边的终点与边权。
using AdjEdge = array<int, 2>;
using AdjList = vector<vector<AdjEdge>>;
using AdjMatrix = vector<vector<int>>;

class ForwardStar
{
    // head[u] 保存点 u 的第一条出边编号，-1 表示没有出边。
    vector<int> head;
    // e[i][0..3] 依次是第 i 条边的终点、边权、下一条同起点边和起点。
    vector<array<int, 4>> e;

  public:
    ForwardStar(int n) : head(n, -1)
    {
        // n 是点数；构造一张没有边的图，无返回值。
    }

    int add(int u, int v, int w = 1)
    {
        // u、v 是边的起点和终点，w 是边权；加入一条有向边并返回边编号。
        e.push_back({v, w, head[u], u});
        head[u] = e.size() - 1;
        return head[u];
    }

    int addResEdge(int u, int v, int w, int rw = 0)
    {
        // u、v 是正向边端点，w、rw 是正反边初值；已有边数须为偶数。
        // 返回正向边编号 id，反向边为 id^1；普通 add 不保证反向边关系。
        assert(e.size() % 2 == 0);
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
        // 无参数；返回链式前向星的边表。
        return e;
    }
};
