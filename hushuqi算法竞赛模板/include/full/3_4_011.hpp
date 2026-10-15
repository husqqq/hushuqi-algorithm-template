#pragma once

#include "../offline.hpp"

vector<int> rangeInv(const vector<int> &a, const vector<pair<int, int>> &qs)
{
    // a 是静态数组，qs 是半开区间；返回每个区间内满足 i<j 且 a[i]>a[j] 的数量。
    int n = (int)a.size();
    if (qs.empty())
    {
        return {};
    }

    // Query[0..2] 依次表示左端点、右端点、原询问编号；固定 32 位使热点排序和扫描更紧凑。
    using Query = array<int32_t, 3>;
    vector<Query> ord(qs.size());
    for (int id = 0; id < (int)qs.size(); id++)
    {
        auto [l, r] = qs[id];
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        ord[id] = {(int32_t)l, (int32_t)r, (int32_t)id};
    }

    Discrete<int> dis(a);
    vector<int32_t> rank(n);
    for (int i = 0; i < n; i++)
    {
        rank[i] = dis.id(a[i]);
    }
    vector<int32_t> bit(dis.a.size() + 1);
    auto add = [&](int x, int delta)
    {
        // x 是离散排名，delta 是频次变化；修改紧凑的 Fenwick 计数树。
        for (x++; x < (int)bit.size(); x += x & -x)
        {
            bit[x] += delta;
        }
    };
    auto sum = [&](int x)
    {
        // x 是排名上界；返回所有排名小于 x 的当前元素个数。
        int ans = 0;
        for (; x; x -= x & -x)
        {
            ans += bit[x];
        }
        return ans;
    };

    int block = max<int>(1, (int)(n / sqrtl(max<int>(1, (int)ord.size()))));
    sort(ord.begin(), ord.end(), [&](const Query &x, const Query &y)
         {
             int bx = x[0] / block, by = y[0] / block;
             if (bx != by)
             {
                 return bx < by;
             }
             return bx & 1 ? x[1] > y[1] : x[1] < y[1];
         });

    vector<int> out(qs.size());
    int l = 0, r = 0, len = 0, ans = 0;
    for (auto q : ord)
    {
        while (r < q[1])
        {
            int x = rank[r++];
            ans += len - sum(x + 1);
            add(x, 1);
            len++;
        }
        while (r > q[1])
        {
            int x = rank[--r];
            add(x, -1);
            len--;
            ans -= len - sum(x + 1);
        }
        while (l > q[0])
        {
            int x = rank[--l];
            ans += sum(x);
            add(x, 1);
            len++;
        }
        while (l < q[0])
        {
            int x = rank[l++];
            add(x, -1);
            len--;
            ans -= sum(x);
        }
        out[q[2]] = ans;
    }
    return out;
}
