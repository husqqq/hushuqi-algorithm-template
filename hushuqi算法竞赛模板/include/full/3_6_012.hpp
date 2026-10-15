#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <int W = 30> struct TaggedTrie
{
    using U = unsigned long long;
    static_assert(1 <= W && W <= 64);

    struct Node
    {
        array<int, 2> ch{};
        array<int, 2> cnt{};
    };

    // tr[0] 是空节点，tr[1] 是根；cnt[u][t] 是标签 t 经过 u 的次数。
    vector<Node> tr{Node{}, Node{}};

    bool valid(U x) const
    {
        if constexpr (W == 64)
        {
            return true;
        }
        return x < (U(1) << W);
    }

    int newNode()
    {
        tr.push_back(Node{});
        return (int)tr.size() - 1;
    }

    void add(U x, int t, int v = 1)
    {
        // t 是 0/1 标签，v 为正时插入、为负时删除。
        assert(valid(x) && 0 <= t && t < 2 && v != 0); // 调试检查，可删
        assert(v > 0 || tr[1].cnt[t] >= -v);           // 调试检查，可删
        int p = 1;
        tr[p].cnt[t] += v;
        for (int i = W - 1; i >= 0; i--)
        {
            int b = x >> i & 1;
            if (!tr[p].ch[b])
            {
                assert(v > 0); // 删除时路径必须已经存在
                tr[p].ch[b] = newNode();
            }
            int q = tr[p].ch[b];
            assert(v > 0 || tr[q].cnt[t] >= -v); // 调试检查，可删
            p = q;
            tr[p].cnt[t] += v;
        }
    }

    optional<U> maxXor(U x, int t) const
    {
        // 只在标签 t 的元素中查询最大的 x xor y。
        assert(valid(x) && 0 <= t && t < 2); // 调试检查，可删
        if (!tr[1].cnt[t])
        {
            return nullopt;
        }
        U ans = 0;
        int p = 1;
        for (int i = W - 1; i >= 0; i--)
        {
            int b = x >> i & 1;
            int c = b ^ 1;
            int q = tr[p].ch[c];
            if (!q || !tr[q].cnt[t])
            {
                c ^= 1;
            }
            if (c != b)
            {
                ans |= U(1) << i;
            }
            p = tr[p].ch[c];
        }
        return ans;
    }

    optional<U> maxElement(U x, int t) const
    {
        // 在标签 t 的元素中取使异或值最大的元素；集合为空时返回空。
        auto ans = maxXor(x, t);
        return ans.has_value() ? optional<U>(*ans ^ x) : nullopt;
    }
};
