#include "../include/full/5_5_004.hpp"

signed main()
{
    vector<MEdge> e{{0, 1, 1}, {1, 2, 1}, {2, 3, 1}, {3, 0, 1}};
    assert(minKForest(4, 4, e) == 0);
    assert(minKForest(4, 2, e) == 2);
    assert(minKForest(4, 1, e) == 3);
    assert(!minKForest(3, 1, {{0, 1, 1}}));
    assert(minKForest(3, 1, {{0, 1, -4}, {1, 2, 2}, {0, 2, 3}}) == -2);

    DynamicMST d(3, {{0, 1, 4}, {1, 2, 5}, {0, 2, 9}});
    assert(d.query() == 9);
    d.set(2, 1);
    assert(d.query() == 5);
}
