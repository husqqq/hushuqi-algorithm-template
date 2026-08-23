#include "../include/full/10_1_007.hpp"

signed main()
{
    assert((xorConv({1, 2, 3, 0}, {3, -1, 2, 1}, 4) ==
            vector<long long>{3, 0, 1, 2}));
    assert((xorConv({7, -8}, {9, 10}, 1) == vector<long long>{0, 0}));
    long long m = numeric_limits<long long>::max() - 1;
    assert((xorConv({m - 1, m - 2}, {m - 3, m - 4}, m) ==
            vector<long long>{11, 10}));
    cout << "OK\n";
    return 0;
}
