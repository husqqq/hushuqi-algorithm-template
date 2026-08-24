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

template <class T> vector<int> prevLess(const vector<T> &a)
{
    // 参数 a 表示 要求每个位置前一个严格更小位置的序列；返回每个位置左侧最近严格更小元素的下标。
    // 改求最近严格更大时，把弹栈条件 >= 改为 <=；若允许相等，则分别用 > 或 <。
    vector<int> s, p(a.size(), -1);
    for (int i = 0; i < (int)a.size(); i++)
    {
        while (!s.empty() && a[s.back()] >= a[i])
        {
            s.pop_back();
        }
        if (!s.empty())
        {
            p[i] = s.back();
        }
        s.push_back(i);
    }
    return p;
}

template <class T> vector<T> windowMin(const vector<T> &a, int k)
{
    // 参数 a 表示 要求每个定长窗口最小值的序列；k 表示 窗口长度；返回每个长度 k 窗口的最小值。
    // 改最大值时把维护队尾的 >= 改为 <=；过期下标的队首删除条件不变。
    assert(1 <= k && k <= (int)a.size());
    deque<int> q;
    vector<T> ans;
    for (int i = 0; i < (int)a.size(); i++)
    {
        while (!q.empty() && q.front() <= i - k)
        {
            q.pop_front();
        }
        while (!q.empty() && a[q.back()] >= a[i])
        {
            q.pop_back();
        }
        q.push_back(i);
        if (i + 1 >= k)
        {
            ans.push_back(a[q.front()]);
        }
    }
    return ans;
}

int maxRect(const vector<int> &h)
{
    // 参数 h 表示非负直方图柱高；返回由 Acc 可表示的最大矩形面积。
    vector<int> s;
    int ans = 0;
    for (int i = 0; i <= (int)h.size(); i++)
    {
        int x = i == (int)h.size() ? 0 : h[i];
        while (!s.empty() && h[s.back()] >= x)
        {
            int p = s.back();
            s.pop_back();
            int l = s.empty() ? 0 : s.back() + 1;
            chmax(ans, h[p] * (i - l));
        }
        s.push_back(i);
    }
    return ans;
}

int maxRect(const vector<vector<int>> &a)
{
    // 参数 a 表示规则矩阵，非零元素视为 1；返回由 Acc 可表示的最大全 1 子矩形面积。
    if (a.empty())
    {
        return 0;
    }
    vector<int> h(a[0].size());
    int ans = 0;
    for (auto &row : a)
    {
        assert(row.size() == h.size());
        for (int i = 0; i < (int)h.size(); i++)
        {
            h[i] = row[i] ? h[i] + 1 : 0;
        }
        chmax(ans, maxRect(h));
    }
    return ans;
}
