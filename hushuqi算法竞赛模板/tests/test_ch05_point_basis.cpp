#include "../include/full/5_10_009.hpp"

signed main()
{
    assert(minPtBasis({}).empty());
    vector<vector<int>> g = {{1}, {0, 2}, {}, {2}};
    assert(minPtBasis(g) == vector<int>({0, 3}));
    puts("OK");
    return 0;
}
