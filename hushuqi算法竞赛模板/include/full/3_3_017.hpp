#include <bits/stdc++.h>
using namespace std;
#define int long long

class MajRange
{
    // S[0] 是候选值编号，S[1] 是抵消后的剩余票数。
    using S = array<int, 2>;

    // n 是数组长度，z 是线段树叶子基址；a 保存当前值编号，val 保存原值。
    int n, z = 1;
    vector<int> a, val;
    vector<S> tr;
    // pos[k] 是值 k 预登记的位置，bit[k] 维护这些位置当前是否取值 k。
    vector<vector<int>> pos, bit;

    static S merge(S x, S y)
    {
        // x、y 是相邻两段的抵消摘要；返回合并摘要。
        if (!x[1])
        {
            return y;
        }
        if (!y[1])
        {
            return x;
        }
        if (x[0] == y[0])
        {
            return {x[0], x[1] + y[1]};
        }
        return x[1] >= y[1] ? S{x[0], x[1] - y[1]} : S{y[0], y[1] - x[1]};
    }

    int id(int x) const
    {
        // x 是已预登记的原值；返回其离散编号。
        auto it = lower_bound(val.begin(), val.end(), x);
        assert(it != val.end() && *it == x); // 调试检查，可删
        return it - val.begin();
    }

    void add(int k, int p, int d)
    {
        // k 是值编号，p 是预登记位置，d 是出现次数增量。
        auto it = lower_bound(pos[k].begin(), pos[k].end(), p);
        assert(it != pos[k].end() && *it == p); // 调试检查，可删
        for (int i = it - pos[k].begin() + 1; i < (int)bit[k].size(); i += i & -i)
        {
            bit[k][i] += d;
        }
    }

    int pre(int k, int p) const
    {
        // k 是值编号，p 是位置上界；返回该值在 [0,p) 的出现次数。
        int i = lower_bound(pos[k].begin(), pos[k].end(), p) - pos[k].begin();
        int ans = 0;
        for (; i; i -= i & -i)
        {
            ans += bit[k][i];
        }
        return ans;
    }

    void setLeaf(int p, int x)
    {
        // p 是数组位置，x 是新值编号；修改候选线段树的对应叶子。
        p += z;
        tr[p] = {x, 1};
        while (p >>= 1)
        {
            tr[p] = merge(tr[2 * p], tr[2 * p + 1]);
        }
    }

    S prod(int l, int r) const
    {
        // [l,r) 是查询区间；返回按原顺序抵消后的候选摘要。
        S x{-1, 0}, y{-1, 0};
        for (l += z, r += z; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1)
            {
                x = merge(x, tr[l++]);
            }
            if (r & 1)
            {
                y = merge(tr[--r], y);
            }
        }
        return merge(x, y);
    }

  public:
    MajRange(const vector<int> &v, const vector<pair<int, int>> &plan = {}) : n(v.size())
    {
        // v 是初始数组，plan 列出以后所有可能的 {赋值位置,新值}。
        val = v;
        for (auto [p, x] : plan)
        {
            assert(0 <= p && p < n); // 调试检查，可删
            val.push_back(x);
        }
        sort(val.begin(), val.end());
        val.erase(unique(val.begin(), val.end()), val.end());
        a.resize(n);
        pos.resize(val.size());
        for (int i = 0; i < n; i++)
        {
            a[i] = id(v[i]);
            pos[a[i]].push_back(i);
        }
        for (auto [p, x] : plan)
        {
            pos[id(x)].push_back(p);
        }
        bit.resize(pos.size());
        for (int k = 0; k < (int)pos.size(); k++)
        {
            auto &p = pos[k];
            sort(p.begin(), p.end());
            p.erase(unique(p.begin(), p.end()), p.end());
            bit[k].assign(p.size() + 1, 0);
        }
        while (z < max<int>(1, n))
        {
            z *= 2;
        }
        tr.assign(2 * z, S{-1, 0});
        for (int i = 0; i < n; i++)
        {
            tr[z + i] = {a[i], 1};
            add(a[i], i, 1);
        }
        for (int p = z - 1; p; p--)
        {
            tr[p] = merge(tr[2 * p], tr[2 * p + 1]);
        }
    }

    void set(int p, int x)
    {
        // p 是数组位置，x 是已在 plan 登记的新值；完成单点赋值。
        assert(0 <= p && p < n); // 调试检查，可删
        int k = id(x);
        add(a[p], p, -1);
        add(k, p, 1);
        a[p] = k;
        setLeaf(p, k);
    }

    optional<int> query(int l, int r) const
    {
        // [l,r) 是查询区间；返回严格众数，空区间或不存在时返回空。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (l == r)
        {
            return nullopt;
        }
        S s = prod(l, r);
        if (!s[1])
        {
            return nullopt;
        }
        int k = s[0];
        int c = pre(k, r) - pre(k, l);
        return 2 * c > r - l ? optional<int>(val[k]) : nullopt;
    }
};
