#pragma once

#include "11_1_003.hpp"

struct TriPointCnt
{
    vector<Point<long long>> a;
    vector<int32_t> pointL, pointM;
    vector<vector<int32_t>> edgeL, edgeM;

    TriPointCnt(const vector<Point<long long>> &vertices,
                         const vector<Point<long long>> &points) : a(vertices)
    {
        int n = a.size();
        pointL.assign(n, 0);
        pointM.assign(n, 0);
        edgeL.assign(n, vector<int32_t>(n));
        edgeM.assign(n, vector<int32_t>(n));
        for (int u = 0; u < n; u++)
        {
            for (auto p : points)
            {
                if (p.y == a[u].y)
                {
                    pointL[u] += p.x < a[u].x;
                    pointM[u] += p.x == a[u].x;
                }
            }
        }
        for (int u = 0; u < n; u++)
        {
            for (int v = 0; v < n; v++)
            {
                if (a[u].y >= a[v].y)
                {
                    continue;
                }
                for (auto p : points)
                {
                    if (a[u].y < p.y && p.y < a[v].y)
                    {
                        long long det = cross(a[u] - a[v], p - a[v]);
                        edgeL[u][v] += det < 0;
                        edgeM[u][v] += det == 0;
                    }
                }
            }
        }
    }

    bool lessYX(int u, int v) const
    {
        return pair(a[u].y, a[u].x) < pair(a[v].y, a[v].x);
    }

    int count(int x, int y, int z) const
    {
        int n = a.size();
        assert(0 <= x && x < n && 0 <= y && y < n && 0 <= z && z < n);
        if (lessYX(y, x))
        {
            swap(x, y);
        }
        if (lessYX(z, y))
        {
            swap(y, z);
        }
        if (lessYX(y, x))
        {
            swap(x, y);
        }
        long long det = cross(a[x] - a[z], a[y] - a[z]);
        if (det == 0)
        {
            return 0;
        }
        int ans = 0;
        if (a[x].y == a[y].y)
        {
            ans = edgeL[y][z] - edgeL[x][z] - edgeM[x][z];
        }
        else if (a[y].y == a[z].y)
        {
            ans = edgeL[x][z] - edgeL[x][y] - edgeM[x][y];
        }
        else if (det < 0)
        {
            ans = edgeL[x][z] - edgeL[y][z] - edgeM[y][z] -
                  edgeL[x][y] - edgeM[x][y] - pointL[y] - pointM[y];
        }
        else
        {
            ans = edgeL[x][y] + edgeL[y][z] + pointL[y] -
                  edgeL[x][z] - edgeM[x][z];
        }
        return ans;
    }
};
