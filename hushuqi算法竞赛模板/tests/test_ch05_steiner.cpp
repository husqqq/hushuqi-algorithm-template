#include "../include/full/5_5_009.hpp"

signed main()
{
    Steiner a(0);
    assert(a.solve({}) == 0);

    Steiner b(3);
    b.add(0, 1, 2);
    b.add(1, 2, 3);
    assert(b.solve({0, 2}) == 5);
    assert(b.solve({0, 0}) == 0);

    Steiner c(3);
    c.add(0, 1, 1);
    assert(!c.solve({0, 2}));

    Steiner d(4);
    d.add(0, 3, 1);
    d.add(1, 3, 1);
    d.add(2, 3, 1);
    d.add(0, 1, 10);
    d.add(1, 2, 10);
    assert(d.solve({0, 1, 2}) == 3);
    auto tree = d.solveTree({0, 1, 2});
    assert(tree && tree->first == 3 && tree->second == vector<int>({0, 1, 2}));
}
