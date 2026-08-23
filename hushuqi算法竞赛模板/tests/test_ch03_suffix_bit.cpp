#include "../include/full/3_2_006.hpp"

signed main()
{
    SuffixBIT<int> bit(5);
    vector<int> a{1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++)
    {
        bit.add(i, a[i]);
    }
    assert(bit.sumSuffix(0) == 15);
    assert(bit.sumSuffix(3) == 9);
    assert(bit.sumSuffix(5) == 0);
    assert(bit.sum(1, 4) == 9);
    cout << "OK\n";
}
