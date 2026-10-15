#pragma once
#include "11_1_002.hpp"

struct UpperHull
{
    struct Node
    {
        int l = -1, r = -1, bl = -1, br = -1; // l、r 是子树最左存活点与最右存活点加一（r=-1 表示子树为空），bl、br 是左右儿子上凸壳之间公切桥的端点。
    };

    int n, sz; // n 是点数，sz 是补齐到 2 的幂的叶子数。
    vector<Point<long long>> p; // p 是按坐标排序后的点。
    vector<Node> tr; // tr 是按堆式编号的线段树结点。
    vector<int> orig, pos;

    UpperHull(vector<Point<long long>> a) : n(a.size()), p(move(a))
    {
        // a 是互异整数点；建立包含全部点的动态上凸壳。
        orig.resize(n);
        iota(orig.begin(), orig.end(), 0);
        sort(orig.begin(), orig.end(), [&](int x, int y) { return p[x] < p[y]; });
        vector<Point<long long>> q(n);
        pos.resize(n);
        for (int i = 0; i < n; i++)
        {
            q[i] = p[orig[i]];
            pos[orig[i]] = i;
        }
        p.swap(q);
        sz = 1;
        while (sz < n)
        {
            sz <<= 1;
        }
        tr.resize(2 * sz);
        for (int i = 0; i < n; i++)
        {
            tr[sz + i] = {i, i + 1, i, i};
        }
        for (int i = sz - 1; i; i--)
        {
            pull(i);
        }
    }

    bool has(int x) const
    {
        // x 是线段树结点编号；返回该结点子树中是否还有未删除的点。
        return tr[x].r != -1;
    }

    void pull(int x)
    {
        // 这里的叉积还要与坐标相乘，局部扩为 __int128。
        auto cross128 = [](Point<long long> a, Point<long long> b)
        {
            return (__int128)a.x * b.y - (__int128)a.y * b.x;
        };
        if (!has(2 * x) && !has(2 * x + 1))
        {
            tr[x] = {};
            return;
        }
        if (!has(2 * x))
        {
            tr[x] = tr[2 * x + 1];
            return;
        }
        if (!has(2 * x + 1))
        {
            tr[x] = tr[2 * x];
            return;
        }
        int u = 2 * x, v = 2 * x + 1;
        long long split = p[tr[v].l].x;
        while (u < sz || v < sz)
        {
            if (u < sz && !has(2 * u))
            {
                u = 2 * u + 1;
                continue;
            }
            if (u < sz && !has(2 * u + 1))
            {
                u = 2 * u;
                continue;
            }
            if (v < sz && !has(2 * v))
            {
                v = 2 * v + 1;
                continue;
            }
            if (v < sz && !has(2 * v + 1))
            {
                v = 2 * v;
                continue;
            }
            int a = tr[u].bl, b = tr[u].br, c = tr[v].bl, d = tr[v].br;
            if (a != b && cross128(p[b] - p[a], p[c] - p[a]) > 0)
            {
                u = 2 * u;
            }
            else if (c != d && cross128(p[c] - p[b], p[d] - p[b]) > 0)
            {
                v = 2 * v + 1;
            }
            else if (a == b)
            {
                v = 2 * v;
            }
            else if (c == d)
            {
                u = 2 * u + 1;
            }
            else
            {
                __int128 x1 = cross128(p[b] - p[a], p[c] - p[a]);
                __int128 x2 = cross128(p[a] - p[b], p[d] - p[b]);
                if (x1 + x2 == 0 || x1 * p[d].x + x2 * p[c].x < (__int128)split * (x1 + x2))
                {
                    u = 2 * u + 1;
                }
                else
                {
                    v = 2 * v;
                }
            }
        }
        tr[x].l = tr[2 * x].l;
        tr[x].r = tr[2 * x + 1].r;
        tr[x].bl = tr[u].l;
        tr[x].br = tr[v].l;
    }

    void erase(int id)
    {
        // id 是原输入编号；从当前点集中删除该点。
        int x = sz + pos[id];
        tr[x] = {};
        for (x >>= 1; x; x >>= 1)
        {
            pull(x);
        }
    }

    vector<int> get() const
    {
        // 无参数；返回当前上凸壳从左到右的原输入编号。
        vector<int> ans;
        auto dfs = [&](auto &&self, int x, int l, int r) -> void
        {
            if (!has(x) || l >= r)
            {
                return;
            }
            if (x >= sz)
            {
                ans.push_back(tr[x].l);
                return;
            }
            if (!has(2 * x))
            {
                self(self, 2 * x + 1, l, r);
                return;
            }
            if (!has(2 * x + 1))
            {
                self(self, 2 * x, l, r);
                return;
            }
            if (r <= tr[x].bl)
            {
                self(self, 2 * x, l, r);
                return;
            }
            if (tr[x].br <= l)
            {
                self(self, 2 * x + 1, l, r);
                return;
            }
            self(self, 2 * x, l, tr[x].bl + 1);
            self(self, 2 * x + 1, tr[x].br, r);
        };
        if (n)
        {
            dfs(dfs, 1, 0, n);
        }
        for (auto &x : ans)
        {
            x = orig[x];
        }
        return ans;
    }
};

vector<vector<int>> convexLayers(const vector<Point<long long>> &p)
{
    // p 是互异且坐标绝对值不超过 1e9 的整数点；返回各层逆时针点编号。
    if (p.empty())
    {
        return {};
    }
    vector<Point<long long>> q;
    q.reserve(p.size());
    for (auto [x, y] : p)
    {
        assert(llabs(x) <= 1000000000 && llabs(y) <= 1000000000); // 调试检查，可删。
        q.push_back({-x, -y});
    }
    UpperHull up(p), down(q);
    vector<vector<int>> ans;
    int done = 0;
    while (done < (int)p.size())
    {
        auto a = down.get();
        auto b = up.get();
        if (a.size() == p.size() - done && b.size() == a.size())
        {
            // 剩余点全共线时两条壳相同，只保留其中一条。
        }
        else if (a.size() >= 2)
        {
            a.pop_back();
            b.pop_back();
            a.insert(a.end(), b.begin(), b.end());
            reverse(a.begin(), a.end());
        }
        ans.push_back(a);
        for (int x : a)
        {
            up.erase(x);
            down.erase(x);
            done++;
        }
    }
    return ans;
}
