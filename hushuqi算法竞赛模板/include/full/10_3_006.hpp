#pragma once

#include "10_1_003.hpp"

struct IncSeq
{
    int sz; // 规范化后的边界点数，包含末尾哨兵点。
    bool bad = false; // 是否已经判定不存在合法序列。
    vector<int> lo, hi; // 单调下边界与单调含端点上边界。
    vector<Z> fac, ifac; // 阶乘与逆阶乘表。

    vector<Z> enumF(int n, vector<Z> v, int m)
    {
        // n 是 v 的长度，m 是输出长度；返回 ans[i]=sum(v[j]*C(n-1+i-j,i))。
        reverse(v.begin(), v.end());
        for (int i = 0; i < n; i++)
        {
            v[i] *= ifac[i];
        }
        reverse(v.begin(), v.end());
        assert(convFit(n, n + m - 1)); // 调试检查，可删。
        vector<Z> c(n + m - 1);
        for (int i = 0; i < n + m - 1; i++)
        {
            c[i] = fac[i];
        }
        auto z = conv(v, c);
        vector<Z> ans(m);
        for (int i = 0; i < m; i++)
        {
            ans[i] = z[n - 1 + i] * ifac[i];
        }
        return ans;
    }

    vector<Z> enumG(int n, const vector<Z> &v, int l)
    {
        // n 是 v 与输出的长度，l 是固定下标；返回 ans[i]=sum(v[j]*C(l+i-j,l),j=0..i)。
        assert(convFit(n, n)); // 调试检查，可删。
        vector<Z> c(n);
        for (int i = 0; i < n; i++)
        {
            c[i] = fac[l + i] * ifac[i];
        }
        auto z = conv(v, c);
        vector<Z> ans(n);
        for (int i = 0; i < n; i++)
        {
            ans[i] = z[i] * ifac[l];
        }
        return ans;
    }

    vector<Z> block(int n, const vector<int> &h, const vector<Z> &st)
    {
        // h 是 n 列单调网格块的高度，st 是各列底边入口值；返回最右边界各高度的路径计数。
        int m = h[n - 1];
        if (n == 1)
        {
            vector<Z> ans(m + 1);
            fill(ans.begin(), ans.end(), st[0]);
            return ans;
        }
        if (n == 2)
        {
            vector<Z> ans(m + 1);
            for (int i = 0; i <= m; i++)
            {
                ans[i] = st[0] * min(i + 1, h[0] + 1) + st[1];
            }
            return ans;
        }

        int mid = n / 2, cut = h[mid];
        vector<int> left(mid + 1), right(n - mid);
        vector<Z> ls(mid + 1), rs(n - mid);
        for (int i = 0; i <= mid; i++)
        {
            left[i] = h[i];
            ls[i] = i == mid ? Z(0) : st[i];
        }
        for (int i = mid; i < n; i++)
        {
            right[i - mid] = h[i] - cut;
            rs[i - mid] = st[i];
        }

        auto lv = block(mid + 1, left, ls);
        for (int i = cut; i >= 1; i--)
        {
            lv[i] -= lv[i - 1];
        }
        auto lf = enumF(cut + 1, lv, n - mid);
        auto lg = enumG(cut + 1, lv, n - mid - 1);
        auto rf = enumF(n - mid, rs, cut + 1);
        auto rg = enumG(n - mid, rs, cut);
        vector<Z> pre(cut);
        for (int i = 0; i < cut; i++)
        {
            pre[i] = lg[i] + rf[i];
        }
        for (int i = 0; i < n - mid; i++)
        {
            rs[i] = lf[i] + rg[i];
        }
        for (int i = n - mid - 1; i >= 1; i--)
        {
            rs[i] -= rs[i - 1];
        }
        auto rv = block(n - mid, right, rs);
        vector<Z> ans(m + 1);
        copy(pre.begin(), pre.end(), ans.begin());
        for (int i = 0; i < (int)rv.size(); i++)
        {
            ans[i + cut] = rv[i];
        }
        return ans;
    }

    IncSeq(vector<int> a, vector<int> b) : sz(a.size()), lo(move(a)), hi(move(b))
    {
        // a、b 是非空等长的半开下、上界；构造后保存可供 get 计算的单调网格边界。
        assert(sz > 0 && (int)hi.size() == sz); // 调试检查，可删。
        for (int i = 0; i < sz; i++)
        {
            assert(lo[i] >= 0 && hi[i] >= 0); // 调试检查，可删。
            hi[i]--;
            if (lo[i] > hi[i] || (i && hi[i] < lo[i - 1]))
            {
                bad = true;
            }
        }
        for (int i = sz - 2; i >= 0; i--)
        {
            hi[i] = min(hi[i], hi[i + 1]);
        }
        for (int i = 1; i < sz; i++)
        {
            lo[i] = max(lo[i], lo[i - 1]);
        }
        int al = lo[sz - 1], bl = hi[sz - 1];
        for (int i = sz - 1; i >= 1; i--)
        {
            lo[i] = lo[i - 1];
        }
        int dec = lo[0];
        for (auto &x : lo)
        {
            x -= dec;
        }
        for (auto &x : hi)
        {
            x -= dec;
        }
        sz++;
        lo.push_back(al - dec);
        // 哨兵只需比原末端高一格，使边界游走在原终点后经过唯一的收束竖边。
        hi.push_back(bl + 1 - dec);
        for (int i = 0; i < sz; i++)
        {
            if (lo[i] > hi[i] || (i && hi[i] < lo[i - 1]))
            {
                bad = true;
            }
        }

        if (bad)
        {
            return;
        }
        assert(sz <= mod + 1 && hi.back() <= mod - sz + 1); // 调试检查，可删。
        int m = sz + hi.back() - 1;
        assert(m > 0); // 调试检查，可删。
        fac.assign(m, 1);
        ifac.assign(m, 1);
        for (int i = 1; i < m; i++)
        {
            fac[i] = fac[i - 1] * i;
        }
        ifac[m - 1] = fac[m - 1].inv();
        for (int i = m - 1; i; i--)
        {
            ifac[i - 1] = ifac[i] * i;
        }
    }

    Z get()
    {
        // 无参数；返回原半开边界内非降整数序列数，不可行时返回 0。
        if (bad)
        {
            return 0;
        }
        int dist = upper_bound(lo.begin(), lo.end(), lo[0]) - lo.begin();
        int px = 0, py = lo[0], qx = dist - 1, qy = lo[0];
        if (qx == 0)
        {
            qy = hi[0];
        }
        vector<Z> cur(abs(qx - px) + abs(qy - py) + 1);
        cur[0] = 1;
        while (qx != sz - 1 || qy != hi[sz - 1])
        {
            for (int i = cur.size() - 1; i >= 1; i--)
            {
                if (i == 1 && px == 0 && py == 0)
                {
                    break;
                }
                cur[i] -= cur[i - 1];
            }
            if (py == qy)
            {
                vector<int> h(qx - px + 1);
                for (int i = 0; i <= qx - px; i++)
                {
                    h[i] = hi[px + i] - py;
                }
                cur = block(qx - px + 1, h, cur);
                px = qx;
                py = qy;
                qy = hi[qx];
            }
            else
            {
                int to = upper_bound(lo.begin(), lo.end(), qy) - lo.begin();
                vector<int> h(qy - py + 1);
                for (int i = 0; i <= qy - py; i++)
                {
                    h[i] = upper_bound(lo.begin(), lo.end(), py + i) - lo.begin() - px;
                }
                for (int i = qy - py; i >= 0; i--)
                {
                    h[i] -= h[0];
                }
                cur = block(qy - py + 1, h, cur);
                px = qx;
                py = qy;
                qx = to - 1;
            }
        }
        return cur.back();
    }
};

Z countIncSeq(const vector<int> &a, const vector<int> &b)
{
    // a、b 是非空等长的半开下、上界；返回其中全部非降整数序列数。
    return IncSeq(a, b).get();
}
