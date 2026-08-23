#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是当前值，b 是候选值；若 b 更小则更新 a 并返回 true。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是当前值，b 是候选值；若 b 更大则更新 a 并返回 true。
    return a < b ? a = b, true : false;
}

template <class T, int Bits = numeric_limits<T>::digits> class RangeBasis
{
    static_assert(is_integral_v<T> && is_unsigned_v<T>);
    static_assert(1 <= Bits && Bits <= numeric_limits<T>::digits);

    struct Entry
    {
        // v 是主元，p 是它在前缀基中保留的最靠右来源位置。
        T v{};
        int p = -1;
    };

    // pre[r] 是扫描到下标 r 后的带位置线性基。
    vector<array<Entry, Bits>> pre;

    static bool valid(T x)
    {
        // x 是待处理的无符号整数；返回它是否只使用低 Bits 位。
        if constexpr (Bits == numeric_limits<T>::digits)
        {
            return true;
        }
        else
        {
            return !(x >> Bits);
        }
    }

  public:
    explicit RangeBasis(const vector<T> &a) : pre(a.size())
    {
        // a 是静态无符号整数序列；建立每个右端点的前缀基。
        for (int i = 0; i < (int)a.size(); i++)
        {
            assert(valid(a[i])); // 调试检查，可删。
            if (i)
            {
                pre[i] = pre[i - 1];
            }
            T v = a[i];
            int p = i;
            for (int b = Bits - 1; b >= 0 && v; b--)
            {
                if (!(v >> b & 1))
                {
                    continue;
                }
                auto &e = pre[i][b];
                if (e.p < p)
                {
                    swap(e.v, v);
                    swap(e.p, p);
                }
                v ^= e.v;
            }
        }
    }

    T maxXor(int l, int r, T x = 0) const
    {
        // l、r 是闭区间端点，x 是初值；返回可取得的最大异或值。
        assert(0 <= l && l <= r && r < (int)pre.size()); // 调试检查，可删
        assert(valid(x)); // 调试检查，可删。
        for (int b = Bits - 1; b >= 0; b--)
        {
            auto [v, p] = pre[r][b];
            if (p >= l && (x ^ v) > x)
            {
                x ^= v;
            }
        }
        return x;
    }
};
