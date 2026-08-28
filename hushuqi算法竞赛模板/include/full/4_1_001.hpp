#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变小则返回 true。
    if (b >= a)
    {
        return false;
    }
    a = b;
    return true;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变大则返回 true。
    if (a >= b)
    {
        return false;
    }
    a = b;
    return true;
}

struct HashValue
{
    // x 是模 2^61-1 的代表元；无符号 64 位与 128 位乘积是 Mersenne 快速约简所必需的。
    static constexpr unsigned long long mod = (1ULL << 61) - 1;
    unsigned long long x = 0;

    HashValue(unsigned long long x = 0) : x((x & mod) + (x >> 61))
    {
        // x 是待规范化整数；构造模 mod 的哈希值。
        if (this->x >= mod)
        {
            this->x -= mod;
        }
    }

    friend HashValue operator+(HashValue a, HashValue b)
    {
        // a、b 是同模哈希值；返回模 mod 的和。
        unsigned long long x = a.x + b.x;
        if (x >= mod)
        {
            x -= mod;
        }
        return x;
    }

    friend HashValue operator-(HashValue a, HashValue b)
    {
        // a、b 是同模哈希值；返回模 mod 的差。
        unsigned long long x = a.x + mod - b.x;
        if (x >= mod)
        {
            x -= mod;
        }
        return x;
    }

    friend HashValue operator*(HashValue a, HashValue b)
    {
        // a、b 是同模哈希值；用 128 位乘积返回模 mod 的积。
        __uint128_t x = (__uint128_t)a.x * b.x;
        return (unsigned long long)(x >> 61) + (unsigned long long)(x & mod);
    }

    friend bool operator==(HashValue, HashValue) = default;

    friend ostream &operator<<(ostream &out, HashValue a)
    {
        // out 是输出流，a 是哈希值；输出 a 的规范代表元并返回 out。
        return out << a.x;
    }
};

inline HashValue hashBase()
{
    // 无参数；返回本进程随机选定的大底数。
    unsigned long long seed = (unsigned long long)chrono::steady_clock::now().time_since_epoch().count();
    seed ^= (unsigned long long)random_device{}() << 32;
    seed ^= random_device{}();
    mt19937_64 mt(seed);
    uniform_int_distribution<unsigned long long> dist(HashValue::mod / 2, HashValue::mod - 2);
    return dist(mt);
}

inline const HashValue HASH_BASE = hashBase();

struct HashBase
{
    // pw[i] 是 HASH_BASE 的 i 次幂，按实际需要延长。
    vector<HashValue> pw{HashValue(1)};

    void ensure(int n)
    {
        // n 是需要支持的最大字符串长度；保证 pw[0..n] 已计算。
        while ((int)pw.size() <= n)
        {
            pw.push_back(pw.back() * HASH_BASE);
        }
    }
};

inline HashBase HASH_POWER;

struct DynStrHash
{
    // pre[i] 是当前串前 i 个字符的哈希。
    vector<HashValue> pre{HashValue(0)};

    int size() const
    {
        // 无参数；返回当前字符串长度。
        return pre.size() - 1;
    }

    void push_back(char c)
    {
        // c 是追加字符；在串尾加入 c。
        HASH_POWER.ensure(size() + 1);
        pre.push_back(pre.back() * HASH_BASE + HashValue((unsigned char)c + 1));
    }

    HashValue get(int l, int r) const
    {
        // l、r 是当前串下标；返回半开子串 [l,r) 的哈希值。
        assert(0 <= l && l <= r && r <= size()); // 调试检查，可删。
        return pre[r] - pre[l] * HASH_POWER.pw[r - l];
    }

    HashValue concat(HashValue value, int l, int r) const
    {
        // value 是已有串的哈希，l、r 指定当前串的半开子串；返回两段拼接后的哈希。
        return value * HASH_POWER.pw[r - l] + get(l, r);
    }
};

struct StrHash
{
    // pre[i] 是原串前 i 个字符的哈希，rev[i] 是反转串前 i 个字符的哈希。
    vector<HashValue> pre, rev;

    StrHash(const string &s) : pre(s.size() + 1), rev(s.size() + 1)
    {
        // s 是原字符串；预处理正向、反向前缀哈希与所需底数幂。
        int n = s.size();
        HASH_POWER.ensure(n);
        for (int i = 0; i < n; i++)
        {
            pre[i + 1] = pre[i] * HASH_BASE + HashValue((unsigned char)s[i] + 1);
            rev[i + 1] = rev[i] * HASH_BASE + HashValue((unsigned char)s[n - 1 - i] + 1);
        }
    }

    int size() const
    {
        // 无参数；返回原字符串长度。
        return pre.size() - 1;
    }

    HashValue get(int l, int r) const
    {
        // l、r 是原串下标；返回半开子串 [l,r) 的正向哈希。
        assert(0 <= l && l <= r && r <= size()); // 调试检查，可删。
        return pre[r] - pre[l] * HASH_POWER.pw[r - l];
    }

    HashValue getrev(int l, int r) const
    {
        // l、r 是原串下标；返回反转 s[l,r) 后的哈希。
        assert(0 <= l && l <= r && r <= size()); // 调试检查，可删。
        int n = size();
        return rev[n - l] - rev[n - r] * HASH_POWER.pw[r - l];
    }

    HashValue concat(HashValue value, int l, int r) const
    {
        // value 是已有串的哈希，l、r 指定原串半开子串；返回两段拼接后的哈希。
        return value * HASH_POWER.pw[r - l] + get(l, r);
    }

    HashValue concatrev(HashValue value, int l, int r) const
    {
        // value 是已有串的哈希，l、r 指定原串半开子串；返回拼接反转子串后的哈希。
        return value * HASH_POWER.pw[r - l] + getrev(l, r);
    }

    static vector<int> match(const string &s, const string &t)
    {
        // s 是文本串，t 是模式串；返回 t 的全部匹配起点，按下标递增排列。
        if (t.size() > s.size())
        {
            return {};
        }
        StrHash a(s), b(t);
        HashValue target = b.get(0, t.size());
        vector<int> ans;
        for (int i = 0; i + (int)t.size() <= (int)s.size(); i++)
        {
            if (a.get(i, i + t.size()) == target)
            {
                ans.push_back(i);
            }
        }
        return ans;
    }
};

struct ShortHash
{
    static constexpr uint64_t M = (1ULL << 61) - 1;
    // B 从 [M / 2, M - 2] 中预先选定；固定常数避免现场再写随机设备。
    static constexpr uint64_t B = 1234567890123456789ULL;
    inline static vector<uint64_t> p{1}; // p[i] 是 B 的 i 次幂。
    vector<uint64_t> h;                  // h[i] 是原串前 i 个字符的哈希。

    static uint64_t red(uint64_t x)
    {
        // x 小于 2M；返回 x 模 M 的规范代表元。
        x = (x & M) + (x >> 61);
        return x >= M ? x - M : x;
    }

    static uint64_t mul(uint64_t a, uint64_t b)
    {
        // a、b 是模 M 代表元；返回二者乘积模 M。
        __uint128_t x = (__uint128_t)a * b;
        return red((uint64_t)(x >> 61) + (uint64_t)(x & M));
    }

    ShortHash(string_view s) : h(s.size() + 1)
    {
        // s 是原字符串；预处理前缀哈希和所需底数幂。
        while (p.size() <= s.size())
        {
            p.push_back(mul(p.back(), B));
        }
        for (int i = 0; i < (int)s.size(); i++)
        {
            h[i + 1] = red(mul(h[i], B) + (unsigned char)s[i] + 1);
        }
    }

    uint64_t get(int l, int r) const
    {
        // l、r 是原串下标；返回半开子串 [l,r) 的哈希。
        return red(h[r] + M - mul(h[l], p[r - l]));
    }
};
