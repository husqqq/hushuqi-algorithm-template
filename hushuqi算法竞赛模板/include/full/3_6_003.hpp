#include "3_6_002.hpp"

template <int W = 30> struct EraseBinTrie : BinaryTrie<W>
{
    using BinaryTrie<W>::BinaryTrie;
    using BinaryTrie<W>::count;
    using BinaryTrie<W>::tr;
    using BinaryTrie<W>::valid;

    bool erase(unsigned long long x, int v = 1)
    {
        // x 是待删除整数，v 是删除份数；数量不足时不修改并返回 false。
        assert(valid(x) && v > 0); // 调试检查，可删
        if (count(x) < v)
        {
            return false;
        }
        int p = 1;
        tr[p][2] -= v;
        for (int i = W - 1; i >= 0; i--)
        {
            p = tr[p][x >> i & 1];
            tr[p][2] -= v;
        }
        return true;
    }
};
