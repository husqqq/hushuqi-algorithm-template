#include "../include/full/10_1_006.hpp"

signed main()
{
    vector<long long> a{1, 2, 3, 4};
    vector<long long> b{5, 6, 7, 8};
    assert((bitConv(a, b, BitOp::Xor) == vector<long long>{70, 68, 62, 60}));
    assert((bitConv(a, b, BitOp::Or) == vector<long long>{5, 28, 43, 184}));
    assert((bitConv(a, b, BitOp::And) == vector<long long>{103, 52, 73, 32}));
    vector<vector<pair<int, long long>>> factors = {
        {{0, 2}, {1, 3}}, {{0, 5}, {2, 7}}};
    assert((sparseXor(2, factors) ==
            vector<uint64_t>{10, 15, 14, 21}));
    cout << "OK\n";
    return 0;
}
