#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

class BIT2
{
    // xs 是可更新横坐标；ys[i] 是外层节点 i 收集的纵坐标。
    vector<int> xs;
    vector<vector<int>> ys;
    // tr[i] 是外层节点 i 对应的纵坐标 BIT。
    vector<vector<long long>> tr;

  public:
    explicit BIT2(const vector<pair<int, int>> &pts = {})
    {
        // pts 列出所有可能发生更新的二维坐标。
        prepare(pts);
    }

    void prepare(const vector<pair<int, int>> &pts)
    {
        // pts 列出所有可能更新点；清空旧权值并重建两层离散索引。
        xs.clear();
        for (auto [x, y] : pts)
        {
            xs.push_back(x);
        }
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        int n = xs.size();
        ys.assign(n + 1, {});
        for (auto [x, y] : pts)
        {
            int p = lower_bound(xs.begin(), xs.end(), x) - xs.begin() + 1;
            for (int i = p; i <= n; i += i & -i)
            {
                ys[i].push_back(y);
            }
        }
        tr.resize(n + 1);
        for (int i = 1; i <= n; ++i)
        {
            auto &v = ys[i];
            sort(v.begin(), v.end());
            v.erase(unique(v.begin(), v.end()), v.end());
            tr[i].assign(v.size() + 1, 0);
        }
    }

    void add(int x, int y, long long v)
    {
        // (x,y) 必须在构造时登记，v 是该点的权值增量。
        int n = xs.size();
        auto it = lower_bound(xs.begin(), xs.end(), x);
        assert(it != xs.end() && *it == x); // 调试检查，可删
        int p = it - xs.begin() + 1;
        for (int i = p; i <= n; i += i & -i)
        {
            auto jt = lower_bound(ys[i].begin(), ys[i].end(), y);
            assert(jt != ys[i].end() && *jt == y); // 调试检查，可删
            int q = jt - ys[i].begin() + 1;
            for (int j = q; j < (int)tr[i].size(); j += j & -j)
            {
                tr[i][j] += v;
            }
        }
    }

    long long prefixSum(int x, int y) const
    {
        // x、y 是开前缀上界；返回横坐标小于 x 且纵坐标小于 y 的点权和。
        int p = lower_bound(xs.begin(), xs.end(), x) - xs.begin();
        long long ans = 0;
        for (int i = p; i > 0; i -= i & -i)
        {
            int q = lower_bound(ys[i].begin(), ys[i].end(), y) - ys[i].begin();
            for (int j = q; j > 0; j -= j & -j)
            {
                ans += tr[i][j];
            }
        }
        return ans;
    }

    void addRectangle(int l, int d, int r, int u, long long w)
    {
        // [l,r) x [d,u) 是半开加权矩形，w 是增量；四角必须已在构造时预登记。
        assert(l < r && d < u); // 调试检查，可删
        add(l, d, w);
        add(l, u, -w);
        add(r, d, -w);
        add(r, u, w);
    }

    long long pointGet(int x, int y) const
    {
        // (x,y) 是查询点；返回所有包含该点的半开加权矩形的权值和。
        return prefixSum(x + 1, y + 1);
    }
};
