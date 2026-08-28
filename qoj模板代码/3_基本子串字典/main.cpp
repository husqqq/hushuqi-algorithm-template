
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct BorderInfo
{
    // mn、mx 是最短和最长真 border 长度，cnt 是真 border 数量。
    int mn = 0, mx = 0, cnt = 0;
};

namespace basic_substring_dict
{
struct Q
{
    int l, r;
};

inline vector<BorderInfo> solve(const string &s,
                                const vector<pair<int, int>> &qs)
{
    // s 是字符串；qs 是 0 下标闭区间；返回每个区间的真 border 统计。
    int n = s.size(), q = qs.size();
    vector<BorderInfo> ans(q);
    if (!n || !q)
    {
        return ans;
    }
    vector<Q> a(q);
    vector<vector<int>> lq(n + 2), rq(n + 2);
    for (int i = 0; i < q; i++)
    {
        int l = qs[i].first + 1, r = qs[i].second + 1;
        assert(1 <= l && l <= r && r <= n);
        a[i] = {l, r};
        lq[l].push_back(i);
        rq[r].push_back(i);
    }

    vector<int> sa(n + 1), rk(n + 1), id(n + n + 3), c(256);
    for (int i = 1; i <= n; i++)
    {
        c[(unsigned char)s[i - 1]]++;
    }
    for (int i = 1; i < 256; i++)
    {
        c[i] += c[i - 1];
    }
    for (int i = n; i; i--)
    {
        sa[c[(unsigned char)s[i - 1]]--] = i;
    }
    int p = 0;
    for (int i = 1; i <= n; i++)
    {
        if (i == 1 || s[sa[i] - 1] != s[sa[i - 1] - 1])
        {
            p++;
        }
        rk[sa[i]] = p;
    }

    vector<int> nx(n + 2), ll(q, n + 1), lr(q), rl(q, n + 1), rr(q);
    vector<int> lo(q, n + 1), hi(q), num(q), bu(n + 3);

    auto one = [&](int i, int x, int l, int r, int d)
    {
        if (l <= x && x <= r && (x - l) % d == 0)
        {
            hi[i] = max(hi[i], x);
            lo[i] = min(lo[i], x);
            num[i]++;
        }
    };
    auto two = [&](int i, int l1, int r1, int d1, int l2, int r2, int d2)
    {
        if (r1 - l1 <= d1)
        {
            for (int x = l1; x <= r1; x += d1)
            {
                one(i, x, l2, r2, d2);
            }
            return;
        }
        if (r2 - l2 <= d2)
        {
            for (int x = l2; x <= r2; x += d2)
            {
                one(i, x, l1, r1, d1);
            }
            return;
        }
        if ((l1 - l2) % d1)
        {
            return;
        }
        l1 = max(l1, l2);
        r1 = min(r1, r2);
        if (l1 <= r1)
        {
            hi[i] = max(hi[i], r1);
            lo[i] = min(lo[i], l1);
            num[i] += (r1 - l1) / d1 + 1;
        }
    };

    auto work = [&](int t)
    {
        fill(bu.begin(), bu.begin() + p + 1, n + 1);
        for (int i = n - t + 1; i >= 1; i--)
        {
            nx[i] = bu[rk[i]];
            bu[rk[i]] = i;
        }
        fill(bu.begin(), bu.begin() + p + 1, -1);
        for (int i = 2 - t; i <= n; i++)
        {
            if (1 <= i && i <= n)
            {
                bu[rk[i]] = i;
            }
            int j = i + t - 1;
            if (1 <= j && j <= n)
            {
                for (int z : rq[j])
                {
                    if (a[z].r - a[z].l + 1 > t)
                    {
                        lr[z] = bu[rk[a[z].l]];
                    }
                }
            }
            j = i - t + 1;
            if (1 <= j && j <= n)
            {
                for (int z : lq[j])
                {
                    if (a[z].r - a[z].l + 1 > t)
                    {
                        rr[z] = min(bu[rk[a[z].r - t + 1]], a[z].r - t + 1);
                    }
                }
            }
        }

        fill(bu.begin(), bu.begin() + p + 1, n + 1);
        for (int i = n; i + t - 1; i--)
        {
            if (1 <= i && i <= n)
            {
                bu[rk[i]] = i;
            }
            int j = i + 2 * t - 2;
            if (1 <= j && j <= n)
            {
                for (int z : rq[j])
                {
                    if (a[z].r - a[z].l + 1 > t)
                    {
                        ll[z] = max(bu[rk[a[z].l]], nx[a[z].l]);
                    }
                }
            }
            if (1 <= i && i <= n)
            {
                for (int z : lq[i])
                {
                    if (a[z].r - a[z].l + 1 > t)
                    {
                        rl[z] = bu[rk[a[z].r - t + 1]];
                    }
                }
            }
        }

        for (int i = 0; i < q; i++)
        {
            if (a[i].r - a[i].l + 1 <= t || ll[i] > lr[i] || rl[i] > rr[i])
            {
                continue;
            }
            int d1 = nx[ll[i]] - ll[i], d2 = nx[rl[i]] - rl[i];
            swap(ll[i], lr[i]);
            rl[i] += t - a[i].l;
            rr[i] += t - a[i].l;
            ll[i] = a[i].r - ll[i] + 1;
            lr[i] = a[i].r - lr[i] + 1;
            two(i, ll[i], lr[i], d1, rl[i], rr[i], d2);
        }

        if (p == n)
        {
            return;
        }
        fill(bu.begin(), bu.begin() + p + 1, 0);
        int z = 0;
        for (int i = n; i > n - t; i--)
        {
            id[++z] = i;
        }
        for (int i = 1; i <= n; i++)
        {
            if (sa[i] > t)
            {
                id[++z] = sa[i] - t;
            }
        }
        for (int i = 1; i <= n; i++)
        {
            bu[rk[i]]++;
        }
        for (int i = 2; i <= p; i++)
        {
            bu[i] += bu[i - 1];
        }
        for (int i = n; i; i--)
        {
            sa[bu[rk[id[i]]]--] = id[i];
            id[i] = rk[i];
        }
        p = 0;
        for (int i = 1; i <= n; i++)
        {
            int x = sa[i], y = sa[i - 1];
            if (i == 1 || id[x] != id[y] || id[x + t] != id[y + t])
            {
                p++;
            }
            rk[x] = p;
        }
    };

    for (int t = 1; t <= n; t <<= 1)
    {
        work(t);
    }
    for (int i = 0; i < q; i++)
    {
        if (num[i])
        {
            ans[i] = {lo[i], hi[i], num[i]};
        }
    }
    return ans;
}
}

inline vector<BorderInfo> basicDict(const string &s,
                                    const vector<pair<int, int>> &qs)
{
    // s 是字符串；qs 是 0 下标闭区间；返回每个区间的真 border 统计。
    return basic_substring_dict::solve(s, qs);
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int q;
    cin >> s >> q;
    vector<pair<int, int>> qs(q);
    for (auto &[l, r] : qs)
    {
        cin >> l >> r;
    }
    auto ans = basicDict(s, qs);
    for (auto x : ans)
    {
        if (!x.cnt)
        {
            cout << -1 << '\n';
        }
        else
        {
            cout << x.mn << ' ' << x.mx << ' ' << x.cnt << '\n';
        }
    }
}

