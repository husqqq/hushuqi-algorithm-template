#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // 若 b 更小则以 b 更新 a；返回是否发生更新。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // 若 b 更大则以 b 更新 a；返回是否发生更新。
    return a < b ? a = b, true : false;
}

template <class T> class GcdSegs
{
    // groups[r] 按左端点递增保存以 r 结尾的各段 gcd 分组。
    vector<vector<pair<int, T>>> groups;

  public:
    explicit GcdSegs(const vector<T> &a) : groups(a.size())
    {
        // a 是原序列；预处理每个右端点的 gcd 分组。
        vector<pair<int, T>> previous;
        for (int r = 0; r < a.size(); ++r)
        {
            vector<pair<int, T>> current{{r, abs(a[r])}};
            for (auto [left, value] : previous)
            {
                value = gcd(value, abs(a[r]));
                if (current.back().second == value)
                {
                    current.back().first = left;
                }
                else
                {
                    current.push_back({left, value});
                }
            }
            ranges::reverse(current);
            groups[r] = current;
            previous = current;
            ranges::reverse(previous);
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
