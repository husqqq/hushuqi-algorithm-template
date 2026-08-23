#include "../include/full/3_1_005.hpp"

signed main()
{
    PerDSU empty(0);
    assert(empty.build() == 0);

    PerDSU d(5);
    int r0 = d.build();
    int r1 = d.merge(r0, 0, 1);
    int r2 = d.merge(r1, 1, 2);
    int r3 = d.merge(r0, 3, 4);

    assert(d.size(r0, 0) == 1);
    assert(d.same(r1, 0, 1));
    assert(!d.same(r1, 0, 2));
    assert(d.same(r2, 0, 2));
    assert(d.size(r2, 1) == 3);
    assert(!d.same(r3, 0, 1));
    assert(d.same(r3, 3, 4));
    assert(d.merge(r2, 0, 2) == r2);
    assert(d.t.size() < 50);

    cout << "OK\n";
}
