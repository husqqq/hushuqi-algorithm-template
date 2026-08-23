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

template <class T, class Cmp = less<T>> void mergeSort(vector<T> &a, Cmp cmp = {})
{
    // 按严格弱序 cmp 稳定排序 a。
    vector<T> b(a.size());
    auto dfs = [&](auto &&self, int l, int r) -> void
    {
        if (r - l <= 1)
        {
            return;
        }
        int m = (l + r) / 2;
        self(self, l, m);
        self(self, m, r);
        int i = l, j = m, k = l;
        while (i < m || j < r)
        {
            if (j == r || (i < m && !cmp(a[j], a[i])))
            {
                b[k++] = move(a[i++]);
            }
            else
            {
                b[k++] = move(a[j++]);
            }
        }
        for (i = l; i < r; i++)
        {
            a[i] = move(b[i]);
        }
    };
    dfs(dfs, 0, a.size());
}

template <class T> void radixSort(vector<T> &a)
{
    // 按整数自然顺序稳定排序 a。
    static_assert(is_integral_v<T> && sizeof(T) <= 8);
    using U = make_unsigned_t<T>;
    vector<T> b(a.size());
    constexpr U sign = is_signed_v<T> ? U(1) << (sizeof(T) * 8 - 1) : 0;
    for (int k = 0; k < (int)(sizeof(T)); k++)
    {
        array<int, 256> c{};
        for (T x : a)
        {
            c[((U(x) ^ sign) >> (8 * k)) & 255]++;
        }
        partial_sum(c.begin(), c.end(), c.begin());
        for (int i = (int)a.size() - 1; i >= 0; i--)
        {
            int x = ((U(a[i]) ^ sign) >> (8 * k)) & 255;
            b[--c[x]] = a[i];
        }
        a.swap(b);
    }
}
