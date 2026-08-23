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

template <class S, class F, class G, class R>
pair<S, long double> anneal(
    S cur, F score, G next, R &rng, long double t = 1, long double stop = 1E-8L, long double rate = .995L, int each = 1)
{
    // cur 是初态，score 越小越优，next(cur,rng,t) 生成邻态；返回最佳状态及其分数。
    // t、stop、rate 控制温度区间与降温比例，each 是每个温度的尝试次数。
    long double val = score(cur);
    S best = cur;
    long double ans = val;
    uniform_real_distribution<long double> dis(0, 1);
    for (; t > stop; t *= rate)
    {
        for (int z = 0; z < each; z++)
        {
            S p = next(cur, rng, t);
            long double v = score(p), d = v - val;
            if (d <= 0 || dis(rng) < expl(-d / t))
            {
                cur = move(p), val = v;
            }
            if (val < ans)
            {
                best = cur, ans = val;
            }
        }
    }
    return {best, ans};
}
