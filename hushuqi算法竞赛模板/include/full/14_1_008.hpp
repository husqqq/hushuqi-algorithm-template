#pragma once
#include "3_3_001.hpp"

// 知识点 14.1.008：装箱问题的近似与小规模精确解。
namespace bin_packing
{
pair<int, vector<int>> firstFit(const vector<int> &a, int cap)
{
    // a 是到达顺序的体积，cap>=0 是容量；要求 0<=a[i]<=cap，返回箱数及各物品的 0 下标箱号。
    assert(cap >= 0); // 调试检查，可删
    int n = (int)a.size();
    vector<int> rem(n, cap), box;
    vector<MaxInfo> init(n, MaxInfo(cap));
    Seg<MaxInfo> st(init);
    box.reserve(n);
    int used = 0;
    for (int x : a)
    {
        assert(0 <= x && x <= cap); // 调试检查，可删
        int p = st.findFirst(0, n,
                             [&](MaxInfo z)
                             {
                                 // z 是区间剩余容量的最大值；返回该区间是否有能装下当前体积 x
                                 // 的箱子。
                                 return z.val >= x;
                             });
        assert(p != -1); // 前 i 件物品至多占用 i 个箱子
        rem[p] -= x;
        st.set(p, MaxInfo(rem[p]));
        used = max(used, p + 1);
        box.push_back(p);
    }
    return {used, box};
}

int firstFitDec(vector<int> a, int cap)
{
    // a 是物品体积，cap>=0 是箱容量；要求 0<=a[i]<=cap，返回降序首次适应的箱数。
    sort(a.begin(), a.end(), greater<>());
    return firstFit(a, cap).first;
}

int exactBins(const vector<int> &a, int cap)
{
    // a 是至多 24 个物品体积，cap 是箱容量；返回最少箱数。
    int n = (int)a.size();
    assert(0 <= n && n <= 24); // 调试检查，可删。
    assert(cap >= 0);          // 调试检查，可删。
    for (int x : a)
    {
        assert(0 <= x && x <= cap); // 调试检查，可删。
    }
    // dp[s][0] 是集合 s 的箱数，[1] 是末箱已用容量，按二者的字典序取最小。
    vector<array<int, 2>> dp(1LL << n, {n + 1, 0});
    dp[0] = {1, 0};
    for (int s = 0; s < (1LL << n); s++)
    {
        for (int i = 0; i < n; i++)
        {
            if (s >> i & 1)
            {
                continue;
            }
            auto cur = dp[s];
            if (a[i] <= cap - cur[1])
            {
                cur[1] += a[i];
            }
            else
            {
                cur[0]++;
                cur[1] = a[i];
            }
            dp[s | 1LL << i] = min(dp[s | 1LL << i], cur);
        }
    }
    return n == 0 ? 0 : dp.back()[0];
}
} // 命名空间 bin_packing
