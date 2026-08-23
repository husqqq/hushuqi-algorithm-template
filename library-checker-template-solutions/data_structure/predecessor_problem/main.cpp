
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

class FastSet
{
    // n 是元素宇宙 [0,n) 的大小。
    int n = 0;
    // a[0] 按元素位存集合；a[k+1] 按 a[k] 中非空 64 位字存摘要位。
    vector<vector<unsigned long long>> a;

  public:
    FastSet(int n = 0)
    {
        // n 是元素宇宙大小；构造一个初始为空的集合。
        init(n);
    }

    void init(int m)
    {
        // m 是新的元素宇宙大小；清空集合并建立所有 64 叉摘要层。
        assert(m >= 0); // 调试检查，可删
        n = m;
        a.clear();
        do
        {
            a.emplace_back((m + 63) / 64, 0);
            m = (m + 63) / 64;
        } while (m > 1);
    }

    void set(int x)
    {
        // x 是 [0,n) 内元素；把 x 插入集合，已经存在时不改变状态。
        assert(0 <= x && x < n); // 调试检查，可删
        for (auto &level : a)
        {
            int p = x / 64;
            unsigned long long b = 1ULL << (x % 64);
            if (level[p] & b)
            {
                break;
            }
            level[p] |= b;
            x /= 64;
        }
    }

    void reset(int x)
    {
        // x 是 [0,n) 内元素；把 x 从集合删除，原本不存在时不改变状态。
        assert(0 <= x && x < n); // 调试检查，可删
        for (auto &level : a)
        {
            int p = x / 64;
            unsigned long long b = 1ULL << (x % 64);
            if (!(level[p] & b))
            {
                return;
            }
            level[p] &= ~b;
            if (level[p])
            {
                break;
            }
            x /= 64;
        }
    }

    bool test(int x) const
    {
        // x 是 [0,n) 内元素；返回 x 当前是否属于集合。
        assert(0 <= x && x < n); // 调试检查，可删
        return a[0][x / 64] >> (x % 64) & 1ULL;
    }

    int next(int x) const
    {
        // x 是 [0,n] 内起点；返回不小于 x 的最小集合元素，不存在时返回 -1。
        assert(0 <= x && x <= n); // 调试检查，可删
        if (x == n)
        {
            return -1;
        }
        for (int h = 0; h < (int)a.size(); h++)
        {
            int p = x / 64;
            if (p >= (int)a[h].size())
            {
                return -1;
            }
            unsigned long long w = a[h][p] >> (x % 64);
            if (!w)
            {
                x = p + 1;
                continue;
            }
            x += countr_zero(w);
            while (h)
            {
                h--;
                x *= 64;
                x += countr_zero(a[h][x / 64]);
            }
            return x < n ? x : -1;
        }
        return -1;
    }

    int prev(int x) const
    {
        // x 是 [-1,n) 内起点；返回不大于 x 的最大集合元素，不存在时返回 -1。
        assert(-1 <= x && x < n); // 调试检查，可删
        if (x < 0)
        {
            return -1;
        }
        for (int h = 0; h < (int)a.size() && x >= 0; h++)
        {
            int p = x / 64;
            unsigned long long w = a[h][p] << (63 - x % 64);
            if (!w)
            {
                x = p - 1;
                continue;
            }
            x -= countl_zero(w);
            while (h)
            {
                h--;
                x = x * 64 + 63;
                x -= countl_zero(a[h][x / 64]);
            }
            return x;
        }
        return -1;
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    string s;
    cin >> n >> q >> s;
    FastSet st(n);
    for (int i = 0; i < n; ++i)
    {
        if (s[i] == '1')
        {
            st.set(i);
        }
    }
    while (q--)
    {
        int type, x;
        cin >> type >> x;
        if (type == 0)
        {
            st.set(x);
        }
        else if (type == 1)
        {
            st.reset(x);
        }
        else if (type == 2)
        {
            cout << st.test(x) << '\n';
        }
        else if (type == 3)
        {
            cout << st.next(x) << '\n';
        }
        else
        {
            cout << st.prev(x) << '\n';
        }
    }
    return 0;
}
