#include "../include/full/13_1_009.hpp"

unsigned long long slowProduct(unsigned long long a, unsigned long long b, int bits = 64)
{
    if (!a || !b)
    {
        return 0;
    }
    if (a == 1)
    {
        return b;
    }
    if (b == 1)
    {
        return a;
    }
    if (bits == 1)
    {
        return a & b;
    }
    int h = bits / 2;
    unsigned long long mask = (1ULL << h) - 1;
    unsigned long long al = a & mask, ah = a >> h;
    unsigned long long bl = b & mask, bh = b >> h;
    unsigned long long c = slowProduct(al, bl, h);
    unsigned long long z = slowProduct(al ^ ah, bl ^ bh, h) ^ c;
    unsigned long long hh = slowProduct(ah, bh, h);
    unsigned long long low = c ^ slowProduct(hh, 1ULL << (h - 1), h);
    return z << h | low;
}

signed main()
{
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int i = 0; i < 10000; i++)
    {
        unsigned long long a = rng(), b = rng();
        auto x = nimProduct(a, b);
        assert(x == slowProduct(a, b));
        assert(x == nimProduct(b, a));
        assert(nimProduct(a, 1) == a);
        assert(nimProduct32((uint32_t)a, (uint32_t)b) == (uint32_t)slowProduct((uint32_t)a, (uint32_t)b, 32));
        agg = agg * 1000000007ULL + x;
    }
    cout << agg << '\n';
    return 0;
}
