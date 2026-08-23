#include "../include/full/3_1_003.hpp"

signed main()
{
    WDSU<int> d(4);
    assert(d.merge(0, 1, 5));
    assert(d.merge(1, 2, -2));
    assert(d.diff(0, 2) == 3);
    assert(d.diff(2, 0) == -3);
    assert(d.merge(0, 2, 3));
    assert(!d.merge(0, 2, 4));
    assert(d.merge(3, 2, 7));
    assert(d.diff(3, 0) == 4);

    XorDSU x(5);
    assert(x.merge(0, 1, 5));
    assert(x.merge(1, 2, 3));
    assert(x.diff(0, 2) == 6);
    assert(x.merge(0, 2, 6));
    assert(!x.merge(0, 2, 7));
    assert(x.merge(3, 2, 4));
    assert(x.diff(3, 0) == 2);
    assert(x.merge(4, 3, 1));
    assert((x.value(4) ^ x.value(3)) == 1);
    cout << "OK\n";
}
