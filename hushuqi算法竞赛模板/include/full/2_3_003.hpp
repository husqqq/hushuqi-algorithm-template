#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

// MoQuery[0..2] 依次是左端点、右端点与答案编号。
using MoQuery = array<int, 3>;

// 状态初始为空。add(i) 加入一个位置；snap() 返回快照；undo(s) 回滚；get() 取答案。
template <class Ans, class Add, class Snap, class Undo, class Get>
vector<Ans> rollbackMo(int n, vector<MoQuery> q, Add add, Snap snap, Undo undo, Get get)
{
    // n 是序列长度，q 的每项为 {l,r,id}；add 加入位置，snap 取快照，undo 回滚。
    // get 返回当前答案，结果按 id 排列。
    int z = max<int>(1, (int)(sqrt(max<int>(1, n))));
    sort(q.begin(),
         q.end(),
         [&](const MoQuery &a, const MoQuery &b)
         {
             int x = a[0] / z, y = b[0] / z;
             return x != y ? x < y : a[1] < b[1];
         });
    vector<Ans> ans(q.size());
    auto empty = snap();
    for (int i = 0; i < q.size();)
    {
        int k = q[i][0] / z, j = i;
        while (j < q.size() && q[j][0] / z == k)
        {
            j++;
        }
        int mid = min(n, (k + 1) * z), r = mid;
        for (int p = i; p < j; p++)
        {
            auto [l, qr, id] = q[p];
            if (qr <= mid)
            {
                auto s = snap();
                for (int x = l; x < qr; x++)
                {
                    add(x);
                }
                ans[id] = get();
                undo(s);
                continue;
            }
            while (r < qr)
            {
                add(r++);
            }
            auto s = snap();
            for (int x = mid - 1; x >= l; x--)
            {
                add(x);
            }
            ans[id] = get();
            undo(s);
        }
        undo(empty);
        i = j;
    }
    return ans;
}
