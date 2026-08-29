#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

class BitVec
{
    // n 是逻辑位数，words 按低位到高位保存 64 位块。
    int n = 0;
    vector<unsigned long long> words;

    void trim()
    {
        // 清除最高字中超出逻辑长度的位。
        if (n % 64)
        {
            words.back() &= (1ULL << (n % 64)) - 1;
        }
    }

  public:
    explicit BitVec(int n = 0) : n(n)
    {
        // n 是逻辑位数；初值全为零。
        assert(n >= 0); // 调试检查，可删
        words.resize(n / 64 + (n % 64 != 0));
    }

    bool test(int x) const
    {
        // x 是位下标；返回该位是否为一。
        assert(0 <= x && x < n); // 调试检查，可删
        return words[x / 64] >> (x % 64) & 1;
    }

    void set(int x, bool value = true)
    {
        // x 是位下标，value 是新位值。
        assert(0 <= x && x < n); // 调试检查，可删
        if (value)
        {
            words[x / 64] |= 1ULL << (x % 64);
        }
        else
        {
            reset(x);
        }
    }

    void reset(int x)
    {
        // x 是位下标；把该位清零。
        assert(0 <= x && x < n); // 调试检查，可删
        words[x / 64] &= ~(1ULL << (x % 64));
    }

    void setAll()
    {
        // 把全部有效位设为一。
        ranges::fill(words, ~0ULL);
        if (!words.empty())
        {
            trim();
        }
    }

    void resetAll()
    {
        // 把全部位清零。
        ranges::fill(words, 0);
    }

    int count() const
    {
        // 返回一的个数。
        int res = 0;
        for (auto word : words)
        {
            res += popcount(word);
        }
        return res;
    }

    BitVec &operator|=(const BitVec &other)
    {
        // other 是等长位向量；逐位或后返回自身。
        assert(n == other.n); // 调试检查，可删
        for (int i = 0; i < (int)words.size(); ++i)
        {
            words[i] |= other.words[i];
        }
        return *this;
    }

    BitVec &operator&=(const BitVec &other)
    {
        // other 是等长位向量；逐位与后返回自身。
        assert(n == other.n); // 调试检查，可删
        for (int i = 0; i < (int)words.size(); ++i)
        {
            words[i] &= other.words[i];
        }
        return *this;
    }

    BitVec &operator^=(const BitVec &other)
    {
        // other 是等长位向量；逐位异或后返回自身。
        assert(n == other.n); // 调试检查，可删
        for (int i = 0; i < (int)words.size(); ++i)
        {
            words[i] ^= other.words[i];
        }
        return *this;
    }

    BitVec &operator<<=(int shift)
    {
        // shift 是非负左移量；越界位丢弃，返回自身。
        assert(shift >= 0); // 调试检查，可删
        if (shift >= n)
        {
            resetAll();
            return *this;
        }
        int block = shift / 64, offset = shift % 64;
        for (int i = (int)words.size() - 1; i >= 0; --i)
        {
            unsigned long long value = i >= block ? words[i - block] << offset : 0;
            if (offset && i > block)
            {
                value |= words[i - block - 1] >> (64 - offset);
            }
            words[i] = value;
        }
        trim();
        return *this;
    }

    BitVec &operator>>=(int shift)
    {
        // shift 是非负右移量；越界位丢弃，返回自身。
        assert(shift >= 0); // 调试检查，可删
        if (shift >= n)
        {
            resetAll();
            return *this;
        }
        int block = shift / 64, offset = shift % 64;
        for (int i = 0; i < (int)words.size(); ++i)
        {
            unsigned long long value = i + block < (int)words.size() ? words[i + block] >> offset : 0;
            if (offset && i + block + 1 < (int)words.size())
            {
                value |= words[i + block + 1] << (64 - offset);
            }
            words[i] = value;
        }
        return *this;
    }
};
