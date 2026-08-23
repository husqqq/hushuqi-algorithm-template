#include "../include/full/3_2_011.hpp"

signed main()
{
    BIT2 b({{1, 2}, {1, 5}, {4, 2}});
    b.add(1, 2, 3);
    b.add(1, 5, 7);
    b.add(4, 2, -2);

    assert(b.prefixSum(1, 9) == 0);
    assert(b.prefixSum(2, 3) == 3);
    assert(b.prefixSum(2, 6) == 10);
    assert(b.prefixSum(5, 3) == 1);

    BIT2 rect({{0, 0}, {0, 3}, {2, 0}, {2, 3}, {1, 1}, {1, 4}, {4, 1}, {4, 4}});
    rect.addRectangle(0, 0, 2, 3, 5);
    rect.addRectangle(1, 1, 4, 4, -2);
    assert(rect.pointGet(0, 0) == 5);
    assert(rect.pointGet(1, 1) == 3);
    assert(rect.pointGet(2, 1) == -2);
    assert(rect.pointGet(1, 3) == -2);
    assert(rect.pointGet(4, 4) == 0);

    cout << "OK\n";
}
