#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

template <int W = 30> struct BinaryTrie
{
    // Node[0..2] 依次是 0 儿子、1 儿子、经过节点的元素个数。
    using Node = array<int, 3>;

    // tr[0] 是空节点，tr[1] 是根；节点只增不删。
    vector<Node> tr;

    BinaryTrie(int n = 0)
    {
        // n 是预计插入次数，只用于预留空间；W 是参与比较的二进制位数。
        static_assert(1 <= W && W <= 64);
        if (n)
        {
            tr.reserve(n * (W + 1) + 2);
        }
        clear();
    }

    int newNode()
    {
        // 新建全零节点并返回编号。
        tr.push_back({});
        return (int)tr.size() - 1;
    }

    bool valid(unsigned long long x) const
    {
        // x 是待操作整数；返回 x 是否能由 W 位无符号整数表示。
        if constexpr (W == 64)
        {
            return true;
        }
        else
        {
            return x < ((unsigned long long)1 << W);
        }
    }

    int size() const
    {
        // 返回 Trie 中元素总数，重复值重复计数。
        return tr[1][2];
    }

    bool empty() const
    {
        // 返回 Trie 是否为空。
        return size() == 0;
    }

    void clear()
    {
        // 清空全部元素并重建空节点和根节点。
        tr.clear();
        newNode();
        newNode();
    }

    int count(unsigned long long x) const
    {
        // x 是待查询整数；返回 x 的出现次数。
        assert(valid(x)); // 调试检查，可删
        int p = 1;
        for (int i = W - 1; i >= 0; i--)
        {
            p = tr[p][x >> i & 1];
            if (!p)
            {
                return 0;
            }
        }
        return tr[p][2];
    }

    void insert(unsigned long long x, int v = 1)
    {
        // x 是待插入整数，v 是一次插入的份数，必须为正。
        assert(valid(x) && v > 0); // 调试检查，可删
        int p = 1;
        tr[p][2] += v;
        for (int i = W - 1; i >= 0; i--)
        {
            int b = x >> i & 1;
            if (!tr[p][b])
            {
                tr[p][b] = newNode();
            }
            p = tr[p][b];
            tr[p][2] += v;
        }
    }

    unsigned long long maxXor(unsigned long long x) const
    {
        // x 是询问整数；返回集合中最大的 x xor y。
        assert(valid(x) && !empty()); // 调试检查，可删
        int p = 1;
        unsigned long long ans = 0;
        for (int i = W - 1; i >= 0; i--)
        {
            int b = x >> i & 1;
            int q = tr[p][b ^ 1];
            int c = q && tr[q][2] ? b ^ 1 : b;
            if (c != b)
            {
                ans |= (unsigned long long)1 << i;
            }
            p = tr[p][c];
        }
        return ans;
    }

    unsigned long long maxElement(unsigned long long x) const
    {
        // x 是询问整数；返回一个使 x xor y 最大的集合元素 y。
        return x ^ maxXor(x);
    }

    unsigned long long minXor(unsigned long long x) const
    {
        // x 是询问整数；返回集合中最小的 x xor y。
        assert(valid(x) && !empty()); // 调试检查，可删
        int p = 1;
        unsigned long long ans = 0;
        for (int i = W - 1; i >= 0; i--)
        {
            int b = x >> i & 1;
            int q = tr[p][b];
            int c = q && tr[q][2] ? b : b ^ 1;
            if (c != b)
            {
                ans |= (unsigned long long)1 << i;
            }
            p = tr[p][c];
        }
        return ans;
    }

    unsigned long long minElement(unsigned long long x) const
    {
        // x 是询问整数；返回一个使 x xor y 最小的集合元素 y。
        return x ^ minXor(x);
    }

    unsigned long long kthXor(unsigned long long x, int k) const
    {
        // x 是询问整数，k 从 0 开始；返回第 k 小的 x xor y。
        assert(valid(x) && 0 <= k && k < size()); // 调试检查，可删
        int p = 1;
        unsigned long long ans = 0;
        for (int i = W - 1; i >= 0; i--)
        {
            int b = x >> i & 1;
            int q = tr[p][b];
            int c = q ? tr[q][2] : 0;
            if (k < c)
            {
                p = q;
            }
            else
            {
                k -= c;
                p = tr[p][b ^ 1];
                ans |= (unsigned long long)1 << i;
            }
        }
        return ans;
    }

    unsigned long long kthElement(unsigned long long x, int k) const
    {
        // x 是询问整数，k 从 0 开始；返回使 x xor y 第 k 小的元素 y。
        return x ^ kthXor(x, k);
    }

    int countXorLess(unsigned long long x, unsigned long long k) const
    {
        // x、k 是询问参数；返回满足 (x xor y) < k 的元素个数。
        assert(valid(x)); // 调试检查，可删
        if constexpr (W < 64)
        {
            if (k >= ((unsigned long long)1 << W))
            {
                return size();
            }
        }
        int p = 1, ans = 0;
        for (int i = W - 1; i >= 0 && p; i--)
        {
            int a = x >> i & 1;
            int b = k >> i & 1;
            if (b)
            {
                int q = tr[p][a];
                if (q)
                {
                    ans += tr[q][2];
                }
                p = tr[p][a ^ 1];
            }
            else
            {
                p = tr[p][a];
            }
        }
        return ans;
    }
};
