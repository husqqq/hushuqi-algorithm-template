#include "../include/full/6_1_010.hpp"

signed main()
{
    auto z = preorderTab({});
    assert(z.par.empty() && z.ord.empty() && z.pos.empty() && z.siz.empty());
    vector<vector<int>> g{{1}, {0}};
    z = preorderTab(g, 1);
    assert((z.par == vector<int>{1, -1}));
    assert((z.ord == vector<int>{1, 0}));
    return 0;
}
