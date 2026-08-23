#include "../include/full/5_8_011.hpp"

signed main()
{
    MinCostBFlow<> g(2);
    g.add(0, 1, 1, 3, 2);
    g.add(1, 0, 0, 3, 1);
    assert(g.solve() == optional<long long>(3));

    MinCostBFlow<> h(2);
    h.add(0, 1, 0, 3, 2);
    h.supply(0, 2);
    h.supply(1, -2);
    assert(h.solve() == optional<long long>(4));

    MinCostBFlow<> bad(2);
    bad.add(0, 1, 1, 1, 0);
    assert(!bad.solve());
}
