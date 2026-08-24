#pragma once

#include "base.hpp"

template <class F> int firstTrue(int l, int r, F f)
{
    // l、r 是答案所在闭区间，f 是由假变真的谓词；返回第一个为真的位置。
    while (l < r)
    {
        auto m = midpoint(l, r);
        if (f(m))
        {
            r = m;
        }
        else
        {
            l = m + 1;
        }
    }
    return l;
}

template <class F> int lastTrue(int l, int r, F f)
{
    // l、r 是答案所在闭区间，f 是由真变假的谓词；返回最后一个为真的位置。
    while (l < r)
    {
        auto m = midpoint(l, r);
        if (m == l)
        {
            m = r;
        }
        if (f(m))
        {
            l = m;
        }
        else
        {
            r = m - 1;
        }
    }
    return l;
}

template <class F> long double firstReal(long double l, long double r, F f, int it = 100)
{
    // l、r 是初始实数区间，f 是单调谓词，it 是迭代轮数；返回分界点近似值。
    for (int t = 0; t < it; t++)
    {
        auto m = midpoint(l, r);
        if (f(m))
        {
            r = m;
        }
        else
        {
            l = m;
        }
    }
    return midpoint(l, r);
}

template <class F> int ternaryInt(int l, int r, F f)
{
    // l、r 是整数闭区间，f 是单峰函数；返回一个最大值位置。
    // 求最小值要把收缩循环和末尾扫描中的 f 比较同时反向；只改一处会得到错误区间。
    while (r - l > 4)
    {
        auto d = r - l;
        auto x = l + d / 3;
        auto y = r - d / 3;
        if (f(x) < f(y))
        {
            l = x + 1;
        }
        else
        {
            r = y - 1;
        }
    }
    auto p = l;
    auto x = l;
    while (x < r)
    {
        x++;
        if (f(p) < f(x))
        {
            p = x;
        }
    }
    return p;
}

template <class F> long double ternaryReal(long double l, long double r, F f, int it = 120)
{
    // l、r 是实数区间，f 是单峰函数，it 是迭代轮数；返回最大值位置近似值。
    // 求最小值时反向循环内的 f(x)、f(y) 比较；若改传 -f，先确认取负不会溢出。
    for (int t = 0; t < it; t++)
    {
        auto x = lerp(l, r, 1.0L / 3);
        auto y = lerp(l, r, 2.0L / 3);
        if (f(x) < f(y))
        {
            l = x;
        }
        else
        {
            r = y;
        }
    }
    return midpoint(l, r);
}

template <class F> long double goldenMax(long double l, long double r, F f, int it = 120)
{
    // l、r 是实数区间，f 是单峰函数，it 是迭代轮数；返回黄金分割搜索结果。
    // 求最小值时反向 fx、fy 的比较；其余复用点和区间更新方向不变。
    constexpr long double ratio = 0.6180339887498948482L;
    auto x = lerp(l, r, 1 - ratio);
    auto y = lerp(l, r, ratio);
    auto fx = f(x);
    auto fy = f(y);
    for (int t = 0; t < it; t++)
    {
        if (fx < fy)
        {
            l = x;
            x = y;
            fx = fy;
            y = lerp(l, r, ratio);
            fy = f(y);
        }
        else
        {
            r = y;
            y = x;
            fy = fx;
            x = lerp(l, r, 1 - ratio);
            fx = f(x);
        }
    }
    return midpoint(l, r);
}

vector<int> bfs(const vector<vector<int>> &g, int s)
{
    // g 是邻接表，s 是起点；返回各顶点到 s 的最少边数，-1 表示不可达。
    vector<int> d(g.size(), -1);
    queue<int> q;
    d[s] = 0;
    q.push(s);
    while (!q.empty())
    {
        auto x = q.front();
        q.pop();
        for (auto y : g[x])
        {
            if (d[y] != -1)
            {
                continue;
            }
            d[y] = d[x] + 1;
            q.push(y);
        }
    }
    return d;
}

vector<int> dfsOrder(const vector<vector<int>> &g, int s)
{
    // g 是邻接表，s 是起点；返回从 s 出发的递归 DFS 首次访问顺序。
    vector<int> ans, vis(g.size());
    auto dfs = [&](auto &&self, int x) -> void
    {
        vis[x] = 1;
        ans.push_back(x);
        for (auto y : g[x])
        {
            if (!vis[y])
            {
                self(self, y);
            }
        }
    };
    dfs(dfs, s);
    return ans;
}

// WEdge[0] 表示终点，WEdge[1] 表示边权。
using WEdge = array<int, 2>;

optional<int> kthShortest(const vector<vector<WEdge>> &g, int s, int t, int k, bool allowEmpty = false)
{
    // g 是非负权图，s、t 是起终点，k 是排名，allowEmpty 表示是否计入空路；返回第 k 短游走长度，不存在时为空。
    auto n = (int)g.size();
    assert(0 <= s && s < n && 0 <= t && t < n); // 调试检查，可删
    assert(k >= 1);                              // 调试检查，可删
    vector<vector<WEdge>> rg(n);
    for (int x = 0; x < n; x++)
    {
        for (auto [y, w] : g[x])
        {
            assert(0 <= y && y < n && w >= 0); // 调试检查，可删
            rg[y].push_back({x, w});
        }
    }
    vector<optional<int>> h(n);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> dq;
    h[t] = 0;
    dq.push({0, t});
    while (!dq.empty())
    {
        auto [dx, x] = dq.top();
        dq.pop();
        if (!h[x] || dx != *h[x])
        {
            continue;
        }
        for (auto [y, w] : rg[x])
        {
            auto d = dx + w;
            if (!h[y] || d < *h[y])
            {
                h[y] = d;
                dq.push({d, y});
            }
        }
    }
    if (!h[s])
    {
        return nullopt;
    }
    auto need = (unsigned long long)k + (s == t && !allowEmpty);
    // State 依次保存估价距离、已走距离和当前顶点。
    using State = tuple<int, int, int>;
    priority_queue<State, vector<State>, greater<>> q;
    vector<unsigned long long> cnt(n);
    q.push({*h[s], 0, s});
    while (!q.empty())
    {
        auto cur = q.top();
        q.pop();
        auto d = get<1>(cur);
        auto x = get<2>(cur);
        cnt[x]++;
        if (cnt[x] > need)
        {
            continue;
        }
        if (x == t && cnt[x] == need)
        {
            return d;
        }
        for (auto [y, w] : g[x])
        {
            if (!h[y])
            {
                continue;
            }
            auto nd = d + w;
            q.push({nd + *h[y], nd, y});
        }
    }
    return nullopt;
}

template <class S, class Score, class Next, class Rng>
pair<S, long double> anneal(S cur,
                            Score score,
                            Next next,
                            Rng &rng,
                            long double t = 1,
                            long double stop = 1E-8L,
                            long double rate = .995L,
                            int each = 1)
{
    // cur 是初态，score/next 是评分与邻域函数，rng 是随机源；其余参数控制降温，返回最佳状态及分数。
    assert(t > stop && stop > 0 && 0 < rate && rate < 1 && each > 0); // 调试检查，可删
    long double val = (long double)score(cur);
    auto best = cur;
    long double ans = val;
    uniform_real_distribution<long double> dis(0, 1);
    while (t > stop)
    {
        for (int z = 0; z < each; z++)
        {
            auto p = next(cur, rng, t);
            long double v = (long double)score(p);
            long double d = v - val;
            if (d <= 0 || dis(rng) < expl(-d / t))
            {
                cur = move(p);
                val = v;
            }
            if (val < ans)
            {
                best = cur;
                ans = val;
            }
        }
        t *= rate;
    }
    return {best, ans};
}
