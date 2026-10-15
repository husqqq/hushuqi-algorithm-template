#include <bits/stdc++.h>
using namespace std;
#define int long long

struct OrderBlock
{
    int n, B, m; // n 是长度，B 是块长，m 是块数。
    vector<int> a, tag; // a 是元素（不含所在块标记），tag 是块加标记。
    vector<vector<int>> s; // s[k] 是第 k 块元素（不含标记）的升序副本。

    int L(int k) const
    {
        // k 是块编号；返回第 k 块的起始下标（含）。
        return k * B;
    }

    int R(int k) const
    {
        // k 是块编号；返回第 k 块的结束下标（不含），末块截到 n。
        return min(n, (k + 1) * B);
    }

    void pull(int k)
    {
        // k 是块编号；用块内 a 值重建升序副本 s[k]，要求该块标记已下放（tag[k]=0）。
        s[k].assign(a.begin() + L(k), a.begin() + R(k));
        sort(s[k].begin(), s[k].end());
    }

    void push(int k)
    {
        // k 是块编号；把 tag[k] 加到块内每个 a[i] 后清零。
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
        // v 是初始数列（0 起下标）；取块长 B≈√n 分块，清零标记并为每块建立升序副本。
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
        // [l,r) 是 0 起半开区间，v 是增量；给区间内每个元素加 v，整块只改标记，零散块下放后重排。
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
        // [l,r) 是查询区间，x 是阈值；le 为假时返回区间内 <x 的元素个数，为真时返回 ≤x 的个数。
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
        // [l,r) 是查询区间，x 是阈值；返回区间内严格小于 x 的最大元素，不存在时为空。
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
        // [l,r) 是查询区间，x 是阈值；返回区间内大于等于 x 的最小元素，不存在时为空。
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
        // [l,r) 是查询区间，k 从 0 计数，[lo,hi] 是须覆盖答案的整数值域；二分返回区间第 k 小的元素值。
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
