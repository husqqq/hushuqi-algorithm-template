#include "../include/full/3_5_010.hpp"

signed main()
{
    TopK<int> top(2);
    assert(!top.query());
    top.insert(4);
    top.insert(1);
    top.insert(7);
    top.insert(4);
    assert(top.query() == optional<int>(11));
    assert(top.erase(7) && top.query() == optional<int>(8));
    assert(!top.erase(9));
    assert(top.erase(4) && top.query() == optional<int>(5));

    TopK<int> zero(0);
    zero.insert(9);
    assert(zero.query() == optional<int>(0));
    cout << "OK\n";
}
