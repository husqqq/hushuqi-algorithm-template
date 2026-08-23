#include "../include/full/3_3_005.hpp"

signed main()
{
    vector<int> a{2, -1, 4, 3};
    ZkwSeg<int, plus<int>> seg(a, 0, plus<int>{});
    assert(seg.prod(1, 4) == 6);
    a[2] += 3;
    seg.set(2, a[2]);
    assert(seg.prod(0, 3) == 8);
    assert(seg.prod(2, 2) == 0);
    cout << "OK\n";
}
