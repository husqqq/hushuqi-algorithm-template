#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <class T> struct GcdRanges
{
    using U = make_unsigned_t<T>;

    vector<vector<pair<int, U>>> g; // g[r] 保存以 r 结尾的不同 GCD 及最小左端点。

    static U mag(T x)
    {
        // x 是原数组元素；返回不对最小负数取正值的无符号幅值。
        auto u = (U)x;
        if constexpr (is_signed_v<T>)
        {
            if (x < 0)
            {
                return (U)0 - u;
            }
        }
        return u;
    }

    explicit GcdRanges(const vector<T> &a) : g(a.size())
    {
        // a 是用于预处理所有右端点 GCD 分段的原数组。
        static_assert(is_integral_v<T>);
        vector<pair<int, U>> pre;
        for (int r = 0; r < (int)a.size(); r++)
        {
            auto v = mag(a[r]);
            vector<pair<int, U>> cur{{r, v}};
            for (auto [l, x] : pre)
            {
                x = gcd(x, v);
                if (cur.back().second == x)
                {
                    cur.back().first = l;
                }
                else
                {
                    cur.push_back({l, x});
                }
            }
            reverse(cur.begin(), cur.end());
            g[r] = cur;
            pre = cur;
            reverse(pre.begin(), pre.end());
        }
    }

    U query(int l, int r) const
    {
        // l、r 是闭区间端点；返回 a[l..r] 的绝对值 GCD。
        assert(0 <= l && l <= r && r < (int)g.size()); // 调试检查，可删
        auto it = upper_bound(g[r].begin(),
                              g[r].end(),
                              l,
                              [](int x, const auto &p)
                              {
                                  return x < p.first;
                              });
        return prev(it)->second;
    }
};
