#include "../include/full/3_6_008.hpp"

signed main()
{
    BitVec a(130);
    a.set(0);
    a.set(63);
    a.set(64);
    a.set(129);
    assert(a.count() == 4);
    a <<= 1;
    assert(a.test(1) && a.test(64) && a.test(65));
    assert(!a.test(0) && !a.test(129) && a.count() == 3);
    a >>= 64;
    assert(a.test(0) && a.test(1) && a.count() == 2);

    BitVec b(130);
    b.setAll();
    assert(b.count() == 130);
    b ^= a;
    assert(b.count() == 128);
    b &= a;
    assert(b.count() == 0);

    BitVec empty(0);
    empty.setAll();
    empty <<= 0;
    assert(empty.count() == 0);
    cout << "OK\n";
}
