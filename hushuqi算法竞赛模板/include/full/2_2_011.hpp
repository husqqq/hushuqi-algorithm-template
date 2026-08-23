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

int lis2(vector<pair<int, int>> a)
{
    // a 是二维点集；返回两维都严格递增的最长链长度。
    sort(a.begin(),
         a.end(),
         [](auto x, auto y)
         {
             return x.first != y.first ? x.first < y.first : x.second > y.second;
         });
    vector<int> d;
    for (auto [x, y] : a)
    {
        auto it = lower_bound(d.begin(), d.end(), y);
        if (it == d.end())
        {
            d.push_back(y);
        }
        else
        {
            *it = y;
        }
    }
    return d.size();
}

template <class T> int lcis(const vector<T> &a, const vector<T> &b)
{
    // 返回序列 a、b 的最长公共严格上升子序列长度。
    vector<int> f(b.size());
    for (T x : a)
    {
        int best = 0;
        for (int j = 0; j < (int)b.size(); j++)
        {
            if (b[j] < x)
            {
                best = max(best, f[j]);
            }
            if (b[j] == x)
            {
                f[j] = max(f[j], best + 1);
            }
        }
    }
    return f.empty() ? 0 : *max_element(f.begin(), f.end());
}
