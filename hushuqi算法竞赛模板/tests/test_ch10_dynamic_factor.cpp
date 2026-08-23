#include "../include/full/10_2_015.hpp"

signed main()
{
    auto a = factorPrime({1, 0, 0, 0, 1}, 2);
    assert(a.size() == 1 && a[0].e == 4 && a[0].f == vector<int>({1, 1}));
    auto b = factorPrime({2, 0, 0, 1}, 3);
    assert(b.size() == 1 && b[0].e == 3 && b[0].f == vector<int>({2, 1}));
    auto c = factorPrime({1, 0, 1}, 3);
    assert(c.size() == 1 && c[0].e == 1 && c[0].f == vector<int>({1, 0, 1}));
    constexpr int p = 4294967291LL;
    auto d = factorPrime({6, 5, 1}, p, 1);
    assert(d.size() == 2);
    assert(d[0].e == 1 && d[0].f == vector<int>({2, 1}));
    assert(d[1].e == 1 && d[1].f == vector<int>({3, 1}));
    cout << "OK\n";
    return 0;
}
