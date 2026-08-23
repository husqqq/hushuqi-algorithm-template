#pragma once

#include "14_1_001.hpp"
#include "3_3_001.hpp"

namespace bin_packing
{
int firstFitDec(vector<int> a, int cap)
{
    // a 是物品体积，cap 是箱容量；返回降序首次适应使用的箱数。
    // 调试检查，可删。
    assert(cap >= 0);
    for (int x : a)
    {
        // 调试检查，可删。
        assert(0 <= x && x <= cap);
    }
    sort(a.begin(), a.end(), greater<>());
    vector<int> rem;
    for (int x : a)
    {
        int p = 0;
        while (p < (int)rem.size() && rem[p] < x)
        {
            p++;
        }
        if (p == (int)rem.size())
        {
            rem.push_back(cap);
        }
        rem[p] -= x;
    }
    return rem.size();
}

pair<int, vector<int>> firstFit(const vector<int> &a, int cap)
{
    // 按输入顺序放置，返回使用箱数与每件物品所在的 0 下标箱号。
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
        int p = st.findFirst(0, n, [&](MaxInfo z) { return z.val >= x; });
        assert(p != -1); // 前 i 件物品至多占用 i 个箱子
        rem[p] -= x;
        st.set(p, MaxInfo(rem[p]));
        used = max(used, p + 1);
        box.push_back(p);
    }
    return {used, box};
}

int exactBins(const vector<int> &a, int cap)
{
    // a 是至多 24 个物品体积，cap 是箱容量；返回最少箱数。
    int n = (int)a.size();
    // 调试检查，可删。
    assert(0 <= n && n <= 24);
    // 调试检查，可删。
    assert(cap >= 0);
    for (int x : a)
    {
        // 调试检查，可删。
        assert(0 <= x && x <= cap);
    }
    // State[0..1] 依次是已用箱数与最后一个箱的已用容量。
    using State = array<int, 2>;
    State inf{n + 1, 0};
    vector<State> dp(1LL << n, inf);
    dp[0] = {1, 0};
    for (int s = 0; s < (1LL << n); s++)
    {
        for (int i = 0; i < n; i++)
        {
            if (s >> i & 1)
            {
                continue;
            }
            State cur = dp[s];
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
}
