#include "../include/full/3_2_005.hpp"

signed main()
{
    MaxBIT<int> bit(5);
    bit.chmax(2, 7);
    bit.chmax(0, 3);
    bit.chmax(4, 5);
    bit.chmax(2, 6);
    assert(bit.maxPrefix(0) == numeric_limits<int>::lowest());
    assert(bit.maxPrefix(2) == 3);
    assert(bit.maxPrefix(4) == 7);
    assert(bit.maxPrefix(5) == 7);
    cout << "OK\n";
}
