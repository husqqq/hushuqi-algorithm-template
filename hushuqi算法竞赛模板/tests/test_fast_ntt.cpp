#include "../include/full/10_1_003.hpp"

signed main()
{
    vector<Z> a{1, 2, 3}, b{4, 5};
    assert(conv(a, b) == vector<Z>({4, 13, 22, 15}));

    mt19937_64 rng(0);
    a.resize(1 << 15);
    b.resize(1 << 15);
    for (auto &x : a) x = rng() % mod;
    for (auto &x : b) x = rng() % mod;
    assert(conv(a, b) == plain_ntt::conv(a, b));
    return 0;
}
