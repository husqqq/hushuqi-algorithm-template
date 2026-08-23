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

template <class T> struct WDSU
{
    // f[x] 是 x 的父亲；s[x] 仅在 x 为根时表示集合大小。
    vector<int> f, s;
    // w[x] 是 x 到父亲的势能差 val[x]-val[f[x]]。
    vector<T> w;

    WDSU(int n = 0)
    {
        // n 是元素个数；初始势能均为零。
        init(n);
    }

    void init(int n)
    {
        // n 是新的元素个数；清空旧状态并重建单点集合。
        assert(n >= 0); // 调试检查，可删
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        s.assign(n, 1);
        w.assign(n, T{});
    }

    int find(int x)
    {
        // x 是元素编号；压缩父链、累加到根的势能并返回根。
        if (x == f[x])
        {
            return x;
        }
        int p = f[x];
        f[x] = find(p);
        w[x] += w[p];
        return f[x];
    }

    bool merge(int x, int y, T d)
    {
        // 施加 val[y]-val[x]=d；已连通时返回该约束是否一致。
        int a = find(x), b = find(y);
        T v = d + w[x] - w[y];
        if (a == b)
        {
            return v == T{};
        }
        if (s[a] < s[b])
        {
            f[a] = b;
            w[a] = -v;
            s[b] += s[a];
        }
        else
        {
            f[b] = a;
            w[b] = v;
            s[a] += s[b];
        }
        return true;
    }

    bool same(int x, int y)
    {
        // x、y 是元素编号；返回二者是否同属一个集合。
        return find(x) == find(y);
    }

    T diff(int x, int y)
    {
        // x、y 必须连通；返回 val[y]-val[x]。
        assert(same(x, y)); // 调试检查，可删
        return w[y] - w[x];
    }
};

struct XorDSU
{
    // f[x] 是 x 的父亲；s[x] 仅在 x 为根时表示集合大小。
    vector<int> f, s, xr;

    XorDSU(int n = 0)
    {
        // n 是元素个数；初始异或势能均为零。
        init(n);
    }

    void init(int n)
    {
        // n 是新的元素个数；清空旧状态并重建单点集合。
        assert(n >= 0); // 调试检查，可删
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        s.assign(n, 1);
        xr.assign(n, 0);
    }

    int find(int x)
    {
        // x 是元素编号；压缩父链、累积到根的异或值并返回根。
        if (x == f[x])
        {
            return x;
        }
        int p = f[x];
        f[x] = find(p);
        xr[x] ^= xr[p];
        return f[x];
    }

    bool merge(int x, int y, int w)
    {
        // 施加 val[x]^val[y]=w；已连通时返回该约束是否一致。
        int a = find(x), b = find(y);
        int v = xr[x] ^ xr[y] ^ w;
        if (a == b)
        {
            return v == 0;
        }
        if (s[a] < s[b])
        {
            f[a] = b;
            xr[a] = v;
            s[b] += s[a];
        }
        else
        {
            f[b] = a;
            xr[b] = v;
            s[a] += s[b];
        }
        return true;
    }

    bool same(int x, int y)
    {
        // x、y 是元素编号；返回二者是否同属一个集合。
        return find(x) == find(y);
    }

    int diff(int x, int y)
    {
        // x、y 必须连通；返回 val[x]^val[y]。
        assert(same(x, y)); // 调试检查，可删
        return xr[x] ^ xr[y];
    }

    int value(int x)
    {
        // x 是元素编号；把当前根的值视为零，返回 val[x]。
        find(x);
        return xr[x];
    }
};
