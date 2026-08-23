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

struct DSU
{
    // f[x] 是 x 的父亲；s[x] 仅在 x 为根时表示集合大小。
    vector<int> f, s;

    DSU(int n = 0)
    {
        // n 是元素个数；初始时每个元素自成集合。
        init(n);
    }

    void init(int n)
    {
        // n 是新的元素个数；清空旧状态并重建单点集合。
        assert(n >= 0); // 调试检查，可删
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        s.assign(n, 1);
    }

    int find(int x)
    {
        // x 是元素编号；压缩父链并返回集合根。
        return x == f[x] ? x : f[x] = find(f[x]);
    }

    bool merge(int x, int y)
    {
        // x、y 是元素编号；合并两集合并返回是否真正发生合并。
        x = find(x);
        y = find(y);
        if (x == y)
        {
            return false;
        }
        if (s[x] < s[y])
        {
            swap(x, y);
        }
        f[y] = x;
        s[x] += s[y];
        return true;
    }

    bool same(int x, int y)
    {
        // x、y 是元素编号；返回二者是否同属一个集合。
        return find(x) == find(y);
    }

    int size(int x)
    {
        // x 是元素编号；返回其集合大小。
        return s[find(x)];
    }
};
