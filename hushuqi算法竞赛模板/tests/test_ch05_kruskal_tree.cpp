#include "../include/full/5_5_010.hpp"

signed main()
{
    KruskalTree a(4);
    a.addEdge(0, 1, -2);
    a.addEdge(1, 2, 3);
    a.addEdge(2, 3, 5);
    a.addEdge(0, 3, 10);
    a.build();
    assert(a.bottleneck(0, 0) == 0);
    assert(a.bottleneck(0, 1) == -2);
    assert(a.bottleneck(0, 2) == 3);
    assert(a.bottleneck(0, 3) == 5);
    a.build();
    assert(a.bottleneck(1, 3) == 5);

    KruskalTree b(4);
    b.addEdge(0, 1, 7);
    b.addEdge(2, 3, 7);
    b.build();
    assert(b.bottleneck(0, 1) == 7);
    assert(!b.bottleneck(0, 2));
}
