#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

// 知识点 11.3.012：三角形四心（内心、外心、垂心、费马点）。
namespace triangle_fermat
{
using Real = long double;

struct Point
{
    Real x = 0; // x 是横坐标。
    Real y = 0; // y 是纵坐标。

    Point operator+(Point p) const
    {
        // p 是另一个点或向量；返回逐坐标相加的结果。
        return {x + p.x, y + p.y};
    }

    Point operator-(Point p) const
    {
        // p 是另一个点或向量；返回从 p 指向当前点的向量。
        return {x - p.x, y - p.y};
    }

    Point operator*(Real k) const
    {
        // k 是缩放倍数；返回当前向量乘 k 的结果。
        return {x * k, y * k};
    }

    Point operator/(Real k) const
    {
        // k 是非零除数；返回当前向量除以 k 的结果。
        return {x / k, y / k};
    }
};

Real dot(Point a, Point b)
{
    // a、b 是两个向量；返回它们的点积。
    return a.x * b.x + a.y * b.y;
}

Real cross(Point a, Point b)
{
    // a、b 是两个向量；返回它们的有向叉积。
    return a.x * b.y - a.y * b.x;
}

Real cross(Point a, Point b, Point c)
{
    // a、b、c 是三个点；返回三角形 abc 的二倍有向面积。
    return cross(b - a, c - a);
}

Real len(Point a)
{
    // a 是向量；返回它的欧氏长度。
    return sqrtl(dot(a, a));
}

Point rot(Point a, Real t)
{
    // a 是向量，t 是逆时针旋转弧度；返回旋转后的向量。
    return {a.x * cosl(t) - a.y * sinl(t), a.x * sinl(t) + a.y * cosl(t)};
}

optional<Point> lineIsect(Point p, Point v, Point q, Point w)
{
    // p、q 是两直线上的点，v、w 是方向向量；返回交点，平行时返回空。
    auto d = cross(v, w);
    if (fabsl(d) <= 1E-18L)
    {
        return nullopt;
    }
    return p + v * (cross(q - p, w) / d);
}

optional<Point> incenter(Point a, Point b, Point c)
{
    // a、b、c 是三角形顶点；返回内心，三点共线或重合时返回空。
    if (fabsl(cross(a, b, c)) <= 1E-18L)
    {
        return nullopt;
    }
    auto x = len(b - c);
    auto y = len(c - a);
    auto z = len(a - b);
    return (a * x + b * y + c * z) / (x + y + z);
}

optional<Point> circumcenter(Point a, Point b, Point c)
{
    // a、b、c 是三角形顶点；返回外心，三点共线或重合时返回空。
    auto u = b - a;
    auto v = c - a;
    auto d = 2 * cross(u, v);
    if (fabsl(d) <= 1E-18L)
    {
        return nullopt;
    }
    Point q{(dot(u, u) * v.y - dot(v, v) * u.y) / d, (u.x * dot(v, v) - v.x * dot(u, u)) / d};
    return a + q;
}

optional<Point> orthocenter(Point a, Point b, Point c)
{
    // a、b、c 是三角形顶点；返回垂心，三点共线或重合时返回空。
    auto o = circumcenter(a, b, c);
    if (!o)
    {
        return nullopt;
    }
    return a + b + c - *o * 2;
}

optional<Point> fermat(Point a, Point b, Point c)
{
    // a、b、c 是三角形顶点；返回到三顶点距离和最小的费马点，退化时返回空。
    auto ar = cross(a, b, c);
    if (fabsl(ar) <= 1E-18L)
    {
        return nullopt;
    }
    auto wide = [](Point o, Point p, Point q)
    {
        auto u = p - o;
        auto v = q - o;
        return 2 * dot(u, v) <= -len(u) * len(v) + 1E-18L;
    };
    if (wide(a, b, c))
    {
        return a;
    }
    if (wide(b, a, c))
    {
        return b;
    }
    if (wide(c, a, b))
    {
        return c;
    }
    if (ar < 0)
    {
        swap(b, c);
    }
    auto pi = acosl(-1.0L);
    auto d = a + rot(b - a, -pi / 3);
    auto e = a + rot(c - a, pi / 3);
    return lineIsect(c, d - c, b, e - b);
}
} // 命名空间 triangle_fermat

// 知识点 14.1.015：把部分完全图边染色扩展成完整 1-因子分解。
namespace factor_ext
{
optional<vector<vector<int>>> extendFactor(vector<vector<int>> a, int m)
{
    // a 是部分完全图的对称颜色矩阵，主对角线为 -1；m 是颜色数；成功时返回 m+1 阶完整染色，否则返回空。
    auto n = static_cast<int>(a.size());
    if (m <= 0 || m % 2 == 0 || n > m + 1)
    {
        return nullopt;
    }
    for (auto &r : a)
    {
        if (static_cast<int>(r.size()) != n)
        {
            return nullopt;
        }
    }
    vector<vector<char>> used(n, vector<char>(m));
    vector<int> ec(m);
    for (int i = 0; i < n; i++)
    {
        if (a[i][i] != -1)
        {
            return nullopt;
        }
        for (int j = i + 1; j < n; j++)
        {
            auto c = a[i][j];
            if (c < 0 || c >= m || a[j][i] != c)
            {
                return nullopt;
            }
            if (used[i][c] || used[j][c])
            {
                return nullopt;
            }
            used[i][c] = used[j][c] = true;
            ec[c]++;
        }
    }

    auto cap = (m + 1) / 2;
    while (n < m + 1)
    {
        for (int c = 0; c < m; c++)
        {
            if (n - ec[c] > cap)
            {
                return nullopt;
            }
        }

        vector<vector<int>> g(m);
        for (int c = 0; c < m; c++)
        {
            for (int v = 0; v < n; v++)
            {
                if (!used[v][c])
                {
                    g[c].push_back(v);
                }
            }
            if (n - ec[c] < cap)
            {
                for (int v = n; v < m; v++)
                {
                    g[c].push_back(v);
                }
            }
        }

        vector<int> mt(m, -1);
        vector<int> ord(m);
        iota(ord.begin(), ord.end(), 0);
        stable_sort(ord.begin(),
                    ord.end(),
                    [&](int x, int y)
                    {
                        return g[x].size() < g[y].size();
                    });
        auto dfs = [&](auto &&self, int c, vector<char> &vis) -> bool
        {
            for (auto v : g[c])
            {
                if (vis[v])
                {
                    continue;
                }
                vis[v] = true;
                if (mt[v] == -1 || self(self, mt[v], vis))
                {
                    mt[v] = c;
                    return true;
                }
            }
            return false;
        };
        for (auto c : ord)
        {
            vector<char> vis(m);
            if (!dfs(dfs, c, vis))
            {
                return nullopt;
            }
        }

        for (auto &r : a)
        {
            r.push_back(-1);
        }
        a.push_back(vector<int>(n + 1, -1));
        used.push_back(vector<char>(m));
        for (int v = 0; v < n; v++)
        {
            auto c = mt[v];
            a[v][n] = a[n][v] = c;
            used[v][c] = used[n][c] = true;
            ec[c]++;
        }
        n++;
    }
    return a;
}
} // 命名空间 factor_ext
