#include "../include/full/3_1_001.hpp"

signed main()
{
    DSU d(5);
    assert(d.merge(0, 1));
    assert(d.merge(1, 2));
    assert(!d.merge(0, 2));
    assert(d.same(0, 2) && !d.same(0, 3));
    assert(d.size(1) == 3 && d.size(4) == 1);
    d.init(2);
    assert(!d.same(0, 1));
    cout << "OK\n";
}
