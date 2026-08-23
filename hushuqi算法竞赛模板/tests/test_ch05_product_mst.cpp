#include "../include/full/5_5_005.hpp"

signed main()
{
    ProductMST a(0);
    assert(a.solve() == ProductMST::Cost(0, 0));

    ProductMST b(3);
    b.add(0, 1, 1, 1);
    assert(!b.solve());

    ProductMST c(2);
    c.add(0, 1, 1, 1000);
    c.add(0, 1, 20, 20);
    c.add(0, 1, 1000, 1);
    assert(c.solve() == ProductMST::Cost(20, 20));

    long long w = numeric_limits<long long>::max();
    ProductMST d(3);
    d.add(0, 1, w, w);
    d.add(1, 2, w, w);
    auto z = d.solve();
    assert(z == ProductMST::Cost((__int128)w * 2, (__int128)w * 2));
}
