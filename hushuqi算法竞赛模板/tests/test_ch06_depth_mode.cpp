#include "../include/full/6_5_003.hpp"

signed main()
{
    assert(depthMode({}).empty());
    vector<vector<int>> g{{1, 2, 3}, {0}, {0}, {0}};
    assert((depthMode(g) == vector<int>{1, 0, 0, 0}));
    return 0;
}
