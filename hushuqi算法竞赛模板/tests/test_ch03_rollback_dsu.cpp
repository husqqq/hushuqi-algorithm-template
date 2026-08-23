#include "../include/full/3_1_004.hpp"

signed main()
{
    RollbackDSU d(5);
    int s0 = d.snap();
    assert(d.merge(0, 1));
    int s1 = d.snap();
    assert(d.merge(1, 2));
    assert(!d.merge(0, 2));
    assert(d.size(0) == 3);
    d.rollback(s1);
    assert(d.same(0, 1) && !d.same(0, 2));
    assert(d.size(0) == 2);
    d.rollback(s0);
    assert(!d.same(0, 1) && d.size(0) == 1);
    cout << "OK\n";
}
