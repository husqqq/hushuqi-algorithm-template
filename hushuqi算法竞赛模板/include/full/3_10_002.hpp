#include <bits/stdc++.h>
using namespace std;
#define int long long

struct OrderBlock
{
    int n, B, m;
    vector<int> a, tag;
    vector<vector<int>> s;

    int L(int k) const
    {
        return k * B;
    }

    int R(int k) const
    {
        return min(n, (k + 1) * B);
    }

    void pull(int k)
    {
        s[k].assign(a.begin() + L(k), a.begin() + R(k));
        sort(s[k].begin(), s[k].end());
    }

    void push(int k)
    {
        if (!tag[k])
        {
            return;
        }
        for (int i = L(k); i < R(k); i++)
        {
            a[i] += tag[k];
        }
        tag[k] = 0;
    }

    OrderBlock(vector<int> v) : n(v.size()), a(move(v))
    {
        B = max<int>(1, (int)sqrtl(max<int>(1, n)));
        m = (n + B - 1) / B;
        tag.assign(m, 0);
        s.resize(m);
        for (int k = 0; k < m; k++)
        {
            pull(k);
        }
    }

    void add(int l, int r, int v)
    {
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删。
        if (l == r)
        {
            return;
        }
        for (int k = l / B; L(k) < r; k++)
        {
            int x = max(l, L(k)), y = min(r, R(k));
            if (x == L(k) && y == R(k))
            {
                tag[k] += v;
            }
            else
            {
                push(k);
                for (int i = x; i < y; i++)
                {
                    a[i] += v;
                }
                pull(k);
            }
        }
    }

    int count(int l, int r, int x, bool le = false) const
    {
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删。
        int ans = 0;
        for (int k = l / B; L(k) < r; k++)
        {
            int p = max(l, L(k)), q = min(r, R(k));
            if (p == L(k) && q == R(k))
            {
                auto it = partition_point(s[k].begin(), s[k].end(),
                                          [&](int v)
                                          {
                                              return le ? v + tag[k] <= x : v + tag[k] < x;
                                          });
                ans += it - s[k].begin();
            }
            else
            {
                for (int i = p; i < q; i++)
                {
                    int v = a[i] + tag[k];
                    ans += le ? v <= x : v < x;
                }
            }
        }
        return ans;
    }

    optional<int> prev(int l, int r, int x) const
    {
        optional<int> ans;
        for (int k = l / B; L(k) < r; k++)
        {
            int p = max(l, L(k)), q = min(r, R(k));
            if (p == L(k) && q == R(k))
            {
                auto it = partition_point(s[k].begin(), s[k].end(),
                                          [&](int v)
                                          {
                                              return v + tag[k] < x;
                                          });
                if (it != s[k].begin())
                {
                    int v = *(it - 1) + tag[k];
                    if (!ans || *ans < v)
                    {
                        ans = v;
                    }
                }
            }
            else
            {
                for (int i = p; i < q; i++)
                {
                    int v = a[i] + tag[k];
                    if (v < x && (!ans || *ans < v))
                    {
                        ans = v;
                    }
                }
            }
        }
        return ans;
    }

    optional<int> next(int l, int r, int x) const
    {
        optional<int> ans;
        for (int k = l / B; L(k) < r; k++)
        {
            int p = max(l, L(k)), q = min(r, R(k));
            if (p == L(k) && q == R(k))
            {
                auto it = partition_point(s[k].begin(), s[k].end(),
                                          [&](int v)
                                          {
                                              return v + tag[k] < x;
                                          });
                if (it != s[k].end())
                {
                    int v = *it + tag[k];
                    if (!ans || v < *ans)
                    {
                        ans = v;
                    }
                }
            }
            else
            {
                for (int i = p; i < q; i++)
                {
                    int v = a[i] + tag[k];
                    if (x <= v && (!ans || v < *ans))
                    {
                        ans = v;
                    }
                }
            }
        }
        return ans;
    }

    int kth(int l, int r, int k, int lo, int hi) const
    {
        assert(0 <= k && k < r - l && lo <= hi); // 调试检查，可删。
        while (lo < hi)
        {
            int mid = midpoint(lo, hi);
            if (count(l, r, mid, true) > k)
            {
                hi = mid;
            }
            else
            {
                lo = mid + 1;
            }
        }
        return lo;
    }
};
