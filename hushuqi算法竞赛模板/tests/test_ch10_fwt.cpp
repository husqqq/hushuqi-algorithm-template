#include "../include/full/10_1_006.hpp"

signed main()
{
    vector<long long> a{1, 2, 3, 4};
    vector<long long> b{5, 6, 7, 8};
    assert((bitConv(a, b, BitOp::Xor) == vector<long long>{70, 68, 62, 60}));
    assert((bitConv(a, b, BitOp::Or) == vector<long long>{5, 28, 43, 184}));
    assert((bitConv(a, b, BitOp::And) == vector<long long>{103, 52, 73, 32}));
    cout << "OK\n";
    return 0;
}
