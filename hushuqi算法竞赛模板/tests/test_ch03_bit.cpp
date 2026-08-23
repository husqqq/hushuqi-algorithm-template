#include "../include/full/3_2_001.hpp"

signed main()
{
    BIT<int> bit(5);
    vector<int> a{2, 0, 3, 1, 4};
    for (int i = 0; i < 5; i++)
    {
        bit.add(i, a[i]);
    }
    assert(bit.sum(0) == 0 && bit.sum(5) == 10);
    assert(bit.sum(1, 4) == 4);
    assert(bit.kth(0) == 0);
    assert(bit.kth(1) == 0);
    assert(bit.kth(2) == 2);
    assert(bit.kth(5) == 3);
    assert(bit.kth(10) == 5);
    cout << "OK\n";
}
