#include "../include/full/3_7_005.hpp"

signed main()
{
    KDBlocks<int> kd;
    kd.add(0, 0, 2);
    kd.add(1, 2, 3);
    kd.add(-1, 1, 5);
    kd.add(3, 3, 7);
    kd.add(1, 2, -1);
    assert(kd.n == 5);
    assert(kd.query(-10, 10, -10, 10) == 16);
    assert(kd.query(1, 1, 2, 2) == 2);
    assert(kd.query(-1, 1, 0, 1) == 7);
    assert(kd.query(9, 10, 9, 10) == 0);
    cout << "OK\n";
}
