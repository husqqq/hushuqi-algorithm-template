#include <bits/stdc++.h>
using namespace std;
#define int long long

struct SqrtBlock
{
    int n, B, m;
    vector<int> a, tag, sum, mn, mx;

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
        sum[k] = 0;
        mn[k] = numeric_limits<int>::max();
        mx[k] = numeric_limits<int>::lowest();
        for (int i = L(k); i < R(k); i++)
        {
            sum[k] += a[i];
            mn[k] = min(mn[k], a[i]);
            mx[k] = max(mx[k], a[i]);
        }
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

    SqrtBlock(vector<int> v) : n(v.size()), a(move(v))
    {
        B = max<int>(1, (int)sqrtl(max<int>(1, n)));
        m = (n + B - 1) / B;
        tag.assign(m, 0);
        sum.resize(m);
        mn.resize(m);
        mx.resize(m);
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
                sum[k] += v * (y - x);
                mn[k] += v;
                mx[k] += v;
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

    array<int, 3> query(int l, int r) const
    {
        // 返回区间的 {和,最小值,最大值}。
        assert(0 <= l && l < r && r <= n); // 调试检查，可删。
        array<int, 3> ans{0, numeric_limits<int>::max(), numeric_limits<int>::lowest()};
        for (int k = l / B; L(k) < r; k++)
        {
            int x = max(l, L(k)), y = min(r, R(k));
            if (x == L(k) && y == R(k))
            {
                ans[0] += sum[k];
                ans[1] = min(ans[1], mn[k]);
                ans[2] = max(ans[2], mx[k]);
            }
            else
            {
                for (int i = x; i < y; i++)
                {
                    int v = a[i] + tag[k];
                    ans[0] += v;
                    ans[1] = min(ans[1], v);
                    ans[2] = max(ans[2], v);
                }
            }
        }
        return ans;
    }
};
