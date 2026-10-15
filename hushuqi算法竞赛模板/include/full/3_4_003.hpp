#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

// 静态半群：运算只需满足结合律，不要求可逆或幂等。
template <class T, class F> struct DST
{
    // n 是数组长度，f 是结合律运算，a 是原数组，t 保存各层左右半段积。
    int n;
    F f;
    vector<T> a;
    vector<vector<T>> t;

    DST(const vector<T> &a, F f = {}) : n((int)a.size()), f(f), a(a)
    {
        // a 是静态数组，f 是满足结合律的合并运算。
        int lg = n <= 1 ? 0 : bit_width((unsigned)(n - 1));
        t.assign(lg, vector<T>(n));
        for (int k = 0; k < lg; k++)
        {
            int len = 1 << k;
            for (int l = 0; l < n; l += 2 * len)
            {
                int m = min(l + len, n), r = min(l + 2 * len, n);
                if (m == l || m == r)
                {
                    continue;
                }
                t[k][m - 1] = a[m - 1];
                for (int i = m - 2; i >= l; i--)
                {
                    t[k][i] = f(a[i], t[k][i + 1]);
                }
                t[k][m] = a[m];
                for (int i = m + 1; i < r; i++)
                {
                    t[k][i] = f(t[k][i - 1], a[i]);
                }
            }
        }
    }

    T query(int l, int r) const
    {
        // [l,r) 是非空查询区间；返回按原顺序合并的结果。
        assert(0 <= l && l < r && r <= n); // 调试检查，可删
        if (r - l == 1)
        {
            return a[l];
        }
        int k = bit_width((unsigned)(l ^ (r - 1))) - 1;
        return f(t[k][l], t[k][r - 1]);
    }
};
