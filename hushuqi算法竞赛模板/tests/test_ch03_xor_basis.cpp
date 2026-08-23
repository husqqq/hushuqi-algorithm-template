#include "../include/full/3_8_001.hpp"

signed main()
{
    XorBasis<8> b;
    assert(b.add(5));
    assert(b.add(3));
    assert(!b.add(6));
    assert(b.rk == 2 && b.has(0) && b.has(6) && !b.has(1));
    assert(b.maxXor() == 6);
    vector<unsigned long long> want{0, 3, 5, 6};
    for (int k = 0; k < 4; k++)
    {
        assert(b.kth(k) == optional<unsigned long long>(want[k]));
    }
    assert(!b.kth(4));

    XorBasis<8> c;
    c.add(8);
    b.merge(c);
    assert(b.has(14) && b.maxXor() == 14);
    cout << "OK\n";
}
