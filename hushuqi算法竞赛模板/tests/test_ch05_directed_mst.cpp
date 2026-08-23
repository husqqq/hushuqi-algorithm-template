#include "../include/full/5_5_008.hpp"

signed main()
{
    assert(directedMST(1, 0, {}) == 0);
    assert(!directedMST(3, 0, {{0, 1, 1}}));
    assert(directedMST(3, 0, {{0, 1, 5}, {0, 2, 5}, {1, 2, -10}}) == -5);
    assert(directedMST(3, 0, {{0, 1, 10}, {0, 2, 10}, {1, 2, 1}, {2, 1, 1}}) == 11);

    __int128 w = (__int128)numeric_limits<long long>::max() * 2;
    assert(directedMST(3, 0, {{0, 1, w}, {1, 2, w}}) == w * 2);

    auto fast = dmstTree(3, 0, {{0, 1, 10}, {0, 2, 10}, {1, 2, 1}, {2, 1, 1}});
    assert(fast && fast->weight == 11 && fast->parent == vector<int>({0, 0, 1}));
    assert(!dmstTree(3, 0, {{0, 1, 1}}));
}
