#pragma once

#include "12_1_001.hpp"

template <class Acc = long long>
struct PlugDP
{
    int n;
    int m;
    vector<vector<int>> a;

    PlugDP(vector<vector<int>> g) : n(g.size()), m(0), a(move(g))
    {
        assert(n > 0 && !a[0].empty());
        m = a[0].size();
        assert(2 * (m + 1) <= 64);
        for (int i = 0; i < n; i++)
        {
            assert((int)a[i].size() == m);
        }
    }

    int get(unsigned long long s, int p) const
    {
        return (s >> (2 * p)) & 3;
    }

    unsigned long long set(unsigned long long s, int p, int v) const
    {
        return (s & ~(3ULL << (2 * p))) | ((unsigned long long)v << (2 * p));
    }

    Acc solve()
    {
        int ex = -1;
        int ey = -1;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (!a[i][j])
                {
                    ex = i;
                    ey = j;
                }
            }
        }
        if (ex < 0)
        {
            return 0;
        }
        map<unsigned long long, Acc> cur;
        cur[0] = 1;
        Acc ans = 0;
        for (int i = 0; i < n; i++)
        {
            map<unsigned long long, Acc> nxt;
            for (auto [s, c] : cur)
            {
                nxt[s << 2] += c;
            }
            cur = nxt;
            for (int j = 0; j < m; j++)
            {
                map<unsigned long long, Acc> nx;
                for (auto [s, c] : cur)
                {
                    int l = get(s, j);
                    int u = get(s, j + 1);
                    if (a[i][j])
                    {
                        if (!l && !u)
                        {
                            nx[s] += c;
                        }
                        continue;
                    }
                    if (!l && !u)
                    {
                        if (i + 1 < n && j + 1 < m)
                        {
                            nx[set(set(s, j, 1), j + 1, 2)] += c;
                        }
                    }
                    else if (!l || !u)
                    {
                        int t = l ? l : u;
                        if (j + 1 < m)
                        {
                            nx[set(set(s, j, 0), j + 1, t)] += c;
                        }
                        if (i + 1 < n)
                        {
                            nx[set(set(s, j, t), j + 1, 0)] += c;
                        }
                    }
                    else if (l == 1 && u == 1)
                    {
                        unsigned long long t = set(set(s, j, 0), j + 1, 0);
                        int cnt = 1;
                        for (int k = j + 2; k <= m; k++)
                        {
                            int v = get(t, k);
                            if (v == 1)
                            {
                                cnt++;
                            }
                            else if (v == 2)
                            {
                                cnt--;
                                if (cnt == 0)
                                {
                                    t = set(t, k, 1);
                                    break;
                                }
                            }
                        }
                        nx[t] += c;
                    }
                    else if (l == 2 && u == 2)
                    {
                        unsigned long long t = set(set(s, j, 0), j + 1, 0);
                        int cnt = 1;
                        for (int k = j - 1; k >= 0; k--)
                        {
                            int v = get(t, k);
                            if (v == 2)
                            {
                                cnt++;
                            }
                            else if (v == 1)
                            {
                                cnt--;
                                if (cnt == 0)
                                {
                                    t = set(t, k, 2);
                                    break;
                                }
                            }
                        }
                        nx[t] += c;
                    }
                    else if (l == 2 && u == 1)
                    {
                        nx[set(set(s, j, 0), j + 1, 0)] += c;
                    }
                    else if (i == ex && j == ey && set(set(s, j, 0), j + 1, 0) == 0)
                    {
                        ans += c;
                    }
                }
                cur = nx;
            }
            map<unsigned long long, Acc> f;
            for (auto [s, c] : cur)
            {
                if (!get(s, m))
                {
                    f[s] += c;
                }
            }
            cur = f;
        }
        return ans;
    }
};
