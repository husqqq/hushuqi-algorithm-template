#include "../include/full/10_1_016.hpp"

uint64_t slowMul(uint64_t a, uint64_t b)
{
    unsigned __int128 x = carrylessMul(a, b);
    unsigned __int128 p = ((unsigned __int128)1 << 64) | 0b11011;
    for (int i = 127; i >= 64; i--)
    {
        if (x >> i & 1)
        {
            x ^= p << (i - 64);
        }
    }
    return (uint64_t)x;
}

vector<uint64_t> brute(const vector<uint64_t> &a, const vector<uint64_t> &b)
{
    if (a.empty() || b.empty())
    {
        return {};
    }
    vector<uint64_t> c(a.size() + b.size() - 1);
    for (int i = 0; i < (int)a.size(); i++)
    {
        for (int j = 0; j < (int)b.size(); j++)
        {
            c[i + j] ^= slowMul(a[i], b[j]);
        }
    }
    return c;
}

signed main()
{
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int i = 0; i < 10000; i++)
    {
        uint64_t a = rng(), b = rng();
        assert((GF64(a) * GF64(b)).val() == slowMul(a, b));
    }
    for (int tc = 0; tc < 80; tc++)
    {
        int n = tc == 0 ? 600 : rng() % 50 + 1;
        int m = tc == 0 ? 620 : rng() % 50 + 1;
        vector<uint64_t> a(n), b(m);
        for (auto &x : a)
        {
            x = rng();
        }
        for (auto &x : b)
        {
            x = rng();
        }
        auto got = convGF64(a, b);
        assert(got == brute(a, b));
        for (auto x : got)
        {
            agg = agg * 1000000007ULL + x;
        }
    }
    {
        vector<uint64_t> a(1025), b(1025);
        for (auto &x : a)
        {
            x = rng();
        }
        for (auto &x : b)
        {
            x = rng();
        }
        auto got = convGF64(a, b);
        assert(got == brute(a, b));
    }
    cout << agg << '\n';
    return 0;
}
