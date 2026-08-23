#pragma once

#include "12_1_001.hpp"

struct SubsetSum
{
    int n;
    vector<unsigned long long> a;

    static size_t words(int n)
    {
        assert(n >= 0);
        return (size_t)(n / 64 + 1);
    }

    SubsetSum(int n) : n(n), a(words(n))
    {
        a[0] = 1;
    }

    void add(int x)
    {
        assert(x >= 0);
        int q = x >> 6;
        int r = x & 63;
        for (int i = (int)a.size() - 1; i >= q; i--)
        {
            unsigned long long z = a[i - q] << r;
            if (r && i > q)
            {
                z |= a[i - q - 1] >> (64 - r);
            }
            a[i] |= z;
        }
        if ((n & 63) != 63)
        {
            a.back() &= (1ULL << ((n & 63) + 1)) - 1;
        }
    }

    bool has(int x) const
    {
        return 0 <= x && x <= n && (a[x >> 6] >> (x & 63) & 1);
    }
};
