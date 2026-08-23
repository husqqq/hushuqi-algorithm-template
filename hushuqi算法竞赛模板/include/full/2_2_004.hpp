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

class KthOffline
{
    struct Query
    {
        // left、right 是查询半开区间；kth 是该区间内从 1 开始的排名；id 是答案下标。
        int left, right, kth, id;
    };

    // values 是去重升序值域，rank[i] 是原数组第 i 项的值域编号。
    // bit 在当前分治层统计左半值域中各位置的出现次数，answer 按查询编号保存第 k 小。
    vector<int> values, rank, bit, answer;

    void add(int x, int value)
    {
        // 在位置 x 的 Fenwick 计数上加 value。
        for (++x; x < bit.size(); x += x & -x)
        {
            bit[x] += value;
        }
    }

    int prefixSum(int x) const
    {
        // 返回 Fenwick 中半开前缀 [0,x) 的计数。
        int result = 0;
        for (; x > 0; x -= x & -x)
        {
            result += bit[x];
        }
        return result;
    }

    void solve(int low, int high, vector<int> positions, vector<Query> queries)
    {
        // [low,high) 是当前值域，positions 的 rank 落在其中，queries 的答案也落在其中。
        if (queries.empty())
        {
            return;
        }
        if (high - low == 1)
        {
            for (auto query : queries)
            {
                answer[query.id] = values[low];
            }
            return;
        }
        int middle = midpoint(low, high);
        vector<int> lp, rp;
        for (int position : positions)
        {
            if (rank[position] < middle)
            {
                add(position, 1), lp.push_back(position);
            }
            else
            {
                rp.push_back(position);
            }
        }
        vector<Query> leftQs, rightQs;
        for (auto query : queries)
        {
            int count = prefixSum(query.right) - prefixSum(query.left);
            if (query.kth <= count)
            {
                leftQs.push_back(query);
            }
            else
            {
                query.kth -= count, rightQs.push_back(query);
            }
        }
        for (int position : lp)
        {
            add(position, -1);
        }
        solve(low, middle, move(lp), move(leftQs));
        solve(middle, high, move(rp), move(rightQs));
    }

  public:
    // 每个询问使用左闭右开区间 [left,right)，k 从 1 开始。
    vector<int> solve(const vector<int> &a, const vector<array<int, 3>> &inQs)
    {
        // a 是原数组；inQs 的每项为 {left,right,kth}，返回对应区间第 kth 小。
        values = a;
        ranges::sort(values);
        values.erase(unique(values.begin(), values.end()), values.end());
        rank.resize(a.size());
        for (int i = 0; i < a.size(); ++i)
        {
            rank[i] = lower_bound(values.begin(), values.end(), a[i]) - values.begin();
        }
        vector<int> positions(a.size());
        iota(positions.begin(), positions.end(), 0);
        vector<Query> queries;
        for (int id = 0; id < inQs.size(); ++id)
        {
            auto [left, right, kth] = inQs[id];
            assert(0 <= left && left < right && right <= a.size() && 1 <= kth && kth <= right - left);
            queries.push_back({left, right, kth, id});
        }
        bit.assign(a.size() + 1, 0);
        answer.resize(queries.size());
        solve(0, values.size(), move(positions), move(queries));
        return answer;
    }
};
