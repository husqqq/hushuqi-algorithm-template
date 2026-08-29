#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

template <int W = 64> struct XorBasis
{
    static_assert(1 <= W && W <= 64);

    static bool valid(unsigned long long x)
    {
        // x 是待处理的整数；返回它是否只使用低 W 位。
        if constexpr (W == 64)
        {
            return true;
        }
        else
        {
            return x < (1ULL << W);
        }
    }

    // a[i] 是最高位为 i 的主元，rk 是当前秩。
    array<unsigned long long, W> a{};
    int rk = 0;

    bool add(unsigned long long x)
    {
        // x 是待插入向量；返回它是否使秩增加。
        assert(valid(x)); // 调试检查，可删。
        for (int i = W - 1; i >= 0; i--)
        {
            if (x >> i & 1)
            {
                if (!a[i])
                {
                    a[i] = x;
                    rk++;
                    return true;
                }
                x ^= a[i];
            }
        }
        return false;
    }

    bool has(unsigned long long x) const
    {
        // x 是查询向量；返回它能否由当前基表示。
        assert(valid(x)); // 调试检查，可删。
        for (int i = W - 1; i >= 0; i--)
        {
            if ((x >> i & 1) && a[i])
            {
                x ^= a[i];
            }
        }
        return x == 0;
    }

    unsigned long long maxXor(unsigned long long x = 0) const
    {
        // x 是初值；返回 x 与张成空间中某值异或后的最大结果。
        assert(valid(x)); // 调试检查，可删。
        for (int i = W - 1; i >= 0; i--)
        {
            chmax(x, x ^ a[i]);
        }
        return x;
    }

    void merge(const XorBasis &o)
    {
        // o 是另一组基；把它的张成空间并入当前基。
        for (auto x : o.a)
        {
            if (x)
            {
                add(x);
            }
        }
    }

    optional<unsigned long long> kth(unsigned long long k) const
    {
        // k 是 0 基排名；返回第 k 小不同可表示值，越界返回空。
        if (rk < 64 && k >= (1ULL << rk))
        {
            return nullopt;
        }
        auto b = a;
        for (int i = 0; i < W; i++)
        {
            if (!b[i])
            {
                continue;
            }
            for (int j = i + 1; j < W; j++)
            {
                if (b[j] >> i & 1)
                {
                    b[j] ^= b[i];
                }
            }
        }
        vector<unsigned long long> v;
        for (auto x : b)
        {
            if (x)
            {
                v.push_back(x);
            }
        }
        unsigned long long ans = 0;
        for (int i = 0; i < (int)v.size(); i++)
        {
            if (k >> i & 1)
            {
                ans ^= v[i];
            }
        }
        return ans;
    }
};
