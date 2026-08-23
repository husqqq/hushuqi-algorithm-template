#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // 若 b 更小则更新 a，返回是否更新。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // 若 b 更大则更新 a，返回是否更新。
    return a < b ? a = b, true : false;
}

class BitVec
{
    // n 是逻辑位数，words 按低位到高位保存 64 位块。
    int n = 0;
    vector<uint64_t> words;

    void trim()
    {
        // 清除最高字中超出逻辑长度的位。
        if (n % 64)
        {
            words.back() &= (uint64_t{1} << (n % 64)) - 1;
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
            words[x / 64] |= uint64_t{1} << (x % 64);
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
        words[x / 64] &= ~(uint64_t{1} << (x % 64));
    }

    void setAll()
    {
        // 把全部有效位设为一。
        ranges::fill(words, ~uint64_t{});
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
        int result = 0;
        for (auto word : words)
        {
            result += popcount(word);
        }
        return result;
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
            uint64_t value = i >= block ? words[i - block] << offset : 0;
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
            uint64_t value = i + block < (int)words.size() ? words[i + block] >> offset : 0;
            if (offset && i + block + 1 < (int)words.size())
            {
                value |= words[i + block + 1] << (64 - offset);
            }
            words[i] = value;
        }
        return *this;
    }
};
