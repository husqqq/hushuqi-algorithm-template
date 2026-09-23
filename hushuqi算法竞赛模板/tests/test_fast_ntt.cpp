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
    auto fast = conv(a, b);
    vector<int> x(a.size()), y(b.size());
    for (int i = 0; i < (int)a.size(); i++) x[i] = a[i].val();
    for (int i = 0; i < (int)b.size(); i++) y[i] = b[i].val();
    auto slow = plain_ntt::conv(x, y);
    assert(fast.size() == slow.size());
    for (int i = 0; i < (int)fast.size(); i++) assert(fast[i].val() == slow[i]);
    return 0;
}
