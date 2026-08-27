// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 61 点分治


#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class Dist = long long>
class CentPairs
{
    static_assert(numeric_limits<Dist>::is_integer && numeric_limits<Dist>::is_signed &&
                  sizeof(Dist) >= sizeof(long long));
    int n;
    vector<vector<array<int, 2>>> g;
    vector<int> siz, par;
    vector<bool> ban;

    int cen(int s)
    {
        vector<int> ord;
        auto dfs = [&](auto &&self, int u, int p) -> void
        {
            par[u] = p;
            siz[u] = 1;
            ord.push_back(u);
            for (auto x : g[u])
            {
                int v = x[0];
                if (v == p || ban[v])
                {
                    continue;
                }
                self(self, v, u);
                siz[u] += siz[v];
            }
        };
        dfs(dfs, s, -1);
        int total = ord.size(), c = s, best = total;
        for (int u : ord)
        {
            int mx = total - siz[u];
            for (auto x : g[u])
            {
                int v = x[0];
                if (!ban[v] && par[v] == u)
                {
                    mx = max(mx, siz[v]);
                }
            }
            if (mx < best)
            {
                best = mx;
                c = u;
            }
        }
        return c;
    }

    vector<Dist> distances(int s, int p, long long w) const
    {
        vector<Dist> a;
        auto dfs = [&](auto &&self, int u, int f, Dist d) -> void
        {
            a.push_back(d);
            for (auto [v, ew] : g[u])
            {
                if (v != f && !ban[v])
                {
                    self(self, v, u, d + (Dist)ew);
                }
            }
        };
        dfs(dfs, s, p, (Dist)w);
        return a;
    }

    int pairs(vector<Dist> a, Dist lim) const
    {
        sort(a.begin(), a.end());
        int ans = 0;
        for (int l = 0, r = (int)a.size() - 1; l < r;)
        {
            if (a[r] <= lim && a[l] <= lim - a[r])
            {
                ans += r - l;
                l++;
            }
            else
            {
                r--;
            }
        }
        return ans;
    }

    int solve(int s, Dist lim)
    {
        int c = cen(s);
        ban[c] = true;
        vector<Dist> all{0};
        int ans = 0;
        for (auto [v, w] : g[c])
        {
            if (ban[v])
            {
                continue;
            }
            auto cur = distances(v, c, w);
            ans -= pairs(cur, lim);
            all.insert(all.end(), cur.begin(), cur.end());
        }
        ans += pairs(all, lim);
        for (auto x : g[c])
        {
            int v = x[0];
            if (!ban[v])
            {
                ans += solve(v, lim);
            }
        }
        return ans;
    }

  public:
    CentPairs(const vector<vector<array<int, 2>>> &g) : n(g.size()), g(g), siz(n), par(n), ban(n)
    {
    }

    int countAtMost(Dist lim)
    {
        fill(ban.begin(), ban.end(), false);
        return n ? solve(0, lim) : 0;
    }
};
