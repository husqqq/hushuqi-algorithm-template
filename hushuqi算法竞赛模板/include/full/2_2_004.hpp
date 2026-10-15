#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

#include "2_2_002.hpp"

struct Fenwick
{
    int n = 0;     // n 是下标范围长度。
    vector<int> a; // a 保存从 1 开始的树状数组节点和。

    Fenwick(int m = 0)
    {
        // m 是初始下标范围长度。
        init(m);
    }

    void init(int m)
    {
        // m 是新的下标范围长度；调用后全部频率清零。
        assert(m >= 0); // 调试检查，可删
        n = m;
        a.assign(n + 1, 0);
    }

    void add(int p, int v)
    {
        // p 是从 0 开始的位置，v 是增量。
        assert(0 <= p && p < n); // 调试检查，可删
        for (p++; p <= n; p += p & -p)
        {
            a[p] += v;
        }
    }

    int sum(int r) const
    {
        // r 是右端点；返回半开前缀 [0,r) 的和。
        assert(0 <= r && r <= n); // 调试检查，可删
        int ans = 0;
        for (; r > 0; r -= r & -r)
        {
            ans += a[r];
        }
        return ans;
    }

    int sum(int l, int r) const
    {
        // l、r 是半开区间端点；返回 [l,r) 的和。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return sum(r) - sum(l);
    }
};

// KthQuery[0..2] 依次表示左端点、右端点、从 1 开始的排名。
using KthQuery = array<int, 3>;

vector<int> rangeKth(const vector<int> &a, vector<KthQuery> qs)
{
    // a 是静态数组，qs 中每项是 {l,r,k}；返回各半开区间第 k 小值。
    if (qs.empty())
    {
        return {};
    }
    assert(!a.empty()); // 调试检查，可删
    for (auto q : qs)
    {
        assert(0 <= q[0] && q[0] < q[1] && q[1] <= (int)a.size()); // 调试检查，可删
        assert(1 <= q[2] && q[2] <= q[1] - q[0]);                  // 调试检查，可删
    }
    Discrete<int> dis(a);
    vector<int> rk(a.size());
    for (int i = 0; i < (int)a.size(); i++)
    {
        rk[i] = dis.id(a[i]);
    }
    vector<int> es(a.size()), ids(qs.size()), ans(qs.size());
    iota(es.begin(), es.end(), 0);
    iota(ids.begin(), ids.end(), 0);
    Fenwick bit(a.size());
    auto solve = [&](auto &&self, int vl, int vr, vector<int> ps, vector<int> qi) -> void
    {
        if (qi.empty())
        {
            return;
        }
        if (vr - vl == 1)
        {
            for (auto id : qi)
            {
                ans[id] = dis.value(vl);
            }
            return;
        }
        auto vm = midpoint(vl, vr);
        vector<int> pl, pr, ql, qr;
        for (auto p : ps)
        {
            if (rk[p] < vm)
            {
                bit.add(p, 1);
                pl.push_back(p);
            }
            else
            {
                pr.push_back(p);
            }
        }
        for (auto id : qi)
        {
            auto &q = qs[id];
            auto c = bit.sum(q[0], q[1]);
            if (q[2] <= c)
            {
                ql.push_back(id);
            }
            else
            {
                q[2] -= c;
                qr.push_back(id);
            }
        }
        for (auto p : pl)
        {
            bit.add(p, -1);
        }
        self(self, vl, vm, move(pl), move(ql));
        self(self, vm, vr, move(pr), move(qr));
    };
    solve(solve, 0, dis.a.size(), move(es), move(ids));
    return ans;
}
