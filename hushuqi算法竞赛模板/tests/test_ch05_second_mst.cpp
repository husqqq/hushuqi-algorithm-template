#include "../include/full/5_5_002.hpp"

signed main()
{
    auto z = secondMST(0, {});
    assert(z && z->mst == 0 && !z->other && !z->strict);
    assert(!secondMST(2, {}));

    auto a = secondMST(3, {{0, 1, -5}, {1, 2, 2}});
    assert(a && a->mst == -3 && !a->other && !a->strict);

    auto b = secondMST(3, {{0, 1, 1}, {1, 2, 1}, {2, 0, 1}});
    assert(b && b->mst == 2 && b->other == 2 && !b->strict);

    auto c = secondMST(4, {{0, 1, 1}, {1, 2, 1}, {2, 3, 1}, {3, 0, 1}, {0, 2, 2}});
    assert(c && c->mst == 3 && c->other == 3 && c->strict == 4);

    auto d = secondMST(2, {{0, 1, 5}, {0, 1, 5}, {0, 1, 7}});
    assert(d && d->mst == 5 && d->other == 5 && d->strict == 7);
}
