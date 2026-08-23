#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class Dist = long long> class KDTree
{
    // a[i] 是 {x,y,w}；t[u] 另存儿子、包围盒和子树权值和。
    vector<array<int, 3>> a;
    vector<array<int, 10>> t;
    int rt = -1;

    void pull(int u, int v)
    {
        if (v < 0)
        {
            return;
        }
        t[u][5] = min(t[u][5], t[v][5]);
        t[u][6] = max(t[u][6], t[v][6]);
        t[u][7] = min(t[u][7], t[v][7]);
        t[u][8] = max(t[u][8], t[v][8]);
        t[u][9] += t[v][9];
    }

    int build(int l, int r)
    {
        if (l == r)
        {
            return -1;
        }
        int xl = a[l][0], xr = xl, yl = a[l][1], yr = yl;
        for (int i = l + 1; i < r; i++)
        {
            xl = min(xl, a[i][0]);
            xr = max(xr, a[i][0]);
            yl = min(yl, a[i][1]);
            yr = max(yr, a[i][1]);
        }
        int d = (Dist)xr - (Dist)xl < (Dist)yr - (Dist)yl;
        int m = midpoint(l, r);
        nth_element(a.begin() + l,
                    a.begin() + m,
                    a.begin() + r,
                    [&](const auto &x, const auto &y)
                    {
                        return x[d] < y[d];
                    });
        int u = (int)t.size();
        t.push_back({a[m][0], a[m][1], a[m][2], -1, -1, a[m][0], a[m][0], a[m][1], a[m][1], a[m][2]});
        t[u][3] = build(l, m);
        t[u][4] = build(m + 1, r);
        pull(u, t[u][3]);
        pull(u, t[u][4]);
        return u;
    }

    int rectSum(int u, int xl, int xr, int yl, int yr) const
    {
        if (u < 0 || t[u][6] < xl || xr < t[u][5] || t[u][8] < yl || yr < t[u][7])
        {
            return 0;
        }
        if (xl <= t[u][5] && t[u][6] <= xr && yl <= t[u][7] && t[u][8] <= yr)
        {
            return t[u][9];
        }
        int ans = 0;
        if (xl <= t[u][0] && t[u][0] <= xr && yl <= t[u][1] && t[u][1] <= yr)
        {
            ans += t[u][2];
        }
        return ans + rectSum(t[u][3], xl, xr, yl, yr) + rectSum(t[u][4], xl, xr, yl, yr);
    }

    Dist boxDist(int u, int x, int y) const
    {
        Dist dx = 0, dy = 0;
        if (x < t[u][5])
        {
            dx = (Dist)t[u][5] - (Dist)x;
        }
        else if (t[u][6] < x)
        {
            dx = (Dist)x - (Dist)t[u][6];
        }
        if (y < t[u][7])
        {
            dy = (Dist)t[u][7] - (Dist)y;
        }
        else if (t[u][8] < y)
        {
            dy = (Dist)y - (Dist)t[u][8];
        }
        return dx * dx + dy * dy;
    }

    void nearest(int u, int x, int y, Dist &ans) const
    {
        if (u < 0 || boxDist(u, x, y) >= ans)
        {
            return;
        }
        Dist dx = (Dist)t[u][0] - (Dist)x;
        Dist dy = (Dist)t[u][1] - (Dist)y;
        ans = min(ans, dx * dx + dy * dy);
        int p = t[u][3], q = t[u][4];
        if (p >= 0 && q >= 0 && boxDist(p, x, y) > boxDist(q, x, y))
        {
            swap(p, q);
        }
        if (p >= 0)
        {
            nearest(p, x, y, ans);
        }
        if (q >= 0)
        {
            nearest(q, x, y, ans);
        }
    }

  public:
    explicit KDTree(vector<array<int, 3>> a) : a(move(a))
    {
        t.reserve(this->a.size());
        rt = build(0, (int)this->a.size());
    }

    int rectSum(int xl, int xr, int yl, int yr) const
    {
        assert(xl <= xr && yl <= yr); // 调试检查，可删
        return rectSum(rt, xl, xr, yl, yr);
    }

    optional<Dist> nearest(int x, int y) const
    {
        if (rt < 0)
        {
            return nullopt;
        }
        Dist ans = numeric_limits<Dist>::max();
        nearest(rt, x, y, ans);
        return ans;
    }
};
