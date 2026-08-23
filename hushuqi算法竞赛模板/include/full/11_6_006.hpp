#pragma once

#include "11_6_001.hpp"

vector<array<int, 3>> hull3(vector<P3> p, uint32_t seed = 0)
{
    // p 是点集，seed 是固定洗牌种子；返回外侧朝向的三角面原下标。
    int n = p.size();
    vector<int> id(n);
    iota(id.begin(), id.end(), 0);
    mt19937 rng(seed);
    for (int i = n - 1; i > 0; i--)
    {
        int j = rng() % (i + 1);
        swap(p[i], p[j]);
        swap(id[i], id[j]);
    }
    auto vol = [&](array<int, 3> f, P3 q)
    {
        return dot(cross(p[f[1]] - p[f[0]], p[f[2]] - p[f[0]]),
                   q - p[f[0]]);
    };
    int b = -1;
    for (int i = 1; i < n; i++)
    {
        if (sgn(abs(p[i] - p[0])) != 0)
        {
            b = i;
            break;
        }
    }
    if (b < 0)
    {
        return {};
    }
    swap(p[1], p[b]);
    swap(id[1], id[b]);
    b = -1;
    for (int i = 2; i < n; i++)
    {
        if (sgn(abs(cross(p[1] - p[0], p[i] - p[0]))) != 0)
        {
            b = i;
            break;
        }
    }
    if (b < 0)
    {
        return {};
    }
    swap(p[2], p[b]);
    swap(id[2], id[b]);
    b = -1;
    for (int i = 3; i < n; i++)
    {
        if (sgn(vol({0, 1, 2}, p[i])) != 0)
        {
            b = i;
            break;
        }
    }
    if (b < 0)
    {
        return {};
    }
    swap(p[3], p[b]);
    swap(id[3], id[b]);

    vector<array<int, 3>> f{{0, 1, 2}, {0, 2, 1}};
    vector<vector<int>> seen(n, vector<int>(n, -1));
    for (int i = 3; i < n; i++)
    {
        vector<array<int, 3>> keep, out;
        for (auto x : f)
        {
            if (sgn(vol(x, p[i])) > 0)
            {
                out.push_back(x);
                for (int k = 0; k < 3; k++)
                {
                    seen[x[k]][x[(k + 1) % 3]] = i;
                }
            }
            else
            {
                keep.push_back(x);
            }
        }
        for (auto x : out)
        {
            for (int k = 0; k < 3; k++)
            {
                int u = x[k], v = x[(k + 1) % 3];
                if (seen[v][u] != i)
                {
                    keep.push_back({u, v, i});
                }
            }
        }
        f.swap(keep);
    }
    for (auto &x : f)
    {
        for (int &u : x)
        {
            u = id[u];
        }
    }
    return f;
}
