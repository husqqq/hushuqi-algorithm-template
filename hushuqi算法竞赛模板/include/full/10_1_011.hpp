#pragma once

#include "10_1_006.hpp"

template <class T> struct Ranked
{
    int n = 0; // 集合变量数，掩码范围是 [0,2^n)。
    vector<T> a; // 按掩码优先连续保存，每个掩码依次存放 0..n 秩。

    explicit Ranked(int m = 0) : n(m)
    {
        // m 是集合变量数；构造全零秩表。
        assert(0 <= m && m <= 30); // 调试检查，可删。
        a.resize((1LL << m) * (m + 1));
    }

    T &operator()(int s, int k)
    {
        // s 是集合掩码，k 是秩；返回对应元素的可修改引用。
        return a[s * (n + 1) + k];
    }

    const T &operator()(int s, int k) const
    {
        // s 是集合掩码，k 是秩；返回对应元素的只读引用。
        return a[s * (n + 1) + k];
    }
};

template <class T> Ranked<T> rankedZeta(const vector<T> &a)
{
    // a 是长度为 2 的幂的集合函数；返回按子集大小分层后的子集 Zeta 变换。
    assert(!a.empty() && has_single_bit(a.size())); // 调试检查，可删。
    uint32_t n = countr_zero((unsigned long long)a.size());
    uint32_t m = a.size();
    uint32_t d = n + 1;
    Ranked<T> r(n);
    for (uint32_t s = 0; s < m; s++)
    {
        r.a[(size_t)s * d + popcount(s)] = a[s];
    }
    orFWT<false>(r.a.data(), m, d);
    return r;
}

template <class T> vector<T> rankedMobius(Ranked<T> r)
{
    // r 是按子集大小分层的 Zeta 变换；返回每个集合对应秩层的 Möbius 逆变换系数。
    int n = r.n;
    assert(0 <= n && n <= 30); // 调试检查，可删。
    int m = 1LL << n;
    assert((int)r.a.size() == m * (n + 1)); // 调试检查，可删。
    orFWT<true>(r.a.data(), m, n + 1);
    vector<T> a(m);
    for (uint32_t s = 0; s < (uint32_t)m; s++)
    {
        a[s] = r.a[(size_t)s * (n + 1) + popcount(s)];
    }
    return a;
}

template <class T> vector<T> subsetConv(const vector<T> &a, const vector<T> &b)
{
    // a、b 是等长且长度为 2 的幂的集合函数；返回 c[S]=sum(T subset S) a[T]*b[S\T]。
    assert(a.size() == b.size() && !a.empty() && has_single_bit(a.size())); // 调试检查，可删。
    uint32_t n = countr_zero((unsigned long long)a.size());
    uint32_t m = a.size();
    uint32_t d = n + 1;
    auto f = rankedZeta(a);
    auto g = rankedZeta(b);
    for (uint32_t s = 0; s < m; s++)
    {
        T *x = f.a.data() + (size_t)s * d;
        const T *y = g.a.data() + (size_t)s * d;
        for (int32_t k = n; k >= 0; k--)
        {
            T v{};
            for (int32_t i = 0; i <= k; i++)
            {
                v += x[i] * y[k - i];
            }
            x[k] = v;
        }
    }
    return rankedMobius(move(f));
}
