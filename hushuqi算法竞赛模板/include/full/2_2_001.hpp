#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

template <class T, class Cmp = less<T>> void mergeSort(vector<T> &a, Cmp cmp = {})
{
    // a 是待排序数组，cmp 比较两个 const T& 并构成严格弱序；稳定排序后写回 a，无返回值。
    // 降序传 greater<T> 或反向比较器；不要用 <=，等价元素仍须先取左段。
    assert(cmp_less_equal(a.size(), numeric_limits<int>::max()));
    vector<T> b;
    b.reserve(a.size());
    auto dfs = [&](auto &&self, int l, int r) -> void
    {
        if (r - l <= 1)
        {
            return;
        }
        auto m = midpoint(l, r);
        self(self, l, m);
        self(self, m, r);
        auto i = l;
        auto j = m;
        b.clear();
        while (i < m || j < r)
        {
            if (j == r || (i < m && !cmp(a[j], a[i])))
            {
                b.push_back(move(a[i++]));
            }
            else
            {
                b.push_back(move(a[j++]));
            }
        }
        move(b.begin(), b.end(), a.begin() + l);
    };
    dfs(dfs, 0, a.size());
}

template <class T> void radixSort(vector<T> &a)
{
    // a 是非 bool、至多 64 位的整数数组；按自然升序稳定排序后写回 a，无返回值。
    // 降序可在排序后 reverse；整数相等时无可区分的附加记录，本接口不排序带载荷对象。
    static_assert(is_integral_v<T> && !is_same_v<T, bool> && sizeof(T) <= 8);
    assert(cmp_less_equal(a.size(), numeric_limits<int>::max()));
    if (a.size() < 2)
    {
        return;
    }
    using U = make_unsigned_t<T>; // 无符号键用于定义移位；原值始终保存在 T 中。
    constexpr int W = numeric_limits<U>::digits;
    constexpr int D = min<int>(16, W), B = 1 << D;
    constexpr U sign = is_signed_v<T> ? (U)1 << (W - 1) : 0;
    vector<T> b(a.size());
    vector<int> cnt(B);
    for (int sh = 0; sh < W; sh += D)
    {
        fill(cnt.begin(), cnt.end(), 0);
        for (auto x : a)
        {
            cnt[(((U)x ^ sign) >> sh) & (B - 1)]++;
        }
        partial_sum(cnt.begin(), cnt.end(), cnt.begin());
        for (auto it = a.rbegin(); it != a.rend(); it++)
        {
            auto p = (((U)*it ^ sign) >> sh) & (B - 1);
            b[--cnt[p]] = *it;
        }
        a.swap(b);
    }
}
