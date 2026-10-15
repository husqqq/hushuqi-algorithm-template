#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

struct RollbackDSU
{
    // p[x]<0 表示 x 是根且集合大小为 -p[x]；否则 p[x] 是父亲。
    vector<int> p;
    // st 依次保存每个被改位置及其修改前的 p 值；{-1,0} 表示一次空合并。
    vector<pair<int, int>> st;

    RollbackDSU(int n = 0) : p(n, -1)
    {
        // n 是元素个数；初始时每个元素自成集合。
        assert(n >= 0); // 调试检查，可删
    }

    int find(int x) const
    {
        // x 是元素编号；沿父链返回根，不做路径压缩。
        while (p[x] >= 0)
        {
            x = p[x];
        }
        return x;
    }

    int snap() const
    {
        // 返回当前回滚栈高度，作为之后回滚所用的快照编号。
        return (int)st.size();
    }

    bool merge(int a, int b)
    {
        // a、b 是元素编号；按大小合并并返回是否真正发生合并。
        a = find(a);
        b = find(b);
        if (a == b)
        {
            st.push_back({-1, 0});
            return false;
        }
        if (p[a] > p[b])
        {
            swap(a, b);
        }
        st.push_back({b, p[b]});
        st.push_back({a, p[a]});
        p[a] += p[b];
        p[b] = a;
        return true;
    }

    void rollback(int s)
    {
        // s 是此前由 snap 返回的栈高；撤销其后的所有合并。
        assert(0 <= s && s <= (int)st.size()); // 调试检查，可删
        while ((int)st.size() > s)
        {
            auto [u, x] = st.back();
            st.pop_back();
            if (u >= 0)
            {
                p[u] = x;
            }
        }
    }

    bool same(int a, int b) const
    {
        // a、b 是元素编号；返回二者是否同属一个集合。
        return find(a) == find(b);
    }

    int size(int x) const
    {
        // x 是元素编号；返回其集合大小。
        return -p[find(x)];
    }
};
