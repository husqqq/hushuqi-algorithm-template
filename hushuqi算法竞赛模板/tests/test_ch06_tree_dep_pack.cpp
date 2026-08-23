#include "../include/full/6_5_004.hpp"

signed main()
{
    auto f = treeDepPack(vector<int>{2, 2, 3}, vector<long long>{3, 5, 8}, vector<int>{-1, 0, 0}, 5);
    assert(f[5] && *f[5] == 11);
    assert(f[3] == nullopt);
    vector<__int128> val{3, 5, 8};
    auto wide = treeDepPack<__int128>(vector<int>{2, 2, 3}, val, vector<int>{-1, 0, 0}, 5);
    assert(wide[5] && *wide[5] == 11);
    return 0;
}
