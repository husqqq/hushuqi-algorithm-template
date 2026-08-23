#include "../include/full/5_6_001.hpp"
#include "../include/full/5_6_002.hpp"
#include "../include/full/5_6_003.hpp"
#include "../include/full/5_6_004.hpp"

template <class T>
void check()
{
    T a(0, 0);
    assert(a.matching() == 0);

    T b(4, 70);
    b.addEdge(0, 69);
    b.addEdge(1, 69);
    b.addEdge(1, 1);
    b.addEdge(2, 1);
    b.addEdge(2, 2);
    b.addEdge(3, 2);
    b.addEdge(3, 3);
    assert(b.matching() == 4);
    for (int u = 0; u < 4; u++)
    {
        int v = b.leftMatch()[u];
        assert(v >= 0 && b.matchR()[v] == u);
    }

    T c(2, 2);
    c.addEdge(0, 0);
    assert(c.matching() == 1);
    c.addEdge(1, 1);
    assert(c.matching() == 2);
}

signed main()
{
    check<Hungarian>();
    check<BitHungarian>();
    check<HopcroftKarp>();
    check<HopKarpCSR>();
}
