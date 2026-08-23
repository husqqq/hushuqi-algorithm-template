
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class F> vector<int> monoMin(int rows, int cols, F better)
{
    // better(r,x,y) 判断第 r 行的候选 y 是否不劣于 x；返回每行的最优列下标。
    assert(rows > 0 && cols > 0); // 调试检查，可删。
    vector<int> p(rows);
    auto dfs = [&](auto &&self, int u, int d, int l, int r) -> void
    {
        if (u == d)
        {
            return;
        }
        int x = (u + d) / 2;
        int y = l;
        for (int j = l + 1; j < r; j++)
        {
            if (better(x, y, j))
            {
                y = j;
            }
        }
        p[x] = y;
        self(self, u, x, l, y + 1);
        self(self, x + 1, d, y, r);
    };
    dfs(dfs, 0, rows, 0, cols);
    return p;
}

template <class Acc = long long>
vector<Acc> minPlus(const vector<Acc> &a, const vector<Acc> &b)
{
    // a、b 是任意整数序列；返回精确的 min-plus 卷积，任一输入为空时返回空。
    if (a.empty() || b.empty())
    {
        return {};
    }
    int n = a.size();
    int m = b.size();
    vector<Acc> c(n + m - 1);
    for (int s = 0; s < n + m - 1; s++)
    {
        int l = max<int>(0, s - m + 1);
        int r = min<int>(n - 1, s);
        Acc best = a[l] + b[s - l];
        for (int i = l + 1; i <= r; i++)
        {
            best = min(best, a[i] + b[s - i]);
        }
        c[s] = best;
    }
    return c;
}

template <class Acc = long long>
vector<Acc> minPlusCC(const vector<Acc> &a, const vector<Acc> &b)
{
    // a、b 的相邻差分均单调不降；返回精确的 min-plus 卷积，任一输入为空时返回空。
    if (a.empty() || b.empty())
    {
        return {};
    }
    int n = a.size();
    int m = b.size();
    vector<Acc> c{a[0] + b[0]};
    int i = 1;
    int j = 1;
    while (i < n || j < m)
    {
        if (j == m || (i < n && a[i] - a[i - 1] <= b[j] - b[j - 1]))
        {
            c.push_back(c.back() + a[i] - a[i - 1]);
            i++;
        }
        else
        {
            c.push_back(c.back() + b[j] - b[j - 1]);
            j++;
        }
    }
    return c;
}

template <class Acc = long long>
vector<Acc> minPlusCA(const vector<Acc> &a, const vector<Acc> &b)
{
    // a 的相邻差分单调不降，b 任意；返回精确的 min-plus 卷积，任一输入为空时返回空。
    if (a.empty() || b.empty())
    {
        return {};
    }
    int n = b.size();
    int m = a.size();
    auto better = [&](int s, int x, int y)
    {
        auto valid = [&](int j)
        {
            return j <= s && s - j < m;
        };
        if (!valid(y))
        {
            return false;
        }
        if (!valid(x))
        {
            return true;
        }
        return b[x] + a[s - x] >= b[y] + a[s - y];
    };
    vector<int> p = monoMin(n + m - 1, n, better);
    vector<Acc> c(n + m - 1);
    for (int s = 0; s < (int)c.size(); s++)
    {
        c[s] = b[p[s]] + a[s - p[s]];
    }
    return c;
}

template <class Acc = long long>
vector<Acc> minPlusConcaveArbitrary(const vector<Acc> &concave,
                                    const vector<Acc> &arbitrary)
{
    // concave 的相邻差分不增，arbitrary 任意；返回二者的 min-plus 卷积。
    if (concave.empty() || arbitrary.empty()) return {};
    int n = arbitrary.size(), m = concave.size(), h = n + m - 1;
    vector<Acc> ans(h, numeric_limits<Acc>::max());
    auto solve = [&](auto &&self, int xl, int xr, int yl, int yr) -> void
    {
        if (xl > xr || yl > yr) return;
        int yminL = max(0LL, xl - (m - 1)), ymaxL = min(n - 1, xl);
        int yminR = max(0LL, xr - (m - 1)), ymaxR = min(n - 1, xr);
        if (ymaxL >= yr && yl >= yminR)
        {
            auto better = [&](int i, int j, int k)
            {
                int x = xl + i, y1 = yr - j, y2 = yr - k;
                return arbitrary[y2] + concave[x - y2] < arbitrary[y1] + concave[x - y1];
            };
            auto p = monoMin(xr - xl + 1, yr - yl + 1, better);
            for (int x = xl; x <= xr; x++)
            {
                int y = yr - p[x - xl];
                ans[x] = min(ans[x], arbitrary[y] + concave[x - y]);
            }
            return;
        }
        if ((__int128)(xr - xl + 1) * (yr - yl + 1) <= 1024)
        {
            for (int x = xl; x <= xr; x++)
            {
                int lo = max<int>(yl, max<int>(0, x - m + 1));
                int hi = min<int>(yr, min<int>(n - 1, x));
                for (int y = lo; y <= hi; y++) ans[x] = min(ans[x], arbitrary[y] + concave[x - y]);
            }
            return;
        }
        if (xr - xl > yr - yl)
        {
            int xm = (xl + xr) / 2;
            self(self, xl, xm, yl, min<int>(yr, min<int>(n - 1, xm)));
            self(self, xm + 1, xr, max<int>(yl, max<int>(0, xm - m + 1)), yr);
        }
        else
        {
            int ym = (yl + yr) / 2;
            self(self, xl, min<int>(xr, ym + m - 1), yl, ym);
            self(self, max<int>(xl, ym + 1), xr, ym + 1, yr);
        }
    };
    solve(solve, 0, h - 1, 0, n - 1);
    return ans;
}

signed main(){ int n,m;cin>>n>>m;vector<long long>a(n),b(m);for(auto&x:a)cin>>x;for(auto&x:b)cin>>x;auto c=minPlusConcaveArbitrary(a,b);for(int i=0;i<(int)c.size();i++)cout<<c[i]<<" \n"[i+1==(int)c.size()]; }
