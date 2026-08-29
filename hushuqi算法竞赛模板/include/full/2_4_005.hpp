#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

template <class T> class GcdSegs
{
    // groups[r] 按左端点递增保存以 r 结尾的各段 gcd 分组。
    vector<vector<pair<int, T>>> groups;

  public:
    explicit GcdSegs(const vector<T> &a) : groups(a.size())
    {
        // a 是原序列；预处理每个右端点的 gcd 分组。
        vector<pair<int, T>> pre;
        for (int r = 0; r < a.size(); ++r)
        {
            vector<pair<int, T>> cur{{r, abs(a[r])}};
            for (auto [left, value] : pre)
            {
                value = gcd(value, abs(a[r]));
                if (cur.back().second == value)
                {
                    cur.back().first = left;
                }
                else
                {
                    cur.push_back({left, value});
                }
            }
            ranges::reverse(cur);
            groups[r] = cur;
            pre = cur;
            ranges::reverse(pre);
        }
    }

    T query(int left, int right) const
    {
        // 返回闭区间 [left,right] 的 gcd。
        assert(0 <= left && left <= right && right < groups.size());
        auto it = upper_bound(groups[right].begin(), groups[right].end(), pair{left, numeric_limits<T>::max()});
        return prev(it)->second;
    }

    const vector<pair<int, T>> &endingAt(int right) const
    {
        // 返回所有以 right 结尾的 gcd 分组，按左端点递增排列。
        return groups[right];
    }
};
