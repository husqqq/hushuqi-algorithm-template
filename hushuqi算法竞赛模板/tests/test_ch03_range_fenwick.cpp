#include "../include/full/3_2_004.hpp"

signed main()
{
    RangeFenwick<int> bit(vector<int>{1, 2, 3, 4, 5});
    assert(bit.prefixSum(5) == 15);
    bit.rangeAdd(1, 4, 3);
    assert(bit.rangeSum(0, 5) == 24);
    assert(bit.rangeSum(1, 4) == 18);
    bit.rangeAdd(2, 2, 9);
    assert(bit.rangeSum(2, 2) == 0);

    RangeFenwick<int> wide(4);
    wide.rangeAdd(1, 3, 1500000000);
    assert(wide.rangeSum(0, 4) == 3000000000LL);
    cout << "OK\n";
}
